/**
 * helper functions for calendarparsr.h
 * jonah Stegman
 * 0969112
 */
#include "helper.h"

Event * createEvent(char** lines, int position, int size){
	Event * obj =malloc(sizeof(Event));
	obj->alarms=initializeList(&printAlarm,&deleteAlarm,&compareAlarms);
	obj->properties=initializeList(&printProperty,&deleteProperty,&compareProperties);
	int flag=0;
	int cf=0;
	int sf=0;
	strcpy(obj->UID,"");
	for(position=position+1; position<size; position=position+1){
		if(flag==1){
			strcpy(obj->UID,"ErrorA");
			return obj;
			flag=0;
		}
		if(strstr(lines[position],"END:VEVENT")==NULL){
			if(strstr(lines[position],"UID:")!=NULL){
				strcpy(obj->UID,lines[position]);
				int l=4;
				for(l=4; l<strlen(lines[position]);l++){
					obj->UID[l-4]=lines[position][l];
				}
				obj->UID[l-4]='\0';
				if(strcmp(obj->UID,"")==0){
					strcpy(obj->UID,"ErrorE");
					return obj;
				}

			}else if(strstr(lines[position],"DTSTAMP:")!=NULL){
				cf=1;
			/*	DateTime *d=createDate(lines[position]);
				obj->creationDateTime=(*d);
				free(d);*/
				DateTime d;
				int i=0; 
				int ctr=0;
				int flag=0;
				int tflag=0;
				strcpy(d.date,"");
				strcpy(d.time,"");
				for(i=8;i<strlen(lines[position]); i=i+1){
					if(lines[position][i]=='T'){
						tflag=1;
					}
					if(lines[position][i]!='T'&&flag==0){
						d.date[ctr]=lines[position][i];
						ctr=ctr+1;
					}else if(flag==1&&lines[position][i]!='Z'){
						d.time[ctr]=lines[position][i];
						ctr=ctr+1;
					}else{
						if(flag==0){
							d.date[ctr]='\0';
						}else if(flag==1){
							d.time[ctr]='\0';
						}
						ctr=0;
						flag=flag+1;
					}
				}
				if(lines[position][i-1]=='Z'){
					d.UTC=true;
				}else{
					d.time[ctr]='\0';
					d.UTC=false;
				}
				obj->creationDateTime=d;
				if(strcmp(d.date,"")==0||strcmp(d.time,"")==0 || strlen(lines[position])<23||tflag==0){
					strcpy(obj->UID,"ErrorD");
					return obj;
				}
			}else if(strstr(lines[position],"DTSTART:")!=NULL){
				sf=1;
			/*	DateTime* d=createDate(lines[position]);
				obj->startDateTime=(*d);
				free(d);*/
				DateTime d;
				int i=0;
				int ctr=0;
				int flag=0;
				int tflag=0;
				strcpy(d.date,"");
				strcpy(d.time,"");
				for(i=8; i<strlen(lines[position]);i=i+1){
					if(lines[position][i]=='T'){
						tflag=1;
					}
					if(lines[position][i]!='T' &&flag==0){
						d.date[ctr]=lines[position][i];
						ctr=ctr+1;
					}else if(flag==1 &&lines[position][i]!='Z'){
						d.time[ctr]=lines[position][i];
						ctr=ctr+1;
					}else{
						if(flag==0){
							d.date[ctr]='\0';
						}else if(flag==1){
							d.time[ctr]='\0';
						}
						ctr=0;
						flag=flag+1;
					}
				}
				if(lines[position][i-1]=='Z'){
					d.UTC=true;
				}else{
					d.time[ctr]='\0';
					d.UTC=false;
				}
				obj->startDateTime=d;
				if(strcmp(d.date,"")==0||strcmp(d.time,"")==0||strlen(lines[position])<23||tflag==0){
					strcpy(obj->UID,"ErrorD");
					return obj;
				}
			}else if(strstr(lines[position],"BEGIN:VALARM")!=NULL){
				Alarm *a=createAlarm(lines,position,size);
				insertFront(obj->alarms,a);
				if(strcmp(a->action,"ErrorV")==0||strcmp(a->action,"ErrorP")==0){
					strcpy(obj->UID,"ErrorV");
					return obj;
				}
				int x=position;
				for(x=position; x<size; x=x+1){
					if(strcmp(lines[x],"END:VALARM")==0){
						break;
					}
				}
				position=x;
				if(x==size){
					flag=1;
				}
			}else{
				if(strstr(lines[position],"END:VEVENT")==NULL){
					Property*p=createProperty(lines[position]);
					insertFront(obj->properties,p);
					if(strcmp(p->propName,"ErrorP")==0){
						strcpy(obj->UID,p->propName);
						return obj;
					}
				}
			}
		}else{
			position=size+1;
		}
			
	}
	if(strcmp(obj->UID,"")==0||cf==0||sf==0){
		strcpy(obj->UID,"ErrorE");
	}
	return obj;
}


Alarm * createAlarm(char** lines,int position,int size){
	Alarm * obj=malloc(sizeof(Alarm));
	obj->properties=initializeList(&printProperty,&deleteProperty,&compareProperties);
	int flag=0;
	int aflag=0;
	int tflag=0;
	int eflag=0;
	obj->trigger=malloc(sizeof(char)*strlen(lines[position])+10);
	strcpy(obj->action,"");
	strcpy(obj->trigger,"");
	for (position=position+1; position<size; position=position+1){
		if(strstr(lines[position],"ACTION")!=NULL){
			int x =0;
			int ln=0;
			aflag=1;
			for (x=0; x<strlen(lines[position]);x=x+1){
				if(flag==1){
					obj->action[ln]=lines[position][x];
					ln=ln+1;
				}else if (lines[position][x]==';'||lines[position][x]==':'){
					flag=1;
				}
			}
			obj->action[ln]='\0';
			if(flag!=1||strcmp(obj->action,"")==0){
				strcpy(obj->action,"ErrorV");
			}
			flag=0;
		}else if(strstr(lines[position],"TRIGGER")!=NULL){
			int x=0;
			tflag=1;
			int ln=0;
			int f=0;
			for(x=0; x<strlen(lines[position]);x=x+1){
				if(flag==1){
					if(f==0){
						obj->trigger=realloc(obj->trigger,sizeof(char)*strlen(lines[position])+10);
						f=1;
					}
					obj->trigger[ln]=lines[position][x];
					ln=ln+1;
				}else if(lines[position][x]==';'||lines[position][x]==':'){
					flag=1;
				}
			}
			obj->trigger[ln]='\0';
			if(flag!=1||strcmp(obj->trigger,"")==0){
				strcpy(obj->action,"ErrorV");
			}
			flag=0;
		}else{
			if(strstr(lines[position],"END:VALARM")==NULL){
				Property*p=createProperty(lines[position]);
				if(strcmp(p->propName,"ErrorP")==0){
					strcpy(obj->action,"ErrorP");
					free(p);
					break;
				}
				insertFront(obj->properties,p);
			}else{
				eflag=1;
				break;
			}
		}
	}
	if(aflag==0||tflag==0||eflag==0||strcmp(obj->action,"ErrorP")==0){
		strcpy(obj->action,"ErrorV");
	}
	return obj;
}

Property * createProperty(char* line){
	Property* obj=malloc(sizeof(Property)+sizeof(char)* strlen(line)+10);
	int i=0;
	int flag=0;
	int ctr=0;
	strcpy(obj->propDescr,"");
	strcpy(obj->propName,"");
	if(line!=NULL){
		for(i=0; i< strlen(line); i=i+1){
			if(flag==1){
				obj->propDescr[ctr]=line[i];
				ctr=ctr+1;
			}else if(line[i]!=':' &&line[i]!=';'&& flag !=1){
				
				obj->propName[ctr]=line[i];
				ctr=ctr+1;
				
			}else{
				if(flag==0){
					obj->propName[ctr]='\0';
				}
				ctr=0;
				flag=1;
			}
		}
		obj->propDescr[ctr]='\0';
		if(flag==0||strcmp(obj->propDescr,"")==0||strcmp(obj->propName,"")==0){
			strcpy(obj->propName,"ErrorP");
		}
	}
	return obj;
}

DateTime  *createDate(char* line){
	DateTime* obj=malloc(sizeof(DateTime)+1);
	int i=0;
	int ctr=0;
	int flag=0;
	strcpy(obj->date,"");
	strcpy(obj->time,"");
	for (i=8; i<strlen(line); i=i+1){
		if(line[i]!='T'&&flag==0){
			obj->date[ctr]=line[i];
			ctr=ctr+1;
		}else if(flag==1&&line[i]!='Z'){
			obj->time[ctr]=line[i];
			ctr=ctr+1;
		}else{
			ctr=0;
			flag=flag+1;
		}
	}
	if(line[i-1]=='Z'){
		obj->UTC=true;
	}else{
		obj->UTC=false;
	}
	return obj;
}

char * writeCal(const Calendar * obj){
	if( obj!=NULL){
		ListIterator itrE=createIterator(obj->events);
		ListIterator itrP=createIterator(obj->properties);
		Event* data=nextElement(&itrE);
		char* object=malloc(sizeof(char)*1500);
		strcpy(object,"VERSION:");
		char version[100];
		double ver=obj->version;
		snprintf(version,100,"%f",ver);
		strcat(object,version);
		strcat(object,"\r\n");
		strcat(object,"PRODID:");
		strcat(object,obj->prodID);
		strcat(object,"\r\n");
		while(data!=NULL){
			char* str=writeEvent(data);
			object=realloc(object,sizeof(char)*strlen(str)+strlen(object)+100); 
			strcat(object,str);
			data=nextElement(&itrE);
			free(str);
		}
		Property * prop=nextElement(&itrP);
		while(prop!=NULL){
			char*str=writeProperty(prop);
			object=realloc(object, sizeof(char)*strlen(str)+strlen(object)+100);
			strcat(object,str);
			prop=nextElement(&itrP);
			free(str);
		}
		return object;
	}
	return "INV_CAL";
}
char * writeEvent(Event * obj){
	if(obj!=NULL){
		char* object=malloc(sizeof(char)*1500);
		ListIterator itrA=createIterator(obj->alarms);
		ListIterator itrP=createIterator(obj->properties);
		strcpy(object,"BEGIN:VEVENT\r\n");
		strcat(object,"UID:");
		strcat(object,obj->UID);
		strcat(object,"\r\n");
		DateTime dt=obj->creationDateTime;
		DateTime dts=obj->startDateTime;
		strcat(object,"DTSTART:");
		strcat(object,dts.date);
		strcat(object,"T");
		strcat(object,dts.time);
		if(dts.UTC==true){
			strcat(object,"Z");
		}
		strcat(object,"\r\n");
		strcat(object,"DTSTAMP:");
		strcat(object,dt.date);
		strcat(object,"T");
		strcat(object,dt.time);
		if(dt.UTC==true){
			strcat(object,"Z");
		}
		strcat(object,"\r\n");
		Alarm*al=nextElement(&itrA);
		while(al!=NULL){
			char* str=writeAlarm(al);
			object=realloc(object,sizeof(char)*strlen(str)+strlen(object)+100);
			strcat(object,str);
			al=nextElement(&itrA);
			free(str);
		}
		Property *prop=nextElement(&itrP);
		while(prop!=NULL){
			char*str=writeProperty(prop);
			object=realloc(object,sizeof(char)* strlen(str)+strlen(object)+100);
			strcat(object,str);
			prop=nextElement(&itrP);
			free(str);
		}

		strcat(object,"END:VEVENT\r\n");
		return object;
	}
	return "INV_EVENT";
}
char * writeAlarm(Alarm * obj){
	if(obj!=NULL){
		char* object=malloc(sizeof(char)*(300+strlen(obj->trigger)));
		strcpy(object,"BEGIN:VALARM\r\n");
		ListIterator itr=createIterator(obj->properties);
		Property* prop=nextElement(&itr);
		strcat(object,"ACTION:");
		strcat(object,obj->action);
		strcat(object,"\r\n");
		strcat(object,"TRIGGER;");
		strcat(object,obj->trigger);
		strcat(object,"\r\n");
		while(prop!=NULL){
			char*str=writeProperty(prop);
			object=realloc(object, sizeof(char)*strlen(str)+strlen(object)+100);
			strcat(object,str);
			prop=nextElement(&itr);
			free(str);
		}
		strcat(object,"END:VALARM\r\n");
		return object;
	}
	return "INV_ALARM";
}
char * writeProperty(Property * obj){
	if(obj!=NULL){
		char* desc=obj->propDescr;
		int size=200+strlen(desc)+100;
		char* string=malloc(sizeof(char)*size);
		strcpy(string, obj->propName);
		strcat(string,":");
		strcat(string,desc);
		strcat(string,"\r\n");
		return string;
	}
	return "OTHER_ERROR";
}
