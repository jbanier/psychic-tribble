/******************************************************
 * Author: JBanier
 * Date:  23 Feb 2007.
 * Purpose:  define open options and other contraints per platform ...
 ********************************************************/


#ifndef __REP_SYSCALL_
#define __REP_SYSCALL_

void replaySyscall(void*);

#ifdef __hpux
#define S_RDONLY "O_RDONLY"
#define S_WRONLY "O_WRONLY"
#define S_RDWR   "O_RDWR"
#define S_APPEND "O_APPEND"
#define S_CREAT  "O_CREAT"
#define S_EXCL   "O_EXCL"
#define S_NDELAY "O_NDELAY"
#define S_NOCTTY "O_NOCTTY"
#define S_NONBLOCK "O_NONBLOCK"
#define S_TRUNC  "O_TRUNC"
#define S_DSYNC  "O_DSYNC"
#define S_SYNC "O_SYNC"

#define S_SEEK_SET "SEEK_SET"
#define S_SEEK_CUR "SEEK_CUR"
#define S_SEEK_END "SEEK_END"
#endif

#ifdef __sun
#define S_RDONLY "O_RDONLY"
#define S_WRONLY "O_WRONLY"
#define S_RDWR   "O_RDWR"
#define S_APPEND "O_APPEND"
#define S_CREAT  "O_CREAT"
#define S_EXCL   "O_EXCL"
#define S_NDELAY "O_NDELAY"
#define S_NOCTTY "O_NOCTTY"
#define S_NONBLOCK "O_NONBLOCK"
#define S_TRUNC  "O_TRUNC"
#define S_DSYNC  "O_DSYNC"
#define S_SYNC "O_SYNC"

#define S_SEEK_SET "SEEK_SET"
#define S_SEEK_CUR "SEEK_CUR"
#define S_SEEK_END "SEEK_END"
#endif

#ifdef __linux
#define S_RDONLY "O_RDONLY"
#define S_WRONLY "O_WRONLY"
#define S_RDWR   "O_RDWR"
#define S_APPEND "O_APPEND"
#define S_CREAT  "O_CREAT"
#define S_EXCL   "O_EXCL"
#define S_NDELAY "O_NDELAY"
#define S_NOCTTY "O_NOCTTY"
#define S_NONBLOCK "O_NONBLOCK"
#define S_TRUNC  "O_TRUNC"
#define S_DSYNC  "O_DSYNC"
#define S_SYNC "O_SYNC"

#define S_SEEK_SET "SEEK_SET"
#define S_SEEK_CUR "SEEK_CUR"
#define S_SEEK_END "SEEK_END"
#endif

#endif
