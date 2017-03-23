#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readfileinreverse(FILE *fp)
{
    int i, size, start, loop, counter;
    char *buffer;
    char line[256];
    start = 0;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    buffer = malloc((size+1) * sizeof(char));

    for (i=0; i< size; i++)
    {
        fseek(fp, size-1-i, SEEK_SET);
        buffer[i] = fgetc(fp);

        if(buffer[i] == 10)
        {
           if(i != 0)
           {
            counter = 0;        
            for(loop = i; loop > start; loop--)
            {
                if((counter == 0) && (buffer[loop] == 10))
                {
                    continue;
                }               
                line[counter] = buffer[loop];
                counter++;
            }
            line[counter] = 0;
            start = i;
            printf("%s\n",line);
           }
        }
    }

    if(i > start)
    {    
        counter = 0;
        for(loop = i; loop > start; loop--)
        {       
            if((counter == 0) && ((buffer[loop] == 10) || (buffer[loop] == 0)))
            {
                continue;
            }               
            line[counter] = buffer[loop];
            counter++;
        }
        line[counter] = 0;

        char buf[100];
        strcat(buf,line[counter]);

        if (strstr(buff, "2017-03-23")) {

            printf("%s\n",line);
        }
        return;
    }
}

int main(int argc, char *argv[])
{
    char *file_name;
	file_name = argv[1];

    FILE *fp = fopen(file_name,"r");
    readfileinreverse(fp);
    return 0;
}