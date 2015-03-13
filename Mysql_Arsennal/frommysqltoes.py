#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Author :jbanier
Date: 24-02-2015
Purpose: given a table, push it at json to elasticsearch
"""

import sys
from datetime import datetime
import MySQLdb as mdb
from elasticsearch import Elasticsearch

def getMYSQLRecordsinJson( table_name ):
	con = mdb.connect('localhost', 'root', 'root', 'ofpc_session_default');
	es = Elasticsearch()


	with con:
		cur = con.cursor()
		cur.execute("SELECT * FROM session")
		field_names = [i[0] for i in cur.description]
		for i in range(cur.rowcount):
			doc = { 'timestamp': datetime.now(), '_source': table_name }
			row = cur.fetchone()
			field_num = 0

			"""
			For kibana to work we need to have a timestamp and a _source
			"""
			for field in row:
				try:
					doc[field_names[field_num]] = int(field)
				except:
					doc[field_names[field_num]] = field
				field_num += 1

			res = es.index(index="test-index", doc_type='ofpc', body=doc)
			print(res['created'])

if __name__ == "__main__":
    table_name = sys.argv[1]
    getMYSQLRecordsinJson( table_name )
