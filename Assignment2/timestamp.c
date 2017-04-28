#include "timestamp.h"     
#include <time.h>
#include <stdio.h>

char* string_date_time(char * buffer_time)
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime(buffer_time, 30, "[%Y-%m-%d %H:%M:%S]", timeinfo);

    return buffer_time;
}