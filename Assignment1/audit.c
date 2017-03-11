#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"
#include "audit.h"

void getSummaryReport()
{   
    char* args[] = {"aureport"," > summary.txt ", NULL};
    execv("/bin/audit",args); 
}