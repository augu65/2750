#include "CalendarParser.h"

int main(){
	DateTime*d=malloc(sizeof(DateTime));
	strcpy(d->date,"2019/01/17");
	strcpy(d->time,"10:42");
	d->UTC=0;
	printf("%s\n",printDate(d));

	return 0;
}
