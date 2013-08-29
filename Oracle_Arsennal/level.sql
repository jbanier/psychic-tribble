SELECT un.divname,
LPAD ('>', (LEVEL - 1) * 8, '-')
|| un.LAST
|| ' '
|| un.FIRST username,
3-LEVEL level_id 
FROM u un
CONNECT BY PRIOR un.sapid = un.mger_sapid and SAPID <> -2
START WITH un.sapid = '99900401' 
ORDER BY level, un.divname, un.NAME, un.FIRST;