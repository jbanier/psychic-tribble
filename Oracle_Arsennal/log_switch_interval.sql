-------------------------------------------------------------------------------
--
-- Script:	log_switch_interval.sql
-- Purpose:	to show the shortest log switch interval in living memory
-- For:		8.0 or 8.1
--
-- Copyright:	(c) Ixora Pty Ltd
-- Author:	Steve Adams
--
-------------------------------------------------------------------------------
@save_sqlplus_settings

select
  min(b.first_time - a.first_time) * 1440 * 60  seconds
from
  sys.v_$instance  i,
  sys.v_$log_history  a,
  ( select
      sequence#,
      first_time
    from
      sys.v_$log
    where
      status = 'CURRENT'
    union all
    select
      sequence#,
      first_time
    from
      sys.v_$log_history
  )  b
where
  i.startup_time < a.first_time and
  a.first_time < b.first_time and
  a.sequence# + 1 = b.sequence#
/

@restore_sqlplus_settings
