#! /usr/bin/env python
#vim: set fileencoding=latin-1
# Author: Jérémie Banier (jbanier@gmail.com)
# Date: Oct. 29 2013
# Purpose: implement wget using scapy :-)
# Based on test add-ons sample 
# usage:

import logging
# Set log level to benefit from Scapy warnings
logging.getLogger("scapy").setLevel(1)

from scapy.all import *

class Rmcp(Packet):
    name = "Remote Management Control Protocol"
    fields_desc=[ LEShortField("Version",0x06),
        ByteField("Sequence", 0xFF) ,
        XByteField("Type and Class", 0x07) , 
        ByteEnumField("Authentication type", 0, {0:'None', 6:'RMCP+'}), ]

bind_layers( UDP, Rmcp, sport=623 )
bind_layers( UDP, Rmcp, dport=623 )

class IPMISessionLayer(Packet):
    name = "IPMI Session Wrapper"
    fields_desc=[ IntField("Session sequence number", 0),
            XIntField("Session ID", 0),
            ByteField("Message length", 0), ]

class IPMISessionLayer2(Packet):
    name = "IPMI Session Wrapper v2.0+"
    fields_desc=[ ByteEnumField("Payload type", 0x10, 
                {0x10:"Open session request", 0x11:"Open session response",
                    0x12:"RAKP Message 1", 0x13:"RAKP Message 2"
                    }),
            IntField("Session sequence number", 0),
            XIntField("Session ID", 0),
            ByteField("Message length", 0), ]

bind_layers( Rmcp, IPMISessionLayer, {'Authentication type':0} )
bind_layers( Rmcp, IPMISessionLayer2, {'Authentication type':6} )

class IPMILayer(Packet):
    name = "Intelligent Platform Management Interface"
    fields_desc = [ ByteField("Target address", 0x20), ByteEnumField("Target LUN", 0x18, {0x18:"NetFN Application Request"}),
            ByteField("Header checksum", 0xc8), ByteField("Source address", 0x81),
            ByteField("Source LUN", 0x00), 
            ByteEnumField("Command", 0x38, {0x38:"Get Channel Auth. cap."}),
            ByteEnumField("Version compat.", 0x0e, {0x0e:"IPMI v2.0+"}),
            ByteEnumField("Requested privilege level", 0x04, {0x04:"Administrator"}),
            ByteField("Data checksum", 0xb5) ]

bind_layers( IPMISessionLayer, IPMILayer )

if __name__ == "__main__":
    interact(mydict=globals(), mybanner="IPMI fuzzer")
