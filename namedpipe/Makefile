CC = gcc

.PHONY : all
all : namedpipeprog

namedpipeprog :
	$(CC) -Wall -Wextra -Werror -pedantic -o namedpipeprog -I. namedpipeprog.c

.PHONY : clean
clean :
	rm namedpipeprog
