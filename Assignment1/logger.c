//
// Created by Timothy Barnard on 10/03/2017.
//

#include "logger.h"
#include <syslog.h>

void logInfoMessage( char* message )
{
   openlog("dt228Yr4",LOG_PID|LOG_CONS, LOG_USER);
   //syslog(LOG_INFO, "%s", message);
   syslog (LOG_WARNING, "Log message");
   closelog();
}
