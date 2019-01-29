/*
 * Helper functions for calendarparser.c
 *Jonah Stegmna
 *0969112
 */
#ifndef HELPER_H
#define HELPER_H
#include <ctype.h>
#include "CalendarParser.h"

//creates a event struct and inserts data into it
Event * createEvent(char** lines,int position, int size);

//creates a alarm struct and inserts data into it
Alarm * createAlarm(char** lines,int position,int size);

//creates a property struct and inserts data into it
Property * createProperty(char* line);

//creates a dattiem struct and inserts data into it
DateTime * createDate(char* line);

#endif
