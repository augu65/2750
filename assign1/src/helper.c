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
	for(position=position+1; position<size; position=position+1){
		if(flag==1){
			int x=position;
			for(x=position+1; position<size; x=x+1){
				if(strstr(lines[position],"END:VALARM")!=NULL){
					position=x+1;
				}
			}
			flag=0;
		}
		if(strstr(lines[position],"END:VEVENT")==NULL){
			if(strstr(lines[position],"UID:")!=NULL){
				strcpy(obj->UID,lines[position]);
			}else if(strstr(lines[position],"DTSTAMP:")!=NULL){
			/*	DateTime *d=createDate(lines[position]);
				obj->creationDateTime=(*d);
				free(d);*/
				DateTime d;
				int i=0; 
				int ctr=0;
				int flag=0;
				for(i=8;i<strlen(lines[position]); i=i+1){
					if(lines[position][i]!='T'&&flag==0){
						d.date[ctr]=lines[position][i];
						ctr=ctr+1;
					}else if(flag==1&&lines[position][i]!='Z'){
						d.time[ctr]=lines[position][i];
						ctr=ctr+1;
					}else{
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
			}else if(strstr(lines[position],"DTSTART:")!=NULL){
			/*	DateTime* d=createDate(lines[position]);
				obj->startDateTime=(*d);
				free(d);*/
				DateTime d;
				int i=0;
				int ctr=0;
				int flag=0;
				for(i=8; i<strlen(lines[position]);i=i+1){
					if(lines[position][i]!='T' &&flag==0){
						d.date[ctr]=lines[position][i];
						ctr=ctr+1;
					}else if(flag==1 &&lines[position][i]!='Z'){
						d.time[ctr]=lines[position][i];
						ctr=ctr+1;
					}else{
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
			}else if(strstr(lines[position],"BEGIN:VALARM")!=NULL){
				Alarm *a=createAlarm(lines,position,size);
				insertFront(obj->alarms,a);
				flag=1;
			}else{
				if(lines[position]!=NULL&&strlen(lines[position])>3&&(strstr(lines[position],";")!=NULL ||strstr(lines[position],":")!=NULL)){
					Property*p=createProperty(lines[position]);
					insertFront(obj->properties,p);
				}
			}
		}else{
			break;
		}
			
	}
	return obj;
}


Alarm * createAlarm(char** lines,int position,int size){
	Alarm * obj=malloc(sizeof(Alarm));
	obj->properties=initializeList(&printProperty,&deleteProperty,&compareProperties);
	strcpy(obj->trigger,"");
	strcpy(obj->action,"");
	int flag=0;
	for (position=position+1; position<size; position=position+1){
		if(strstr(lines[position],"BEGIN:ALARM")!=NULL){
			int x =0;
			for (x=0; x<strlen(lines[position]-3);x=x+1){
				if(flag==1){
					obj->action[x]=lines[position][x];
				}else if (lines[position][x]==';'||lines[position][x]==':'){
					flag=1;
				}
			}
			flag=0;
		}else if(strstr(lines[position],"TRIGGER")!=NULL){
			int x=0;
			for(x=0; x<strlen(lines[position])-3;x=x+1){
				if(flag==1){
					obj->trigger[x]=lines[position][x];
				}else if(lines[position][x]==';'||lines[position][x]==':'){
					flag=1;
				}
			}
			flag=0;
		}else{
			insertFront(obj->properties,createProperty(lines[position]));
		}
	}
	return obj;
}

Property * createProperty(char* line){
	Property* obj=malloc(sizeof(Property)*(sizeof(char)* strlen(line)+1));
	int i=0;
	int flag=0;
	int ctr=0;
	if(line!=NULL){
		for(i=0; i< strlen(line); i=i+1){
			if(flag==1){
				obj->propDescr[ctr]=line[i];
				ctr=ctr+1;
			}else if((line[i]!=':'|| line[i] !=';')&& flag !=1){
				obj->propName[ctr]=line[i];
				ctr=ctr+1;
				
			}else{
				ctr=0;
				flag=1;
			}
		}
	}
	return obj;
}

DateTime  *createDate(char* line){
	DateTime* obj=malloc(sizeof(DateTime)+1);
	int i=0;
	int ctr=0;
	int flag=0;
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
