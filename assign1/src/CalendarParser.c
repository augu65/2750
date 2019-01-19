/*
 *Calendar parser functions for 2750
 *Jonah Stegman
 *0969112
 */
#include "CalendarParser.h"
#include "helper.h"
#include <ctype.h>

ICalErrorCode createCalendar (char* fileName, Calendar** obj){
	if(fileName==NULL){
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
	x=x-1;
	for(i=0; i<x; i=i+1){
		if(strstr("VERSION",fileData[i])!=NULL&& strlen(fileData[i])>6){
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
		else if(strstr("PRODID",fileData[i]) !=NULL&& strlen(fileData[i])>6){
			y=0;
			for(y=7; y<strlen(fileData[i]);y=y+1){
				strcat(string,&fileData[i][y]);
			}
			strcpy((*obj)->prodID,string);
			strcpy(string,"");
		}
		else{
			if(strstr("BEGIN:",fileData[i])!=NULL){
				if (strstr("VEVENT",fileData[i])!=NULL){
					insertFront((*obj)->events,createEvent(fileData,i,x));
					for(y=i;y<x;y=y+1){
						if(strstr("END:",fileData[y])==NULL){
							i=y+1;
							y=x;
						}
					}
					y=0;
				}
			}else{
				insertFront((*obj)->properties,createProperty(fileData[i]));
			}
				
		}
	}
	
	for(i=0; i<x; i=i+1){
		free(fileData[i]);
	}
	free(fileData);
	return OK;
}

void deleteCalendar (Calendar*obj){
	if(obj !=NULL){
		freeList(obj->events);
		freeList(obj->properties);
		free(obj);
	}
}

char* printCalendar (const Calendar* obj){
	if(obj !=NULL){
		char * version=malloc(sizeof(char)*sizeof(float));
		sprintf(version,"%f",obj->version);
		char * string=malloc(sizeof(char)*(1000+strlen(version)));
		strcpy(string,"Version:");
		strcat(string,version);
		strcat(string,"\r\n");
		strcat(string,"PRODID:");
		strcat(string,obj->prodID);
		strcat(string,printEvent(obj->events));
		strcat(string,printProperty(obj->properties));
		return string;
	}
	return "";
}

char* printError (ICalErrorCode err){
	return "";
}

ICalErrorCode writeCalendar (char* fileName, const Calendar* obj){
	if(fileName!=NULL && obj!=NULL){
		return OK;
	}
	return OK;
}

ICalErrorCode validateCalendar (const Calendar* obj){
	if(obj!=NULL){
		return OK;
	}
	return OK;
}

void deleteEvent (void* toBeDeleted){
	if(toBeDeleted !=NULL){
		Event* e=(Event*)toBeDeleted;
		freeList(e->properties);
		freeList(e->alarms);
		free(e);
	}	
}

int compareEvents (const void* first, const void* second){
	if(first !=NULL && second !=NULL){
	}
	return 0;
}

char* printEvent (void* toBePrinted){
	Event *e=(Event*)toBePrinted;
	int size=strlen(e->UID)+(2*dateLength())+(sizeof(Property)*getLength(e->properties))+(sizeof(Alarm)*getLength(e->alarms));
	char * str=malloc(sizeof(char)*size);
	if(toBePrinted!=NULL){
		strcpy(str,e->UID);
		strcat(str,printDate(&e->creationDateTime));
		strcat(str,printDate(&e->startDateTime));
		strcat(str,toString(e->properties));
		strcat(str,toString(e->alarms));
		return str;
	}
	free(str);
	free(e);
	return "";
}

void deleteAlarm (void* toBeDeleted){
	if(toBeDeleted!=NULL){
		Alarm * a=(Alarm*)toBeDeleted;
		free(a->trigger);
		freeList(a->properties);
		free(a);
	}
}

int compareAlarms (const void* first, const void* second){
	if(first!=NULL&&second!=NULL){
	}
	return 0;
}

char* printAlarm (void* toBePrinted){
	if(toBePrinted!=NULL){
		Alarm*a=(Alarm*)toBePrinted;
		int size=strlen(a->action)+strlen(a->trigger)+(sizeof(Property)*getLength(a->properties));
		char* str=malloc(sizeof(char)*size);
		strcpy(str,a->action);
		strcat(str,a->trigger);
		strcat(str,toString(a->properties));
		return str;	
	}
	return "";
}

void deleteProperty (void* toBeDeleted){
	if(toBeDeleted!=NULL){
		free((Property*)toBeDeleted);
	}
}

int compareProperties (const void* first, const void* second){
	if(first!=NULL && second!=NULL){
	}
	return 0;
}

char* printProperty (void* toBePrinted){
	if(toBePrinted!=NULL){
		Property * p=(Property*)toBePrinted;
		int size=strlen(p->propDescr);
		size=size+strlen(p->propName);
		char* str=malloc(sizeof(char)*size);
		strcpy(str,p->propName);
		strcat(str,p->propDescr);
		return str;
	}
	return "";
}

void deleteDate (void* toBeDeleted){
	if(toBeDeleted!=NULL){
		free((DateTime*)toBeDeleted);
	}
}

int compareDates (const void* first, const void* second){
	if(first!=NULL&&second!=NULL){
	}
	return 0;
}

char* printDate (void* toBePrinted){
	if(toBePrinted!=NULL){
		DateTime* d=(DateTime*)toBePrinted;
		char* str=malloc(sizeof(char)*dateLength());
		strcpy(str,d->date);
		strcat(str,d->time);
		if(d->UTC){
			strcat(str,"true");
		}else{
			strcat(str,"false");
		}
	}
	return "";
}
