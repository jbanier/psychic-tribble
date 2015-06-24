#! /usr/bin/env python
#vim: set fileencoding=latin-1
# Author: Jérémie Banier (jbanier@gmail.com)
# Date: Oct. 1 2013
# Purpose: implement wget using scapy :-)
# Based on test add-ons sample
# usage:
# sudo python wget.py
# a, u = wget()

import logging
# Set log level to benefit from Scapy warnings
logging.getLogger("scapy").setLevel(1)

from scapy.all import *

seq = 666

def connect_syn():
    return IP(dst='173.255.253.196')/TCP(dport=80, flags="S", seq)

def connect_ack(p):
    ack = p[TCP][0][1][TCP].seq + 1
    seq = seq + 1
    return IP(dst='blog.zonbi.org')/TCP(dport=80, flags="AP", ack=ack, seq=seq)/"GET /key.txt HTTP/1.0\r\n\r\n"

def data_ack(p):
    ack = p[TCP][0][1][TCP].seq + 1
    seq = seq + 1
    return IP(dst='blog.zonbi.org')/TCP(dport=80, flags="A", ack=ack, seq=seq)

def disconnect_fin(p):
    ack = p[TCP][0][1][TCP].seq + 1
    seq = seq + 1
    return IP(dst='blog.zonbi.org')/TCP(dport=80, flags="F", ack=ack, seq=seq)

def disconnect_ack(p):
    ack = p[TCP][0][1][TCP].seq + 1
    seq = seq + 1
    return IP(dst='blog.zonbi.org')/TCP(dport=80, flags="A", ack=ack, seq=seq)

def wget():
    a, u = sr(connect_syn())
    da, du = sr(connect_ack(a))
    a, u = sr(data_ack(da))
    a, u = sr(disconnect_fin(a))
    a, u = sr(disconnect_ack(a))
    return (da, du)

if __name__ == "__main__":
    interact(mydict=globals(), mybanner="scaget?")
