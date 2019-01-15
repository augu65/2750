#include "CalendarParser.h"
#include "LinkedListAPI.h"
#include <ctype.h>

ICalErrorCode createCalendar (char* fileName, Calendar** obj){
	int length =strlen(fileName);
	if (length<5 || !(fileName[length-4]=='.' && fileName[length-3]=='i' && fileName[length-2]=='c' &&fileName[length-1]=='s')){
		free(fileName);
		//set obj to null???????????????????????????????	
		return INV_FILE;
	}
	FILE* fp;
	(*obj)=malloc(sizeof(Calendar));
	List* events=initializeList(&printEvent,&deleteEvent,&compareEvents);
	List* properties=initializeList(&printProperty,&deleteProperty,&compareProperties);
	fp=fopen(fileName,"r");
	if(fp==NULL){
		freeList(events);
		freeList(properties);
		free(fp);
		free(fileName);
		free(events);
		free(properties);
		//set obj to null????????????????????????????????????????????
		return INV_FILE;
	}

	
	
	return OK;
}

void deleteCalendar (Calendar*obj){
	freeList(obj->events);
	freeList(obj->properties);
	free(obj);
}

char* printCalendar (const Calendar* obj){
	return "";
}

char* printError (ICalErrorCode err){
	return "";
}

ICalErrorCode writeCalendar (char* fileName, const Calendar* obj){
	return OK;
}

ICalErrorCode validateCalendar (const Calendar* obj){
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
