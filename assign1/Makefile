CC = gcc
CFLAGS = -Wall -g -std=c11 
Libs = -Lbin/

all: list parser

Calendar: src/CalendarParser.c include/CalendarParser.h
	$(CC) -c $(CFLAGS) src/CalendarParser.c -o bin/CalendarParser.o -I include

Linked: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) -c $(CLFAGS) src/LinkedListAPI.c -o bin/LinkedListAPI.o -I include
	
list: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) -shared $(CFLAGS) -o bin/liblist.so -fPIC src/LinkedListAPI.c -I include
	
parser: src/CalendarParser.c  include/CalendarParser.h
	$(CC) -shared $(CFLAGS) -o bin/libcal.so -fPIC  src/CalendarParser.c -I include

flean:
	rm bin/*.o bin/*.so
