#Make file for 2750
#Jonah Stegman
#0969112
CC = gcc
CFLAGS = -Wall -g -std=c11 
LIBS = -Lbin

all: list helper parser

list: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) -shared $(CFLAGS)-o bin/liblist.so -fPIC src/LinkedListAPI.c
	
list: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) -shared $(CFLAGS)-o bin/liblist.so -fPIC src/LinkedListAPI.c -I include

helper: src/helper.c
	$(CC) -c $(CFLAGS) -fPIC src/helper.c -o bin/helper.o -I include

parser:  helper Calendar
	$(CC) -shared  -o bin/libcal.so bin/helper.o bin/CalendarParser.o

clean:
	rm -rf bin/*.o bin/*.so
