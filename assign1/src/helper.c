/**
 * helper functions for calendarparsr.h
 * jonah Stegman
 * 0969112
 */
#include "helper.h"
int dateLength(){
	DateTime*d =malloc(sizeof(DateTime));
	int length=10+sizeof(bool)+7;
	free(d);
	return length;
}

Event * createEvent(char** lines, int position, int size){
	Event * obj =malloc(sizeof(Event));
	obj->alarms=initializeList(&printAlarm,&deleteAlarm,&compareAlarms);
	obj->properties=initializeList(&printProperty,&deleteProperty,&compareProperties);
	strcpy(obj->UID,"");
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
		if(!(strstr(lines[position],"END:VEVENT")!=NULL)){
			if(strstr(lines[position],"UID:")!=NULL){
				strcpy(obj->UID,lines[position]);
			}else if(strstr(lines[position],"DTSTAMP:")!=NULL){
				obj->creationDateTime=createDate(lines[position]);
			}else if(strstr(lines[position],"DTSTART:")!=NULL){
				obj->startDateTime=createDate(lines[position]);
			}else if(strstr(lines[position],"BEGIN:VALARM")!=NULL){
				Alarm *a=createAlarm(lines,position,size);
				insertFront(obj->alarms,a);
				flag=1;
				free(a);
			}else{
				Property*p=createProperty(lines[position]);
				insertFront(obj->properties,p);
				free(p);
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
	Property* obj=malloc(sizeof(Property)+ sizeof(char)* strlen(line));
	int i=0;
	int flag=0;
	strcpy(obj->propName,"");
	strcpy(obj->propDescr,"");
	if(line!=NULL){
		for(i=0; i< strlen(line); i=i+1){
			if(flag==1){
				obj->propDescr[i]=line[i];
			}else if((line[i]!=':'|| line[i] !=';')&& flag !=1){
				obj->propName[i]=line[i];
			}else{
				flag=1;
			}
		}
	}
	return obj;
}

DateTime  createDate(char* line){
	DateTime obj;
	printf("%s",line);
	int i=0;
	int ctr=0;
	int flag=0;
	for (i=8; i<strlen(line); i=i+1){
		if(line[i]!='T'&&flag==0){
			obj.date[ctr]=line[i];
			ctr=ctr+1;
		}else if(flag==1&&line[i]!='Z'){
			obj.time[ctr]=line[i];
			ctr=ctr+1;
		}else{
			ctr=0;
			flag=flag+1;
		}
	}
	if(line[i-3]=='Z'){
		obj.UTC=true;
	}else{
		obj.UTC=false;
	}
	return obj;
}
