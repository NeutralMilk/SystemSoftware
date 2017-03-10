//
// Created by Timothy Barnard on 10/03/2017.
//

#include "backup.h"

void backup_folder()
{
    char* args[] = {"-R", "aux", NULL};
    execv("/bin/cp",args); 
}
