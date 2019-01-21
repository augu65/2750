#include "CalendarParser.h"
#include "helper.h"

int main(int argc, char**argv){
	DateTime d;
	strcpy(d.date,"20190117");
	strcpy(d.time,"104200");
	d.UTC=0;
	DateTime ds;
	strcpy(ds.date,"2435");
	strcpy(ds.time,"234rt");
	ds.UTC=0;
	Property*p=malloc(sizeof(Property)+sizeof(char)*100);
	strcpy(p->propName,"test");
	strcpy(p->propDescr,"this is a description");
	Alarm * a = malloc(sizeof(Alarm));
	a->trigger=malloc(sizeof(char)*100);
	a->properties=initializeList(&printProperty,&deleteProperty,&compareProperties);
	strcpy(a->action,"test action");
	strcpy(a->trigger,"trigger test");
	insertFront(a->properties,p);
	Event * e= malloc(sizeof(Event));
	strcpy(e->UID,"test UID");
	e->creationDateTime=d;
	e->startDateTime=ds;
	Property*p2=malloc(sizeof(Property)+sizeof(char)*100);
	e->properties=initializeList(&printProperty,&deleteProperty,&compareProperties);
	strcpy(p2->propName,"test2");
	strcpy(p2->propDescr,"tttt");
	insertFront(e->properties,p2);
	e->alarms=initializeList(&printAlarm,&deleteAlarm,&compareAlarms);
	insertFront(e->alarms,a);
	Calendar * obj=malloc(sizeof(Calendar));
	strcpy(obj->prodID,"1234567890");
	obj->version=2.0;
	obj->events=initializeList(&printEvent,&deleteEvent,&compareEvents);
	obj->properties=initializeList(&printProperty,&deleteProperty,&compareProperties);
	insertFront(obj->events,e);
	Property*ps=malloc(sizeof(Property)+sizeof(char)*100);
	strcpy(ps->propName,"233v");
	strcpy(ps->propDescr,"394rtjigfvbgtd");
	insertFront(obj->properties,ps);
	deleteCalendar(obj);

	Calendar * c=malloc(sizeof(Calendar));
	char* file=argv[1];
	createCalendar(file,&c);
	printCalendar(c);
	deleteCalendar(c);
	return 0;
}
