/*******************************************************
 * Author: JBanier
 * Date: 22 Feb 2007
 * Purpose: parse the data file build a linked list of operations
 *******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "listUtil.h"
#include "operation.h"
#include "parser.h"
#include "rep_syscall.h"

#define IOREP_VERSION "0.2"
#define IOREP_AUTHOR "jbanier"

int gb_verbose = 0;

void
verbosePrint (const char *s)
{
  if (gb_verbose)
    {
      printf (s);
    }
}

void
usage ()
{
  printf ("Usage: ioreplay [vh] -f <filename>\n");
  printf ("v verbose   h help\n");
  printf
    ("Where <filename> is the name of the data file,\nThe data file can contain the following instruction:\n");
  printf
    ("open, close, read, write, lseek. 1 instruction per line, arguments are colon separated.\n");
  printf
    ("ie: one would easily implements a copy with the following instructions:\n");
  printf
    ("open,1,/etc/passwd\nread,1,1024\nclose,1\nopen,2,test/copy O_CREAT|O_WRONLY\nwrite,2,1024\nclose,2\n");
  printf
    ("Limitations: 1024 fd max. you've been warned ... set ulimit -n accordingly (+4)\n");
  printf("You can also use the truss2replay perl script to generate files from live excutable\n");
  printf("  On Linux: strace cat /etc/passwd 2>&1 | perl truss2replay.pl > <filename>\n");
  printf ("\nioreplay " IOREP_VERSION " by " IOREP_AUTHOR "\n\n");
}

int
main (int argc, char **argv)
{
  int c;
  int errflg = 0;
  extern char *optarg;
  char *fileName;

  if (argc == 1)
    {
      usage ();
      exit (1);
    }
  while ((c = getopt (argc, argv, "f:hv")) != -1)
    {
      switch (c)
	{
	case 'h':
	  errflg++;
	  break;
	case 'v':
	  gb_verbose++;
	  break;
	case 'f':
	  fileName = optarg;
	  break;
	case ':':
	  usage ();
	  exit (2);
	  break;
	case '?':
	  usage ();
	  exit (3);
	  break;
	}
    }

  if (errflg)
    {
      usage ();
      exit (4);
    }

  verbosePrint ("ioreplay " IOREP_VERSION " by " IOREP_AUTHOR "\n\n");

  verbosePrint ("StartParsing ...");
  if (startParsing (fileName) == -1)
    {
      printf ("\n[ERROR] parsing of data file failed.\n");
      return 5;
    }

  listItem *p = buildOperationList ();

  verbosePrint ("...Done\n");

  verbosePrint ("Starting replay...\n");

  do
    {
      openOperation *t = (openOperation *) (*p).item;
      replaySyscall (t);
      p = (*p).next;
    }
  while ((*p).next != NULL);

  verbosePrint ("Replay completed.\n");
  return 0;
}
