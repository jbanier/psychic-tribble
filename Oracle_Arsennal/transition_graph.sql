select t.ta_transitionid,
concat( 
concat (
	   (select ta_state.name from ta_state where ta_state.ta_flowid=2 and ta_state.ta_stateid=t.ta_stateid), 
	   ' -> '
), 
(select ta_state.name from ta_state where ta_state.ta_flowid=2 and ta_state.ta_stateid=t.next_ta_stateid)
) As Bob
from ta_transition t
where ta_flowid=2;
