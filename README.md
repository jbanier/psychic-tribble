psychic-tribble
===============

This is a collections of little things too small to be worth their own repo, typically stuff I had to build to work with clients and might be useful to others.

## asm

a set of small program written in nasm assembly to explain some concept.

## config_files

config files for vim, screen, tmux, compton and i3wm.

## Mysql arsennal

one script to read a mysql table and push the records as document to ElasticSearch.

one script to read a sql file to run it through MySQL with a minimum of error checking.

## Oracle arsennal

A set of sql script to figure out performance problem with Oracle. also feature a dtrace (solaris) script to intercept Oracle API and produce execution logs, useful or app using Oracle and not reporting metrics or improper metrics.

## scapy-dev

a couple of example of usage for scapy, ping wget and the beginging of an Rmcp fuzzer.

## unix tools 

ioreplay s a tool that will replay I/O ! Given the output of strace or truss to capture the IO operations of a software , the tool can replay them. Useful  to established baseline for disk array benchmarking with something that looks like the real life workload. Or at least that was the plan.

obelix is a SIP honeypot, it will happily reply http 200 to anything and log it to Elastic.

nmap is an nse script to be used with nmap that I use to inventorize laptop infected with Asprox/Kuluoz. 

## win tools

A couple of handy powershell script to create a local admin as part of an instal process and another to inventorize the machines registered to AD with local admins. 

## ElastAlert

Nagios Plugin to ship ElastAlert alerts to Nagios via NCPA.
See config.yml for paramters
