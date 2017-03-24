#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	 //find /root/html -type f -cmin -5 -printf '%TY-%Tm-%Td  %TT %u %p\n'
	char* args[] = {"find","/Users/timothybarnard/Documents/SystemSoftware", "-type", "f","-cmin", "-5", 
					"-printf", "'%TY-%Tm-%Td  %TT %u %p\n'", ">", "/root/logs/text.txt" NULL};
	execv("/bin/find",args); 
}