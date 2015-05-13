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
    	h = {}
    	timestamp = time.time()
    	h["headers"] = message.headers.items()
    	h["body"] = message.toString() 
    	h["probeName"] = probeName
    	h["ts"] = timestamp
        print json.dumps(h, 
        	sort_keys=True, indent=2, separators=(',', ': '))

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
