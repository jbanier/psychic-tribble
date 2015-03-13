#!/usr/bin/python
# -*- coding:  utf-8 -*-
"""
    simple tool to play a sql batch with a minimum of error checking
    and the capacity to resume where it stopped in a previous run
"""
#Author: jbanier
#Purpose: play a sql script file with some error checks and a resume possibility
#This is crude and primitive.

import MySQLdb
import sys

try:
    con = MySQLdb.connect('localhost', 'root', 'root')
except MySQLdb.DatabaseError as e:
    print "An error occured while connectiong to the DB"
    print e
    sys.exit(1)

backup = open("test.sql", "r")
cur = con.cursor()
cur.execute("select VERSION()")
ver = cur.fetchone()
print "Database version is %s" % ver

linenumber = 1
for statement in backup.readlines():
    try:
        sys.stdout.write(statement)
        cur.execute(statement)
        linenumber = linenumber + 1
    except MySQLdb.Error as e:
        sys.stdout.write(("An error occured while running line {0}").format(linenumber))
        print e
        backup.close()
        con.close()
        sys.exit(1)

backup.close()
con.close()
