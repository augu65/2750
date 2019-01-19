#Make file for 2750
#Jonah Stegman
#0969112
CC = gcc
CFLAGS = -Wall -g -std=c11 
Libs = -Lbin/

all: list parser test

test: src/test.c
	$(CC) $(CFLAGS) src/test.c -o bin/test.o -I include

Calendar: src/CalendarParser.c include/CalendarParser.h include/helper.h
	$(CC) -c $(CFLAGS) src/CalendarParser.c -o bin/CalendarParser.o -I include

Linked: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) -c $(CLFAGS) src/LinkedListAPI.c -o bin/LinkedListAPI.o -I include
	
list: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) -shared $(CFLAGS) -o bin/liblist.so -fPIC src/LinkedListAPI.c -I include
	
parser: src/CalendarParser.c  include/CalendarParser.h include/helper.h
	$(CC) -shared $(CFLAGS) -o bin/libcal.so -fPIC  src/CalendarParser.c -I include

clean:
	rm -rf bin/*.o bin/*.so
