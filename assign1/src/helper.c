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
	int aflag=0;
	strcpy(obj->UID,"");
	for(position=position+1; position<size; position=position+1){
		if(flag==1){
			aflag=0;
			int x=position;
			for(x=position; x<size; x=x+1){
				if(strstr(lines[x],"END:VALARM")!=NULL){
					position=x+1;
					aflag=1;
				}
			}
			if(aflag!=1){
				strcpy(obj->UID,"ErrorA");
				return obj;
			}
			flag=0;
		}
		if(strstr(lines[position],"END:VEVENT")==NULL){
			if(strstr(lines[position],"UID:")!=NULL){
				strcpy(obj->UID,lines[position]);
				if(strcmp(obj->UID,"UID:")==0){
					strcpy(obj->UID,"ERRORE");
					return obj;
				}
			}else if(strstr(lines[position],"DTSTAMP:")!=NULL){
			/*	DateTime *d=createDate(lines[position]);
				obj->creationDateTime=(*d);
				free(d);*/
				DateTime d;
				int i=0; 
				int ctr=0;
				int flag=0;
				strcpy(d.date,"");
				strcpy(d.time,"");
				for(i=8;i<strlen(lines[position]); i=i+1){
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
					d.UTC=false;
				}
				obj->creationDateTime=d;
				if(strcmp(d.date,"")==0||strcmp(d.time,"")==0){
					strcpy(obj->UID,"ErrorD");
					return obj;
				}
			}else if(strstr(lines[position],"DTSTART:")!=NULL){
			/*	DateTime* d=createDate(lines[position]);
				obj->startDateTime=(*d);
				free(d);*/
				DateTime d;
				int i=0;
				int ctr=0;
				int flag=0;
				strcpy(d.date,"");
				strcpy(d.time,"");
				for(i=8; i<strlen(lines[position]);i=i+1){
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
					d.UTC=false;
				}
				obj->startDateTime=d;
				if(strcmp(d.date,"")==0||strcmp(d.time,"")==0){
					strcpy(obj->UID,"ErrorD");
					return obj;
				}
			}else if(strstr(lines[position],"BEGIN:VALARM")!=NULL){
				Alarm *a=createAlarm(lines,position,size);
				insertFront(obj->alarms,a);
				if(strcmp(a->action,"ErrorV")==0||strcmp(a->action,"ErrorP")==0){
					strcpy(obj->UID,a->action);
					return obj;
				}
				flag=1;
			}else{
				if(strstr(lines[position],"END:")==NULL){
				if(lines[position]!=NULL&&strlen(lines[position])>3&&(strstr(lines[position],";")!=NULL ||strstr(lines[position],":")!=NULL)){
					Property*p=createProperty(lines[position]);
					insertFront(obj->properties,p);
					if(strcmp(p->propName,"ErrorP")==0){
						strcpy(obj->UID,p->propName);
						return obj;
					}
				}
				}
			}
		}else{
			position=size+1;
		}
			
	}
	if(strcmp(obj->UID,"")==0){
		strcpy(obj->UID,"ERRORE");
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
			aflag=1;
			for (x=0; x<strlen(lines[position]);x=x+1){
				if(flag==1){
					obj->action[x]=lines[position][x];
				}else if (lines[position][x]==';'||lines[position][x]==':'){
					flag=1;
				}
			}
			if(flag!=1){
				strcpy(obj->action,"ErrorV");
			}
			obj->action[x]='\0';
			flag=0;
		}else if(strstr(lines[position],"TRIGGER")!=NULL){
			int x=0;
			tflag=1;
			for(x=0; x<strlen(lines[position]);x=x+1){
				if(flag==1){
					obj->trigger=realloc(obj->trigger,sizeof(char)*strlen(lines[position])+10);
					obj->trigger[x]=lines[position][x];
				}else if(lines[position][x]==';'||lines[position][x]==':'){
					flag=1;
				}
			}
			if(flag!=1){
				strcpy(obj->action,"ErrorV");
			}
			obj->trigger[x]='\0';
			flag=0;
		}else{
			if(strstr(lines[position],"END:")==NULL){
				Property*p=createProperty(lines[position]);
				if(strcmp(p->propName,"ErrorP")==0){
					strcpy(obj->action,"ErrorP");
				}
				insertFront(obj->properties,p);
			}
		}
		if(strstr(lines[position],"END:VALARM")!=NULL){
			eflag=1;
			break;
		}
	}
	if((aflag==0||tflag==0||eflag==0)&&strcmp(obj->action,"ErrorP")!=0){
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
		if(flag==0){
			strcpy(obj->propName,"ErrorP");
		}
		obj->propDescr[ctr]='\0';
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
