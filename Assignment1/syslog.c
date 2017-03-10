#include <syslog.h>
#include <string.h>
 
void writeMessageToSyslogd(const char* message);
 
int main(int argc, char *argv[])
{
	writeMessageToSyslogd("Blair is writing to syslog");
	return 0;
}
 
void writeMessageToSyslogd(const char* message){
	openlog("Demo",LOG_CONS,LOG_USER);
	syslog(LOG_ERR, "%s", message);
	closelog();
}