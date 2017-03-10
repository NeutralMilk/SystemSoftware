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

    char source_file[255] =  "/Users/timothybarnard/Documents/SystemSoftware/Assignment1/tim.txt";
    char target_file[255] =  "/Users/timothybarnard/Documents/SystemSoftware/Assignment1/folder/tim.txt";

    char* args[] = {"cp -R", source_file, target_file, NULL};
    execv("/bin/cp",args); 

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80,"%d_%m_%Y",timeinfo);
    puts (buffer);

    return 0;
}