explain plan for select ta_flowid, (select ta_stateid from ta_transition where ta_transitionid=active_ta_transitionid),
(select next_ta_stateid from ta_transition where ta_transitionid=active_ta_transitionid),
active_ta_transitionid 
from approval 
where approvalid=24

SELECT LPAD('-',2*(LEVEL-1))||OPERATION||' '||OPTIONS||' '|| 
       DECODE(OBJECT_INSTANCE, NULL, OBJECT_NAME,
              TO_CHAR(OBJECT_INSTANCE)||'*'|| OBJECT_NAME) PLAN
FROM PLAN_TABLE
START WITH ID=0
CONNECT BY PRIOR ID = PARENT_ID
ORDER BY ID

rollback;

