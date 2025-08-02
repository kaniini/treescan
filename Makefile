PROG = treescan
SRCS = treescan.c
OBJS = ${SRCS:.c=.o}
CFLAGS ?= -O2 -Wall -ggdb3

${PROG}: ${OBJS}
	${CC} -o $@ $<

.c.o:
	${CC} ${CFLAGS} -c -o $@ $<

clean:
	rm -f ${OBJS} ${PROG}

.PHONY: clean
