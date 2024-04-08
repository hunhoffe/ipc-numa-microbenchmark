CC = gcc
BINDIR=bin

.PHONY : all
all : setup loopbackprog

setup:
	mkdir -p ${BINDIR}

loopbackprog :
	$(CC) -o ${BINDIR}/loopbackprog -Iloopback loopback/loopbackprog.c

.PHONY : clean
clean :
	rm ${BINDIR}/*
