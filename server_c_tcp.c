//Author: Zihao Zhang
//Date: 10.23.2018
//http://www.linuxhowtos.org/data/6/server.c
/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     int sum = 0;
     char sumstr[256];

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

    while (1) {
    	listen(sockfd,5);
     	clilen = sizeof(cli_addr);
     	newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     	if (newsockfd < 0) 
          error("ERROR on accept");
        int one = 1;
      	setsockopt(newsockfd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
     	bzero(buffer,256);

   		sum = 0;
        n = recv(newsockfd,buffer,255,0);
     	if (n < 0) error("ERROR reading from socket");
       //printf("n = %d\n", n);
       for( int i = 0; i < n - 1; i++){
			if(buffer[i] < '0' || buffer[i] > '9'){
				send(newsockfd,"From server: Sorry, cannot compute!" , strlen("From server: Sorry, cannot compute!"),0);
				sum = -1;
				break;
			}
			sum += (buffer[i] - '0');
			//printf("sum = %d\n", sum);
		}
		
		if( sum != -1){
			//printf("%d\n", sum);
			sprintf(sumstr, "%d", sum);
			//printf("%s\n", sumstr);
	        send(newsockfd, sumstr, 256,0);
	        //printf("%s\n", sumstr);
	    }

        while( strlen(sumstr) != 1){
    		int newsum = 0;
 			for(int i =0; i < strlen(sumstr); i++){
 				newsum += (sumstr[i] - '0');
			}	
			bzero(sumstr,256);
 			sprintf(sumstr,"%d",newsum);
 			//printf("sumstr = %s", sumstr);
			send(newsockfd, sumstr, 256,0);
			//printf("sumstr = %s", sumstr);
		
		}
		 close(newsockfd);
     }

     close(sockfd);
     return 0; 
}