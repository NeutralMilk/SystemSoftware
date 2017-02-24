#include <stdio.h>

int main(int argc, char *argv[]) {
	
	FILE *fp;
	FILE *fp2;
	int status;
	char path[1024];
	char path2[1024];
	
	fp = popen("ls -al ", "r");
	
	while (fgets(path, 1024, fp) != NULL) {
		printf("%s", path);
	}
	
	fp2 = popen("awk '{print $1}'", "r");
	
	while (fgets(path2, 1024, fp2) != NULL) {
		printf("%s", path2);
	}
	
	status = pclose(fp);
	status = pclose(fp2);
	
	exit(0);
}