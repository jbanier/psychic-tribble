CFLAGS=-g  
CC=cc
LDFLAGS=

all:	ut ioreplay
clean:
	rm *.o parser ioreplay

ioreplay:	parser.o	ioreplay.o	syscall.o
	${CC} ${LDFLAGS} syscall.o parser.o ioreplay.o -o ioreplay

ut:	parser 

parser: parser.c
	${CC} -D__UT ${CFLAGS} ${LDFLAGS} parser.c -o parser

.c.o:
	${CC} -c ${CFLAGS} $< 
