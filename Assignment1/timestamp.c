#include "timestamp.h"     
#include <time.h>
#include <stdio.h>

char* string_date(char * buffer_time)
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer_time,80,"%d_%m_%Y",timeinfo);

    return buffer_time;
}

int getSeconds( char* time_str ) {

    time_t now = time(NULL);
	struct tm newyear;
	newyear = *localtime(&now);
    //newyear.tm_hour = 16;
	//newyear.tm_min = 25;
	//newyear.tm_sec = 0;
    if (strptime(time_str, "%H:%M:%S", &newyear) != NULL)

    printf("hour: %d; minute: %d; second: %d\n",  newyear.tm_hour, newyear.tm_min, newyear.tm_sec);

    int time_diff = difftime(mktime(&newyear), now);

    return time_diff;
}