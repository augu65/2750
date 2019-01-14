CC = gcc
CFLAGS = -Wall -g -std=c11
Libs = -Lbin/

all: list parser

list: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) -shared $(CFLAGS)-o bin/liblist.so -fPIC src/LinkedListAPI.c
	
parser: src/CalenderParser.c include/CalenderParser.h
	$(CC) -shared $(CFLAGS)-o bin/libcal.so -fPIC src/CalenderParser.c

clean:
	rm bin/*.o bin/*.so
