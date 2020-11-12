from elastalert.alerts import Alerter, BasicMatchString
import requests
from requests.auth import HTTPBasicAuth
import logging

class NagiosNcpaAlerter(Alerter):

    # By setting required_options to a set of strings
    # You can ensure that the rule config file specifies all
    # of the options. Otherwise, ElastAlert will throw an exception
    # when trying to load the rule.
    required_options = frozenset(['nrdp_parent', 'nrdp_token', 'nrdp_client_hostname'])

    # Alert is called
    def alert(self, matches):
        self.nrdp_parent = self.rule.get('nrdp_parent', 'localhost')
        self.nrdp_token = self.rule.get('nrdp_token')
        self.nrdp_client_hostname = self.rule.get('nrdp_client_hostname', 'elastalert')
        # Matches is a list of match dictionaries.
        # It contains more than one match when the alert has
        # the aggregation option set
        self.send(self.build_xml(matches))

    # get_info is called after an alert is sent to get data that is written back
    # to Elasticsearch in the field "alert_info"
    # It should return a dict of information relevant to what the alert does
    def get_info(self):
        return {'type': 'Nagios NCPA Alerter'}

    def build_xml(self, matches):
        xml_payload = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><checkresults>"
        for match in matches:
            xml_payload = xml_payload + "<checkresult type=\"service\"><servicename>{}</servicename><hostname>{}</hostname><state>2</state><output>CRITICAL: {}</output></checkresult>".format(str(self.rule['name']),
                            self.nrdp_client_hostname,
                            str(BasicMatchString(self.rule, match)))
        xml_payload = xml_payload + "</checkresults>"
        return xml_payload

    def send(self, xml):
        logging.debug("received matches {}".format(xml))
        x = requests.post(self.nrdp_parent,
                          data={'token': self.nrdp_token, 'XMLDATA': xml, 'cmd': 'submitcheck'},
                          verify=False,
                          allow_redirects=True,
                          auth=HTTPBasicAuth(self.rule.get('nrdp_username'), 
                                             self.rule.get('nrdp_password'))
                        )
        logging.debug(x.text)
        return x.text
