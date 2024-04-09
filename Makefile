CC = gcc
CFLAGS=-O3 -Werror -Wall -Wextra

BINDIR=bin

.PHONY : all
all : setup loopbackprog udsprog

setup:
	mkdir -p ${BINDIR}

loopbackprog :
	$(CC) $(CFLAGS) -o ${BINDIR}/loopbackprog -Iloopback -I. loopback/loopbackprog.c

udsprog :
	$(CC) $(CFLAGS) -o ${BINDIR}/udsprog -Iuds -I. uds/udsprog.c

.PHONY : clean
clean :
	rm ${BINDIR}/*
