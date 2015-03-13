#!/bin/env python
import json
from StringIO import StringIO
from twisted.internet import reactor
from twisted.protocols import sip
from twisted.internet.protocol import ServerFactory

class MessageDumper():
    def __init__(self, f="obelix.log"):
        self.io = StringIO()

    def save_message(self, message):
        jsonbody = "["
        for kv in message.toString().split('\r\n'):
            try:
                (k,v) = kv.split(': ')
                jsonbody = jsonbody + ('{0}:{1},').format(k,v) 
            except:
                print "Unable to parse " + kv
        jsonbody = jsonbody + "]"
        print message.toString()

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

