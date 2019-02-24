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
		return INV_FILE;
	}
	int length=strlen(fileName);
	(*obj)=malloc(sizeof(Calendar));
	if (length<5 || !(fileName[length-4]=='.' && fileName[length-3]=='i' && fileName[length-2]=='c' &&fileName[length-1]=='s')){
		free((*obj));
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
		//fclose(fp);
		free((*obj));
		(*obj)=NULL;
		return INV_FILE;
	}
	char buffer[1000];
	char ** fileData=malloc(sizeof(char*));
	int x=0;
	int y=0;
	int nflag=0;
	int rflag=0;
	while(fgets(buffer,sizeof(buffer),fp)!=NULL){
		if(buffer[0]!=';'){	
		if((buffer[0]==' '||buffer[0]=='\t'||nflag==1||rflag==1)&&x!=0){
			nflag=0;
			rflag=0;
			fileData[x-1]=realloc(fileData[x-1],sizeof(char)*(strlen(buffer)+strlen(fileData[x-1])+1));
			if(buffer[strlen(buffer)-1]!='\n'){
				nflag=1;
			}else{
				buffer[strlen(buffer)]='\0';
				buffer[strlen(buffer)-1]='\0';
			}
			if(buffer[strlen(buffer)-1]!='\r'){
				rflag=1;
			}else{
				buffer[strlen(buffer)-1]='\0';
			}
			int i=0;
			char temp[1000];
			for(i=1;i<strlen(buffer); i=i+1){
				temp[i-1]=buffer[i];
			}
			temp[i-1]='\0';
			strcat(fileData[x-1],temp);
		}else{
			fileData=realloc(fileData,sizeof(char*)*(x+1));
			fileData[x]=malloc(sizeof(char)*(strlen(buffer)+1));
			if(buffer[strlen(buffer)-1]!='\n'){
				nflag=1;
			}else{

				buffer[strlen(buffer)]='\0';
				buffer[strlen(buffer)-1]='\0';
			}
			if(buffer[strlen(buffer)-1]!='\r'){
				rflag=1;
			}else{
				buffer[strlen(buffer)-1]='\0';
			}
			strcpy(fileData[x],buffer);
			x=x+1;
		}
		}
	}
	int i=0;
	for(i=0; i<x;i=i+1){
	//	printf("%s\n",fileData[i]);
	}
	int bflag=0;
	int vflag=0;
	int pflag=0;
	if(strstr(fileData[0],"BEGIN:VCALENDAR")!=NULL && strstr(fileData[x-1],"END:VCALENDAR")!=NULL){
		x=x-1;
	}else{
		freeList((*obj)->events);
		freeList((*obj)->properties);
		for(i=0; i<x; i=i+1){
			free(fileData[i]);
		}
		free(fileData);
		fclose(fp);
		free((*obj));
		(*obj)=NULL;
		return INV_CAL;
	}
	for(i=1; i<x-1; i=i+1){
		if(fileData[i][0]!=';'){
		if(strstr(fileData[i],"VERSION")!=NULL){
			y=0;
			int ctr=0;
			vflag=vflag+1;
			if(strlen(fileData[i])<=8){
				free((*obj)->events);
				freeList((*obj)->properties);
				fclose(fp);
				for(int ix=0; ix<x+1; ix=ix+1){
					free(fileData[ix]);
				}
				free(fileData);
				free((*obj));
				(*obj)=NULL;
				return INV_VER;
			}
			char * string=malloc(sizeof(char)*(strlen(fileData[i])+1));
			for(y=8; y<strlen(fileData[i]);y=y+1){
				if(isalpha(fileData[i][y])||ispunct(fileData[i][y])){
					if(fileData[i][y]!='.'){
						freeList((*obj)->events);
						freeList((*obj)->properties);
						fclose(fp);
						for(int ix=0; ix<x+1;ix=ix+1){
							free(fileData[ix]);
						}
						free(fileData);
						free((*obj));
						(*obj)=NULL;
						free(string);
						return INV_VER;
					}
				}
				string[ctr]=fileData[i][y];
				ctr=ctr+1;
			}
			string[ctr] = '\0';
			(*obj)->version = atof(string);
			free(string);
		}
		else if(strstr(fileData[i],"PRODID:") !=NULL&& strlen(fileData[i])>6){
			y=0;
			int ctr=0;
			pflag=pflag+1;
			if(strlen(fileData[i])<=7){
				freeList((*obj)->events);
				freeList((*obj)->properties);
				fclose(fp);
				for(int ix=0;ix<x;ix=ix+1){
					free(fileData[ix]);
				}
				free(fileData);
				free((*obj));
				(*obj)=NULL;
				return INV_PRODID;
			}
			for(y=7; y<strlen(fileData[i]);y=y+1){
				(*obj)->prodID[ctr]=fileData[i][y];
				ctr=ctr+1;
			}
			(*obj)->prodID[ctr]='\0';
		}
		else{
			if(strstr(fileData[i],"BEGIN:VEVENT")!=NULL){
				bflag=bflag+1;
				Event*e=createEvent(fileData,i,x);
				if(strcmp(e->UID,"ErrorE")==0||strcmp(e->UID,"ErrorD")==0||strcmp(e->UID,"ErrorV")==0||strcmp(e->UID,"ErrorP")==0){
					for(int ix=0; ix<x+1;ix =ix+1){
						free(fileData[ix]);
					}
					freeList((*obj)->properties);
					freeList((*obj)->events);
					fclose(fp);
					free(fileData);
					free((*obj));
					(*obj)=NULL;
				if(strcmp(e->UID,"ErrorE")==0){
					return INV_EVENT;
				}else if(strcmp(e->UID,"ErrorD")==0){
					return INV_DT;
				}else if(strcmp(e->UID,"ErrorV")==0){
					return INV_ALARM;
				}else if(strcmp(e->UID,"ErrorP")==0){
					return INV_EVENT;
				}
				}
				insertBack((*obj)->events,e);
				int veflag=0;
				y=0;
				for(y=i;y<x;y=y+1){
					if(strstr(fileData[y],"END:VEVENT")!=NULL){
						i=y;
						veflag=1;
						break;
					}
				}
				if(veflag==0||e->UID==NULL){
					for(int ix=0; ix<x+1; ix=ix+1){
						free(fileData[ix]);
					}
					freeList((*obj)->properties);
					freeList((*obj)->events);
					fclose(fp);
					free(fileData);
					free((*obj));
					(*obj)=NULL;
					return INV_EVENT;
				}
				y=0;
			}else{
				if(strstr(fileData[i],"BEGIN:VALARM")!=NULL){
					for(int ix=9; ix<x+1; ix=ix+1){
						free(fileData[ix]);
					}
					freeList((*obj)->properties);
					freeList((*obj)->events);
					fclose(fp);
					free(fileData);
					free((*obj));
					(*obj)=NULL;
					return INV_ALARM;
				}
				if(strstr(fileData[i],"END:")==NULL){
				Property *p=createProperty(fileData[i]);
				insertFront((*obj)->properties,p);
				if(strcmp(p->propName,"ErrorP")==0){
					for(i=0; i<x+1;i=i+1){
						free(fileData[i]);
					}
					freeList((*obj)->properties);
					freeList((*obj)->events);
					fclose(fp);
					free(fileData);
					free((*obj));
					(*obj)=NULL;
					return INV_CAL;
				}
				}
			}
				
		}
		}
	}
	if(vflag!=1){
		if(vflag==0){
			for(i=0; i<x+1; i=i+1){
				free(fileData[i]);
			}
			freeList((*obj)->properties);
			freeList((*obj)->events);
			fclose(fp);
			free(fileData);
			free((*obj));
			(*obj)=NULL;
			return INV_CAL;
		}
		for(i=0; i<x+1; i=i+1){
			free(fileData[i]);
		}
		freeList((*obj)->properties);
		freeList((*obj)->events);
		fclose(fp);
		free(fileData);
		free((*obj));
		(*obj)=NULL;
		return DUP_VER;
		
	}else if(bflag==0){
		for(i=0; i<x+1;i=i+1){
			free(fileData[i]);
		}
		freeList((*obj)->properties);
		freeList((*obj)->events);
		fclose(fp);
		free(fileData);
		free((*obj));
		(*obj)=NULL;
		return INV_CAL;
	}else if (pflag!=1){
		for(i=0; i<x+1;i=i+1){
			free(fileData[i]);
		}
		freeList((*obj)->properties);
		freeList((*obj)->events);
		fclose(fp);
		free(fileData);
		free((*obj));
		(*obj)=NULL;
		if(pflag==0){	
			return INV_CAL;
		}
		return DUP_PRODID;
	}
	for(i=0; i<x+1; i=i+1){
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
		char version[100];
		double ver=obj->version;
		snprintf(version,100,"%f",ver);
		char*p=toString(obj->properties);
		int size=strlen(version)+strlen(e)+strlen(p)+2200;
		char * string=malloc(sizeof(char)*size);
		strcpy(string,"Version:");
		strcat(string,version);
		strcat(string,"PRODID:");
		strcat(string,obj->prodID);
		strcat(string,e);
		strcat(string,p);
		free(p);
		free(e);
		return string;
	}
	return "";
}

char* printError (ICalErrorCode err){
	char* descr=malloc(sizeof(char)*100);
	if(err==INV_FILE){
		strcpy(descr,"An error in the file occured");
	}else if(err==INV_CAL){
		strcpy(descr,"An error with the calendar component occured");
	}else if(err==INV_VER){
		strcpy(descr,"An error with the version occured");
	}else if(err==DUP_VER){
		strcpy(descr,"More than one version was found");
	}else if(err==INV_PRODID){
		strcpy(descr,"An error with the prodid occured");
	}else if(err==DUP_PRODID){
		strcpy(descr,"More than one prodid was found");
	}else if(err==INV_EVENT){
		strcpy(descr,"An error in the Event component occured");
	}else if(err==INV_DT){
		strcpy(descr,"An error in the DateTime component occured");
	}else if(err==INV_ALARM){
		strcpy(descr,"An error in the Alarm component occured");
	}else if(err==OTHER_ERROR){
		strcpy(descr,"An undefined error occured");
	}else if(err==OK){
		strcpy(descr,"Successful");
	}else{
		strcpy(descr,"Could not identify error code");
	}
	return descr;
}

ICalErrorCode writeCalendar (char* fileName, const Calendar* obj){
	if(fileName!=NULL && obj!=NULL){
		Calendar * ob=(Calendar*)obj;
		char * object=writeCal(ob);
		if(strcmp(object,"OTHER_ERROR")!=0&&strcmp(object,"INV_ALARM")!=0&&strcmp(object,"INV_EVENT")!=0&&strcmp(object,"INV_CAL")!=0&&strcmp(object,"INV_DT")!=0){
			FILE *fp=fopen(fileName,"w+");
			char * string=malloc(sizeof(char)*200+strlen(object));
			strcpy(string,"BEGIN:VCALENDAR\r\n");
			strcat(string,object);
			strcat(string,"END:VCALENDAR\r\n");
			fprintf(fp,string);
			free(string);
			fclose(fp);
			free(object);
			free(fileName);
		}else{
			free(fileName);
			if(strcmp(object,"OTHER_ERROR")==0){
				free(object);
				return OTHER_ERROR;
			}else if(strcmp(object,"INV_ALARM")==0){
					free(object);
					return INV_ALARM;
			}else if(strcmp(object,"INV_EVENT")==0){
				free(object);
				return INV_EVENT;
			}else if(strcmp(object,"INV_CAL")==0){
				free(object);
				return INV_CAL;
			}else if(strcmp(object,"INV_DT")==0){
				free(object);
				return INV_DT;
			}
		}
		return OK;
	}
	return INV_FILE;
}

ICalErrorCode validateCalendar (const Calendar* obj){
	if(obj!=NULL){
		if(obj->prodID==NULL||strcmp(obj->prodID,"")==0||strlen(obj->prodID)>1000){
			return INV_PRODID;
		}
		if(obj->version<0){
			return INV_VER;
		}
		if(obj->properties==NULL){
			return INV_CAL;
		}
		ListIterator itrP=createIterator(obj->properties);
		Property*prop=nextElement(&itrP);
		while(prop!=NULL){
			if(prop->propName==NULL||prop->propDescr==NULL||strlen(prop->propName)>200){
				return INV_CAL;		
			}
			if(strcmp(prop->propName,"METHOD")!=0&&strcmp(prop->propName,"CALSCALE")!=0){
				return INV_CAL;
			}
			prop=nextElement(&itrP);
		}
		if(obj->events!=NULL){
			ListIterator itrE=createIterator(obj->events);
			Event*ev=nextElement(&itrE);
			while(ev!=NULL){
				if(ev->UID==NULL||strcmp(ev->UID,"")==0||strlen(ev->UID)>1000){
					return INV_EVENT;
				}
				
				DateTime start=ev->startDateTime;
				DateTime create=ev->creationDateTime;
				if(strlen(start.date)!=9||strlen(start.time)!=7){
					return INV_DT;
				}
				if(strlen(create.date)!=9||strlen(create.time)!=7){
					return INV_DT;
				}
				int i=0;
				for(i=0; i<9;i++){
					if(!isdigit(create.date[i])){
						return INV_DT;
					}else if(!isdigit(start.date[i])){
						return INV_DT;
					}
				}
				for(i=0; i<7; i++){
					if(!isdigit(create.time[i])){
						return INV_DT;
					}else if(!isdigit(start.time[i])){
						return INV_DT;
					}
				}
				if(ev->alarms!=NULL){
					ListIterator itrA=createIterator(ev->alarms);
					Alarm* al=nextElement(&itrA);
					while(al!=NULL){
						if(al->properties==NULL){
							return INV_ALARM;
						}
						ListIterator itrAP=createIterator(al->properties);
						Property* apr=nextElement(&itrAP);
						if(al->trigger==NULL||strcmp(al->trigger,"")==0){
							return INV_ALARM;
						}
						if(al->action==NULL||strcmp(al->action,"")==0||strlen(al->action)>200){
							return INV_ALARM;
						}
						while(apr!=NULL){
							if(apr->propName==NULL||apr->propDescr==NULL){	
								return INV_ALARM;
							}
							char arr[5][100]={"ATTACH","DESCRIPTION","SUMMARY","DURATION","REPEAT"};
							int flag=0;
							for (int ctr=0; ctr<5;ctr++){
								if(strcmp(arr[ctr],apr->propName)==0){
									flag=1;
									break;
								}
							}
							if(flag!=1){
								return INV_ALARM;
							}
							apr=nextElement(&itrAP);
						}
						al=nextElement(&itrA);
					}
				}else{
					return INV_EVENT;
				}
				if(ev->properties!=NULL){
					ListIterator itrEA=createIterator(ev->properties);
					Property *pr=nextElement(&itrEA);
					char arr[28][100]={"CREATED","LAST-MODIFIED","SEQUENCE","REQUEST-STATUS","TRANSP","ATTENDEE","CONTACT","ORGANIZER","RECURRENCE-ID","RELATED-TO","URL","EXDATE","RDATE","RRULE","DTEND","DURATION","ATTACH","CATEGORIES","CLASS","COMMENT","DESCRIPTION","GEO","LOCATION","PERCENT-COMPLETE","PRIORITY","RESOURCES","STATUS","SUMMARY"};
					while(pr!=NULL){
						if(pr->propName==NULL ||pr->propDescr==NULL||strlen(pr->propName)>200){
							return INV_EVENT;
						}
						int flag=0;
						for(int x=0; x<28; x++){
							if(strcmp(arr[x],pr->propName)==0){
								flag=1;
								break;
							}
						}
						if (flag!=1){
							return INV_EVENT;
						}
						
						pr=nextElement(&itrEA);
					}
				}else{
					return INV_EVENT;
				}

				ev=nextElement(&itrE);
			}
		}else{
			return INV_CAL;
		}
		return OK;
	}
	return INV_CAL;
}

char* dtToJSON (DateTime prop){
	char * str=malloc(sizeof(char)*100);
	strcpy(str,"{\"date\":\"");
	strcat(str,prop.date);
	strcat(str,"\",\"time\":\"");
	strcat(str,prop.time);
	strcat(str,"\",\"isUTC\":");
	if(prop.UTC==true){
		strcat(str,"true}");
	}else{
		strcat(str,"false}");
	}
	return str;
}

char * eventToJSON( const Event* event){
	char * str=malloc(sizeof(char)*300);
	if(event!=NULL){
		strcpy(str,"{\"startDT\":");
		char* string=dtToJSON(event->startDateTime);
		strcat(str,string);
		strcat(str,",\"numProps\":");
		int propNum=getLength(event->properties);
		char pr[5];
		propNum=propNum+3;
		sprintf(pr,"%d",propNum);
		strcat(str,pr);
		strcat(str,",\"numAlarms\":");
		int almNum=getLength(event->alarms);
		char al[5];
		sprintf(al,"%d",almNum);
		strcat(str,al);
		strcat(str,",\"summary\":\"");
		ListIterator itr=createIterator(event->properties);
		Property* prop=nextElement(&itr);
		while(prop!=NULL){
			if(strcmp(prop->propName,"SUMMARY")==0){
				strcat(str,prop->propDescr);
				break;
			}
			prop=nextElement(&itr);
		}
		strcat(str,"\"}");
	}else{
		strcpy(str,"{}");
	}
	return str;
}

char * eventListToJSON( const List* eventList){
	char *str= malloc(sizeof(char)*3);
	if(eventList!=NULL){
		ListIterator itr=createIterator((List*)eventList);
		Event *ev=nextElement(&itr);
		int i=0;
		while(ev!=NULL){
			i=i+1;
			str=realloc(str,sizeof(char)*(310*i));	
			strcpy(str,"[");
			char* string=eventToJSON(ev);
			strcat(str,string);
			ev=nextElement(&itr);
			if(ev!=NULL){
				strcat(str,",");
			}
		}
		strcat(str,"]");
	}else{
		strcpy(str,"[]");
	}
	return str;
}

char * calendarToJSON( const Calendar*cal){
	char *str=malloc(sizeof(char)*3);
	if(cal!=NULL){
		str=realloc(str,sizeof(char)*1200);
		strcpy(str,"{\"version\":");
		char ver[10];
		snprintf(ver,10,"%f",cal->version);
		strcat(str,ver);
		strcat(str,",\"prodID\":\"");
		strcat(str,cal->prodID);
		strcat(str,"\",\"numProps\":");
		char numProp[5];
		int num=getLength(cal->properties);
		num=num+2;
		sprintf(numProp,"%d",num);
		strcat(str,numProp);
		strcat(str,",\"numEvents\":");
		num=getLength(cal->events);
		char numEvent[5];
		sprintf(numEvent,"%d",num);
		strcat(str,numEvent);
		strcat(str,"}");
	}else{
		strcpy(str,"{}");
	}
	return str;
}

Calendar * JSONtoCalendar(const char* str){
	if(str!=NULL){
		Calendar* obj=malloc(sizeof(Calendar)*1);
		obj->properties=initializeList(&printProperty,&deleteProperty,&compareProperties);
		obj->events=initializeList(&printEvent,&deleteEvent,&compareEvents);
			
		return obj;
	}
	return NULL;
}

Event * JSONtoEvent(const char* str){
	if(str!=NULL){
		Event* obj=malloc(sizeof(Event));
		obj->alarms=initializeList(&printAlarm,&deleteAlarm,&compareAlarms);
		obj->properties=initializeList(&printProperty,&deleteProperty,&compareProperties);

		return obj;
	}
	return NULL;
}

void addEvent(Calendar*cal, Event*toBeAdded){
}

void deleteEvent (void* toBeDeleted){
	if(toBeDeleted !=NULL){
		Event* e=(Event*)toBeDeleted;
		if(e->properties!=NULL){
			freeList(e->properties);
		}
		if(e->alarms!=NULL){
			freeList(e->alarms);
		}
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
	if(toBePrinted!=NULL){
		char*d=printDate(&e->creationDateTime);
		char*ds=printDate(&e->startDateTime);
		char*p=toString(e->properties);
		char*a=toString(e->alarms);
		int size=strlen(d)+strlen(ds)+strlen(p)+strlen(a)+strlen(e->UID)+400;
		char*str=malloc(sizeof(char)*size);
		strcpy(str,"");
		strcat(str,e->UID);
		strcat(str,d);
		strcat(str,ds);
		strcat(str,a);
		strcat(str,p);
		free(d);
		free(ds);
		free(p);
		free(a);
		return str;
	}
	free(e);
	return "";
}

void deleteAlarm (void* toBeDeleted){
	if(toBeDeleted!=NULL){
		Alarm * a=(Alarm*)toBeDeleted;
		if(a->trigger!=NULL){
			free(a->trigger);
		}
		if(a->properties!=NULL){
			freeList(a->properties);
		}
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
		int size=400+strlen(a->action)+strlen(a->trigger);
		char*s=toString(a->properties);
		size=size+strlen(s);
		char* str=malloc(sizeof(char)*size);
		strcpy(str,a->action);
		strcat(str,a->trigger);
		strcat(str,s);
		free(s);
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
		int size=sizeof(Property)+100;
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
		char* str=malloc(sizeof(char)*200);
		strcpy(str,(*d).date);
		strcat(str," ");
		strcat(str,(*d).time);
		strcat(str," ");
		if((*d).UTC){
			strcat(str,"UTC is true");
		}else{
			strcat(str,"UTC is false");
		}
		return str;
	}
	return "";
}
