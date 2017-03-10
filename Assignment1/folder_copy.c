#include <stdio.h>     
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main ()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80,"%d_%m_%Y/",timeinfo);
    puts (buffer);

    char source_file[255] =  "/Users/timothybarnard/Documents/SystemSoftware/Assignment1/html";
    char target_file[255] =  "/Users/timothybarnard/Documents/SystemSoftware/Assignment1/folder/";
    strcat(target_file,buffer);

    char* args[] = {"cp","-R", source_file, target_file, NULL};
    execv("/bin/cp",args); 

    return 0;
}