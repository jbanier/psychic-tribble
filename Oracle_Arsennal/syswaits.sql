select 
       event,
       total_waits,
       round(100 * (total_waits / sum_waits),2) pct_tot_waits,
       time_wait_sec,
       round(100 * (time_wait_sec / sum_secs),2) pct_secs_waits,
       total_timeouts,
       avg_wait_sec
from
(select
       event,
       total_waits,
       round((time_waited / 100),2) time_wait_sec,
       total_timeouts,
       round((average_wait / 100),2) avg_wait_sec
from 
      sys.v_$system_event
where 
      event not in 
      ('lock element cleanup ', 
       'pmon timer ', 
       'rdbms ipc message ', 
       'smon timer ', 
       'SQL*Net message from client ', 
       'SQL*Net break/reset to client ',
       'SQL*Net message to client ',
       'SQL*Net more data to client ',
       'dispatcher timer ',
       'Null event ',
       'parallel query dequeue wait ',
       'parallel query idle wait - Slaves ',
       'pipe get ',
       'PL/SQL lock timer ',
       'slave wait ',
       'virtual circuit status ',
       'WMON goes to sleep') and 
       event not like 'DFS%' and 
       event not like 'KXFX%'),
(select 
        sum(total_waits) sum_waits,
        sum(round((time_waited / 100),2)) sum_secs
 from 
        sys.v_$system_event
 where  
        event not in 
       ('lock element cleanup ', 
        'pmon timer ', 
        'rdbms ipc message ', 
        'smon timer ', 
        'SQL*Net message from client ', 
        'SQL*Net break/reset to client ',
        'SQL*Net message to client ',
        'SQL*Net more data to client ',
        'dispatcher timer ',
        'Null event ',
        'parallel query dequeue wait ',
        'parallel query idle wait - Slaves ',
        'pipe get ',
        'PL/SQL lock timer ',
        'slave wait ',
        'virtual circuit status ',
        'WMON goes to sleep') and 
        event not like 'DFS%' and 
        event not like 'KXFX%')
order by 
   2 desc;
