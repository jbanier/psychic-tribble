set pages 999;
spool waits.lst;

prompt here are waits on host
select
   sysdate,
   substr(event,1,30) event,
   substr(tablespace_name,1,14) tablespace,
   p2 block
from v$session_wait a, dba_data_files b
where
a.p1 = b.file_id
and
(
event = 'lock element waits'
or
event = 'lock element cleanup'
or
event = 'buffer busy waits'
or
event = 'DFS enqueue lock acquisition'
or
event = 'DFS enqueue lock handle'
or
event = 'db file sequential read'
)
;
prompt here are waits on host
select
   sysdate,
   substr(event,1,30) event,
   substr(tablespace_name,1,14) tablespace,
   p2 block
from v$session_wait@SA9_002 a, dba_data_files@SA9_002 b
where
a.p1 = b.file_id
and
(
event = 'lock element waits'
or
event = 'lock element cleanup'
or
event = 'buffer busy waits'
or
event = 'DFS enqueue lock acquisition'
or
event = 'DFS enqueue lock handle'
or
event = 'db file sequential read'
)
;

prompt here are sessions counts waiting for blocks on host
select
       substr(a.tablespace_name,1,12) ts_name,
count(*)
from v$session b, dba_data_files a
 where
b.row_wait_file# = a.file_id
and
  row_wait_file# <> 0
and type='USER'
group by
       substr(a.tablespace_name,1,12)
;

prompt here are sessions counts waiting for blocks on host
select
       substr(a.tablespace_name,1,12) ts_name,
count(*)
from v$session@SA9_002 b, dba_data_files@SA9_002 a
 where
b.row_wait_file# = a.file_id
and
  row_wait_file# <> 0
and type='USER'
group by
       substr(a.tablespace_name,1,12)
;
