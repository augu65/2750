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
	int length=strlen(fileName);
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
		free(fileName);
		free(fp);
		free((*obj));
		(*obj)=NULL;
		return INV_FILE;
	}
	char buffer[1000]="";
	char ** fileData=malloc(sizeof(char*)+1);
	int x=1;
	int rflag=0;
	while(fgets(buffer,sizeof(buffer),fp)!=NULL){
		fileData=realloc(fileData,sizeof(char*)*x);
		if(buffer[0]!=';'&&buffer!=NULL &&buffer[0]!='\n'&&buffer[0]!='\r'){
			if(strstr(buffer,"\r\n")!=NULL){
				rflag=1;
			}else{
				rflag=0;
			}
			if(buffer[0]=='\t'||buffer[0]==' '||(rflag=0&&x!=1)){
				int si=strlen(fileData[x-1]);
				fileData[x-1]=realloc(fileData[x-1],sizeof(char)*si+1);
				strcat(fileData[x-1],buffer);
			}else{
				fileData[x-1]=malloc(sizeof(char)*strlen(buffer)+1);
				strcpy(fileData[x-1],buffer);
				x=x+1;
			}
		}
		strcpy(buffer,"");
	}
/*	for(int ix=0; ix<x-1; ix=ix+1){
		printf("%s",fileData[ix]);
	}*/
	char string[10000]="";
	int i=0;
	int y=0;
	int bflag=0;
	x=x-1;
	if(strstr(fileData[0],"BEGIN:VCALENDAR")!=NULL &&strstr(fileData[x-1],"END:VCALENDAR")!=NULL){
		x=x-1;
	}else{
		freeList((*obj)->events);
		freeList((*obj)->properties);
		for(i=0; i<x; i=i+1){
			free(fileData[i]);
		}
		free(fileData);
		fclose(fp);
		free(fileName);
		free((*obj));
		(*obj)=NULL;
		return INV_FILE;
	}
	for(i=1; i<x; i=i+1){
		if(strstr(fileData[i],"VERSION:")!=NULL){
			y=0;
			for(y=8; y<strlen(fileData[i]);y=y+1){
				if(isalpha(fileData[i][y])||ispunct(fileData[i][y])){
					if(fileData[i][y]!='.'){
						freeList((*obj)->events);
						freeList((*obj)->properties);
						free(fileName);
						free((*obj));
						(*obj)=NULL;
						fclose(fp);
						return INV_FILE;
					}
				}
				strcat(string,&fileData[i][y]);
			}
			(*obj)->version=atof(string);
			strcpy(string,"");

		}
		else if(strstr(fileData[i],"PRODID:") !=NULL&& strlen(fileData[i])>6){
			y=0;
			for(y=7; y<strlen(fileData[i]);y=y+1){
				strcat(string,&fileData[i][y]);
			}
			strcpy((*obj)->prodID,string);
			strcpy(string,"");
		}
		else{
			if(strstr(fileData[i],"BEGIN:VEVENT")!=NULL){
				bflag=bflag+1;
				insertFront((*obj)->events,createEvent(fileData,i,x));
				for(y=i;y<x;y=y+1){
					if(strcmp(fileData[y],"END:VEVENT\r\n")==0){
						i=y;
						break;
					}
				}
				y=0;
			}else{
				Property *p=createProperty(fileData[i]);
				insertFront((*obj)->properties,p);
				free(p);
			}
				
		}
	}
	
	for(i=0; i<x; i=i+1){
		free(fileData[i]);
	}
	fclose(fp);
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
		char*e=toString(obj->events);
		char * version=malloc(sizeof(char)*100);
		snprintf(version,99,"%f",obj->version);
		char * string=malloc(sizeof(char)*(2000+strlen(version)));
		strcpy(string,"Version:");
		strcat(string,version);
		strcat(string,"\r\n");
		strcat(string,"PRODID:");
		strcat(string,obj->prodID);
		strcat(string,e);
		if(getLength(obj->properties)!=0){
			char*p=toString(obj->properties);
			strcat(string,p);
			free(p);
		}
		free(e);
		free(version);
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
	int size=strlen(e->UID)+50+(sizeof(Property)*getLength(e->properties))+(sizeof(Alarm)*getLength(e->alarms));
	char * str=malloc(sizeof(char)*size+1);
	if(toBePrinted!=NULL){
		strcpy(str,e->UID);
		char * d=printDate(&e->creationDateTime);
		char* ds=printDate(&e->startDateTime);
		strcat(str,d);
		strcat(str,ds);
		if(getLength(e->properties)!=0){
			char* p=toString(e->properties);
			strcat(str,p);
			free(p);
		}
		if(getLength(e->alarms)!=0){
			char* a=toString(e->alarms);
			strcat(str,a);
			free(a);
		}
		free(d);
		free(ds);
		return str;
	}
	free(str);
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
		if(getLength(a->properties)!=0){
			char* s=toString(a->properties);
			strcat(str,s);
			free(s);
		}
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
		size=size+200;
		char* str=malloc(sizeof(char)*size);
		strcpy(str,p->propName);
		strcat(str,p->propDescr);
		return str;
	}
	return "";
}

void deleteDate (void* toBeDeleted){
	if(toBeDeleted!=NULL){
		DateTime*d=(DateTime*)toBeDeleted;
		free(d);
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
		char* str=malloc(sizeof(char)*(dateLength()+30));
		strcpy(str,d->date);
		strcat(str," ");
		strcat(str,d->time);
		strcat(str," ");
		if(d->UTC){
			strcat(str,"UTC is true\r\n");
		}else{
			strcat(str,"UTC is false\r\n");
		}
		return str;
	}
	return "";
}
