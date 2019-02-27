#include "CalendarParser.h"
#include "helper.h"

int main(int argc, char**argv){
	/*
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
	*/
	Calendar * c=malloc(sizeof(Calendar)*1);
	char* file=malloc(sizeof(char)*(strlen(argv[1])+10));
	strcpy(file,argv[1]);
	ICalErrorCode ee=createCalendar(file,&c);
	char*strs=printError(ee);
	printf("%s\n",strs);
//	printf("%s",printCalendar(c));
	char * test=malloc(sizeof(char)*100);
	strcpy(test,"test.ics");
	writeCalendar(test,c);
	free(file);
	//free(test);
	Property*pr=malloc(sizeof(Property)*sizeof(char)+100);
	strcpy(pr->propName,"GEO");
	strcpy(pr->propDescr,"this is a test");
	ListIterator  it=createIterator(c->events);
	Event * ev=nextElement(&it);
	insertBack(ev->properties,pr);
	ee=validateCalendar(c);
	char*str=printError(ee);
	printf("%s\n",str);
	char *s= calendarToJSON(c);
	char *e= eventListToJSON(c->events);
	printf("%s\n",s);
	printf("%s\n",e);
	deleteCalendar(c);
//	deleteCalendar(c);
//	free(c);
	return 0;
}
