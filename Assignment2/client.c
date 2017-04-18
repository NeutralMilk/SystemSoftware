#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<strings.h>
//#include "unp.h" 
#define SERV_TCP_PORT 5035

int main(int argc,char*argv[])
{
	int sockfd;
	struct sockaddr_in serv_addr;
	char buffer[4096];
	char readBuff[4096];
	struct sockaddr_in servaddr;

	// if(argc != 3)
	// 	printf("usage:a.out <IPaddress>");

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0)
		printf("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8888);//htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	printf("\nReady for sending...");
	if(connect(sockfd, (struct sockaddr * ) &servaddr, sizeof(servaddr)) <0)
		printf("connect error");

	while(1) {

		printf("\nEnter the message to send\n");
		printf("\nClient: ");

		//write out
		fgets(buffer,4096,stdin);
		write(sockfd,buffer,4096);

		//read in
		printf("\nWaiting for reponse from server\n");
		read(sockfd,readBuff,4096);
		printf("\nServer message:%s",readBuff);

	}

	close(sockfd);
	return 0;
}