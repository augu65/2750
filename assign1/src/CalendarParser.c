#include "CalendarParser.h"
#include <ctype.h>

ICalErrorCode createCalendar (char* fileName, Calendar** obj){
	if( fileName==NULL){
		(*obj)=NULL;
		free(fileName);
		return INV_FILE;

	}
	int length =strlen(fileName);
	(*obj)=malloc(sizeof(Calendar));
	if (length<5 || !(fileName[length-4]=='.' && fileName[length-3]=='i' && fileName[length-2]=='c' &&fileName[length-1]=='s')){
		free(fileName);
		(*obj)=NULL;
		return INV_FILE;
	}
	FILE* fp;
	(*obj)->events=initializeList(&printEvent,&deleteEvent,&compareEvents);
	(*obj)->properties=initializeList(&printProperty,&deleteProperty,&compareProperties);
	fp=fopen(fileName,"r");
	if(fp==NULL){
		freeList((*obj)->events);
		freeList((*obj)->properties);
		free(fp);
		free(fileName);
		(*obj)=NULL;
		return INV_FILE;
	}
	char buffer[1000]="";
	char ** fileData=malloc(sizeof(char*));
	int x=1;
	while(fgets(buffer,sizeof(buffer),fp)){
		fileData=realloc(fileData,sizeof(char*)+x);
		(*fileData)=malloc(sizeof(char)*strlen(buffer));
		strcpy(fileData[x],buffer);
		x=x+1;
	}
	char string[10000]="";
	int i=0;
	int y=0;
	for(i=0; i<x; i=i+1){
		if(strstr("VERSION",fileData[i])!=NULL){
			y=0;
			for(y=8; y<strlen(fileData[i]);y=y+1){
				if(isalpha(fileData[i][y])||ispunct(fileData[i][y])){
					if(fileData[i][y]!='.'){
						freeList((*obj)->events);
						freeList((*obj)->properties);
						free(fileName);
						(*obj)=NULL;
						return INV_FILE;
					}
				}
				strcat(string,&fileData[i][y]);
			}
			(*obj)->version=atof(string);
			strcpy(string,"");

		}
		else if(strstr("PRODID",fileData[i]) !=NULL){
			y=0;
			for(y=0; y<strlen(fileData[i]);y=y+1){
				strcat(string,&fileData[i][y]);
			}
			strcpy((*obj)->prodID,string);
			strcpy(string,"");
		}
	}
	
	for(i=0; i<x; i=i+1){
		free(fileData[i]);
	}
	free(fileData);
	return OK;
}

void deleteCalendar (Calendar*obj){
	freeList(obj->events);
	freeList(obj->properties);
	free(obj);
}

char* printCalendar (const Calendar* obj){
	char version[100];
	sprintf(version,"%g",obj->version);
	char * string=malloc(sizeof(char)*(1000+strlen(version)));
	strcpy(string,"Version:");
	strcat(string,version);
	strcat(string,"PRODID:");
	strcat(string,obj->prodID);
	strcat(string,printEvent(obj->events));
	strcat(string,printProperty(obj->properties));
	return string;
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
	Event* e=(Event*)toBeDeleted;
	freeList(e->properties);
	freeList(e->alarms);
	free(e);	
}

int compareEvents (const void* first, const void* second){
	return 0;
}

char* printEvent (void* toBePrinted){
	return "";
}

void deleteAlarm (void* toBeDeleted){
	Alarm * a=(Alarm*)toBeDeleted;
	free(a->trigger);
	freeList(a->properties);
	free(a);
}

int compareAlarms (const void* first, const void* second){
	return 0;
}

char* printAlarm (void* toBePrinted){
	return "";
}

void deleteProperty (void* toBeDeleted){
	free((Property*)toBeDeleted);
}

int compareProperties (const void* first, const void* second){
	return 0;
}

char* printProperty (void* toBePrinted){
	return "";
}

void deleteDate (void* toBeDeleted){
	free((DateTime*)toBeDeleted);
}

int compareDates (const void* first, const void* second){
	return 0;
}

char* printDate (void* toBePrinted){
	return "";
}
