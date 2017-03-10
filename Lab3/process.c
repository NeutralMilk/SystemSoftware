#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <string.h>

int main( int argc, char *argv[] )  {
    

    if (argc == 1) {
        char* args[] = {"ps", "aux", NULL};
        execv("/bin/ps",args); 

    } else if (argc == 2 )  {
           
        printf("Finding... %s\n",argv[1]);
        char* args[] = {"pgrep", argv[1], NULL};    
        execv("/usr/bin/pgrep", args);
           
    } else if (argc == 3) {
        
        printf("Killing... %s\n",argv[2]);
        char* args[] = {"kill", argv[2], NULL};
        execv("/bin/kill", args);
    }
    
    return 0;
}
