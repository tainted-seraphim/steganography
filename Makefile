CC    = gcc
FLAGS = -ansi -pedantic -g -Wall

all: main.c steganography.o
	$(CC) $(FLAGS) main.c steganography.o -o steganography

steganography.o: steganography.c
	$(CC) $(FLAGS) -c steganography.c -o steganography.o

clear:
	if [ -f steganography ]; then rm steganography; fi
	if [ -f steganography.o ]; then rm steganography.o; fi

