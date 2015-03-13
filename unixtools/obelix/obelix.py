#!/bin/env python2
"""
    Author: jbanier
    Purpose: have a sip honeypot to feed ES and kibana
"""
import json
from StringIO import StringIO
import time
import datetime
from twisted.internet import reactor
from twisted.protocols import sip
from twisted.internet.protocol import ServerFactory

probeName = "bebr1hon01"

class MessageDumper():
    """
    dumps sip request into a logfile (obelix.log) in json format for easy import
    into elasticsearch or cassandra
    """
    def __init__(self, f="obelix.log"):
        self.io = StringIO()

    def save_message(self, message):
    	timestamp = time.time()
        jsonitems = []
        s = ""

        jsonitems.append("\"probeName\": \"%s\"" % (probeName))
        for n, vs in message.headers.items():
            for v in vs:
            	try:
            		jsonitems.append("\"%s\": %d" % (n, int(v)))
            	except:
                    jsonitems.append("\"%s\": \"%s\"" % (n, v))
        jsonitems.append(datetime.datetime.fromtimestamp( timestamp ).strftime('\"timestamp\": \"%Y-%m-%d %H:%M:%S\"'))
        jsonbody = "{" + (',').join(jsonitems) + "},"
        """add escape of \" \\ \{ 
        add better parsing of composite fields like via, ...
        """
        print jsonbody

class SipProxy(sip.Proxy):
    def __init__(self): 
        sip.Proxy.__init__(self,port=5060) 
        self.tries=0
        self.dumper = MessageDumper()

    def handle_request(self,message,addr):
        self.dumper.save_message(message)
        if message.method=='ACK':
            return 
        r = self.responseFromRequest(200,message)
        r.creationFinished()
        self.deliverResponse(r) 
        self.tries+=1


if __name__ == '__main__':
    print "Starting Obelix...."
    reactor.listenUDP(5060,SipProxy())
    print "Ready to answer request"
    reactor.run()
