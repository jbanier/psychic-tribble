#!/usr/bin/python
# -*- coding:  utf-8 -*-
#Author: jbanier
#Purpose: replay a statement based backup
#This is crude and primitive, you better use mydump

import sys
import MySQLdb as mdb

con = mdb.connect('localhost', 'root', 'voxbone')
backup = open("schemaless_backup.sql", "r")
cur = con.cursor()
cur.execute("select VERSION()")
ver = cur.fetchone()
print "Database version is %s" % ver
foreach line in backup.readlines():
	try:
		cur.execute(line)
	except mdb.error

backup.close():
con.close()
