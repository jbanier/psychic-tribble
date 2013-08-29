select 
   name, 
   value
from 
   sys.v_$sysstat
where 
   name in
    ('consistent changes',
     'consistent gets',
     'db block changes',
     'db block gets',
     'physical reads',
     'physical writes',
     'sorts (disk)',
     'user commits',
     'user rollbacks'
    )
 order by 
1;
