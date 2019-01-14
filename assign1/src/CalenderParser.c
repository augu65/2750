#include "CalenderParser.h"
#include "LinkedListAPI.h"
#include <ctype.h>

ICalErrorCode createCalendar (char* fileName, Calendar** obj){
	return OK;
}

void deleteCalendar (Calendar*obj){
}

char* printCalendar (const Calendar* obj){
	return "";
}

char* printError (ICalErrorCode err){
	return "";
}

ICalErrorCode writeCalendar (const Calendar* obj){
	return OK;
}

void deleteEvent (void* toBeDeleted){
}

int compareEvents (const void* first, const void* second){
	return 0;
}

char* printEvent (void* toBePrinted){
	return "";
}

void deleteAlarm (void* toBeDeleted){
}

int compareAlarms (const void* first, const void* second){
	return 0;
}

char* printAlarm (void* toBePrinted){
	return "";
}

void deleteProperty (void* toBeDeleted){
}

int compareProperties (const void* first, const void* second){
	return 0;
}

char* printProperty (void* toBePrinted){
	return "";
}

void deleteDate (void* toBeDeleted){
}

int compareDates (const void* first, const void* second){
	return 0;
}

char* printDate (void* toBePrinted){
	return "";
}
