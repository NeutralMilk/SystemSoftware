
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"
#include "backup.h"
#include "file_modified.h"

void push_changes(char * directory) {

    //find /root/html -type f -cmin -5 -printf '%TY-%Tm-%Td  %TT %u %p\n'
    char* args[] = {"find",directory, "-type", "f","-cmin", "-5", 
                "-printf", "'%TY-%Tm-%Td  %TT %u %p\n'", ">", "/root/logs/text.txt", NULL};
    execv("/bin/find",args); 
    logInfoMessage("push to live completed");
}