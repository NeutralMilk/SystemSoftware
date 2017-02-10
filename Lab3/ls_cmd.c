#include <sys/types.h> 
#include <sys/dir.h> 
#include <sys/param.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
// List of functions 
int file_select(); 
void find_ext(char* file_name);
extern int alphasort();

int main( int argc, char *argv[] )  {
	
	int count,i;
	struct direct **files;
	char pathname[MAXPATHLEN]; 
	int t = 1;
	getwd(pathname);
	printf("PWD = %s\n",pathname);
	
	count = scandir(pathname, &files, file_select, alphasort);
	/* No files in Dir */ 
	if (count <= 0) {
		printf("No files in Dir\n");
		exit(0); 
	}
	
	printf("Number of files = %d\n",count);
	
	for (i=1;i<count+1;++i) {
		
		find_ext(files[i-1]->d_name, argv[1] );
	}
	printf("\n"); 
}

int file_select(struct direct *entry) {
	
	if(strcmp(entry->d_name, "..") == 0 ) {
		return(0);
	} else if(strcmp(entry->d_name, ".") == 0) {
		return(0);
	} else {
		return (1); 
	}
}

void find_ext(char *file_name, char *ext) {
	char *ret;
	ret = strstr(file_name, ext);
	printf("%s", ret );
}

