#include <stdio.h>
#include <string.h>


void read_str_from_line(char* config_line, char* val) {    
	char prm_name[20];
	char prm_name1[20];
	char prm_name2[20];
	char prm_name3[20];
	sscanf(config_line, " %s %s %s %s %s\n", prm_name, prm_name1, prm_name2, val);
}

 
int main(int argc, char *argv[])
{
	char **to_be_returned[100][50];
	  
	FILE *fp;
	char buf[100];
	char log_file_name[50] = "/root/logs/files.log";

	if ((fp=fopen(log_file_name, "r")) == NULL) {
		logErrorMessages("Failed to open config file", log_file_name);
		//exit(EXIT_FAILURE);
	}
	while(! feof(fp)) {
		fgets(buf, 100, fp);
		
		if (strstr(buf, "23-03-2017")) {
			char tester[50];
			read_str_from_line(buf, tester);

			printf("%s\n",tester);
		}
	}

	fclose(fp);

//	int i = 0;
//	while(*strings) {
//		to_be_returned[i] = malloc( sizeof(char) * strlen( *strings ) );
//		strcpy( to_be_returned[i++], *strings);
//		strings++;
//	}
	
	return 0;
}