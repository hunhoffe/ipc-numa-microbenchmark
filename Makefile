CC = gcc
CFLAGS=-O3 -Werror -Wall -Wextra

BINDIR=bin

.PHONY : all
all : setup loopbackprog

setup:
	mkdir -p ${BINDIR}

loopbackprog :
	$(CC) $(CFLAGS) -o ${BINDIR}/loopbackprog -Iloopback -I. loopback/loopbackprog.c

.PHONY : clean
clean :
	rm ${BINDIR}/*
