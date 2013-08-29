select ta_pendingid, 	
(select ta_state.TIMEOUT_DURATION from ta_state where ta_state.ta_stateid=
(select ta_stateid from ta_transition where ta_transition.ta_transitionid=ta_pending.TA_TRANSITIONID)
and ta_state.ta_flowid=1) as ndays,
(select ta_state.TIMEOUT_TA_DURATIONTYPEID from ta_state where ta_state.ta_stateid=
(select ta_stateid from ta_transition where ta_transition.ta_transitionid=ta_pending.TA_TRANSITIONID)
and ta_state.ta_flowid=1) as typed,
sysdate+(select ta_state.TIMEOUT_DURATION from ta_state where ta_state.ta_stateid=
(select ta_stateid from ta_transition where ta_transition.ta_transitionid=ta_pending.TA_TRANSITIONID)
and ta_state.ta_flowid=(select )
) as timeoutdate
from ta_pending
where ta_pendingid=382;

