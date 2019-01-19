/*
 * Helper functions for calendarparser.c
 *Jonah Stegmna
 *0969112
 */
//Gets the size of a DateTIme struct
int dateLength();

//creates a event struct and inserts data into it
Event * createEvent(char** lines,int position, int size);

//creates a alarm struct and inserts data into it
Alarm * createAlarm(char** lines,int position,int size);

//creates a property struct and inserts data into it
Property * createProperty(char* line);

//creates a dattiem struct and inserts data into it
DateTime * createDate(char* line);
