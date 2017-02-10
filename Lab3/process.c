#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <string.h>

int main( int argc, char *argv[] )  {
    
    int link[2];
    pid_t pid;
    char foo[4096];

    if (pipe(link)==-1) {
        exit(27);
    }

    if ((pid = fork()) == -1) {
        exit(27);
    }

    if(pid == 0) {

        dup2 (link[1], STDOUT_FILENO);
        close(link[0]);
        close(link[1]);
        execl("/bin/ps", "ps", "aux", (char *)0);
        exit(27);

    } else {

        close(link[1]);
        
        int nbytes = read(link[0], foo, sizeof(foo));
        
        if ( argc == 2 ) {
            printf("Finding... %s\n",argv[1]);
            
            execl("/bin/grep", "grep", argv[1], (char *)0);
            exit(27);
            
            
        } else {

            printf("Output: (%.*s)\n", nbytes, foo);
            wait(NULL);
        }

    }
    return 0;
}
