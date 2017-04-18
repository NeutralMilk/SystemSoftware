#include<stdio.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
//#include "unp.h"

#define SERV_TCP_PORT 5035

int main(int argc,char**argv)
{
	int sockfd,newsockfd,clength, listenfd, connfd, n;

	//struct sockaddr_in serv_addr,cli_addr;
    int READ_SIZE;
	char buffer[4096];
	char writebuff[4096];;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t len;
	int counter = 0;

	// if(argc != 2) //verifies number of command-line args; two in this instance
	// 	err_quit("usage: <Program Name> <Port No.>");

	listenfd = socket(AF_INET, SOCK_STREAM, 0); //create socket

    if( listenfd == -1) {
        printf("Could not create socket");
    } else {
        printf("Socket created");
    }

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family 	=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port	= htons(8081);//htons(atoi(argv[1]));

	/*
listenfd = Socket(AF_INET, SOCK_STREAM, 0); //create socket
	sockfd=socket(AF_INET,SOCK_STREAM,0);
 	serv_addr.sin_family=AF_INET;
 	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(SERV_TCP_PORT);
*/
	printf("\nStart");
	if( bind(listenfd, (struct sockaddr *) &servaddr,sizeof(servaddr)) < 0) {
        perror("Bind Issue");
        return 1;
    }

	printf("\nListening...");
    listen(listenfd, 3);

	/*
printf("\nStart");
	bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	printf("\nListening...");
	printf("\n");
	listen(sockfd,5);
*/

	//then we accept the connection from client
	//clength=sizeof(cli_addr);
	//newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clength);

	len = sizeof(cliaddr);
	connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len);

    if (connfd < 0) {
        perror("can't establish connection");
        return 1;
    } else {
	    printf("\nAccepted\n");
    }

	//printf("\r\nClient Addr %lu \r\n", &cliaddr.sin_addr);
	//printf("Client Port %hu \r\n", cliaddr.sin_port);

	//printf("connection from %s, port %d\n",
	//		Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
	//		ntohs(cliaddr.sin_port) );
	while(1) {
		counter	 = 0;
		READ_SIZE = recv(connfd,buffer,4096, 0);
		printf("\nClient message:%s",buffer);
		/*
while( (n = read(connfd, buffer, MAXLINE)) >0)
		{
			counter++;
			buffer[n] = 0; //null terminate
			//exception has occured
			if(n<0)
			{
				break;
			}
			if(strstr(buffer, "\r\n") != NULL)
			{
				break;
			}
		}
		if(n<0)
		{
			break;
*/
		//}
		printf("Client: %s",buffer);
		//write out

		printf("\nEnter the message to send\n");
		printf("\nClient: ");

		//write out
		fgets(writebuff,4096,stdin);
		write(connfd,writebuff,4096);

        if ( READ_SIZE == 0 ) {
            puts("client disconnected");
            fflush(stdout);
        } else if ( READ_SIZE == -1 ) {
            perror("read error");
        }
	}
	   //close(connfd);
	   return 0;
}