#Make file for 2750
#Jonah Stegman
#0969112
CC = gcc
CFLAGS = -Wall -g -std=c11 
LIBS = -Lbin

all: list helper parser

test:   list parser
	$(CC) -c $(CFLAGS) src/test.c -o bin/test.o -I include
	$(CC) $(CFLAGS) $(LIBS) -Wl,-R -Wl,bin/ -o bin/test bin/test.o -llist -lcal

Calendar: helper src/CalendarParser.c include/CalendarParser.h include/helper.h
	$(CC) -c $(CFLAGS)-fPIC src/CalendarParser.c -o bin/CalendarParser.o -I include

Linked: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) -c $(CLFAGS) src/LinkedListAPI.c -o bin/LinkedListAPI.o -I include
	
list: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) -shared $(CFLAGS)-o bin/liblist.so -fPIC src/LinkedListAPI.c -I include

helper: src/helper.c
	$(CC) -c $(CFLAGS) -fPIC src/helper.c -o bin/helper.o -I include

parser:  helper Calendar
	$(CC) -shared  -o bin/libcal.so bin/helper.o bin/CalendarParser.o

clean:
	rm -rf bin/*.o bin/*.so
