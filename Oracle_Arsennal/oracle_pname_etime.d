#!/usr/sbin/dtrace -s
/*
 * oracle_pid_etime.d - measure Oracle query execution latency.
 *                     Written for Solaris 10 (needs DTrace).
 *
 * 21-Dec-2007 Banier Jeremie   Created this.
 */

#pragma D option quiet

dtrace:::BEGIN
{
       printf("Tracing... Hit Ctrl-C to end.\n");
}

pid*::OCIStmtPrepare:entry
/execname == $$1/
{
       self->query = copyinstr(arg2);
}

pid*::*OCIStmtExecute*:entry
/execname == $$1/
{
       self->start = timestamp;
}

pid*::*OCIStmtExecute*:return
/self->start && self->query != NULL/
{
       this->elapsed = timestamp - self->start;
       @time[self->query] = quantize(this->elapsed); 
       self->query = 0;
       self->start = 0;
}

pid*::*OCIStmtExecute*:return
/self->start/
{
       this->elapsed = timestamp - self->start;
       @time["Unknow SQL Statement"] = quantize(this->elapsed); 
       self->query = 0;
       self->start = 0;
}

dtrace:::END
{
       printf("Oracle Query execution latency (ns):\n");
       printa(@time); 
}
