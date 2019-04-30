//Author: Zihao Zhang
//Date: 10.23.2018
//start code is from http://www.linuxhowtos.org/data/6/server_udp.c
/* Creates a datagram server.  The port 
   number is passed as an argument.  This
   server runs forever */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];
   int sum = 0;
   char sumstr[1024];

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   while (1) {
   		sum = 0;
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       //printf("n = %d\n", n);
       for( int i = 0; i < n - 1; i++){
			if(buf[i] < '0' || buf[i] > '9'){
				//printf("ERROR OCCUR\n");
				sendto(sock, "Sorry, cannot compute!" , strlen("Sorry, cannot compute!"), 0, (struct sockaddr *)&from,fromlen);
				sum = -1;
				break;
			}
			sum += (buf[i] - '0');
			//printf("sum = %d\n", sum);
		}
		if( sum != -1){
			//printf("%d\n", sum);
			sprintf(sumstr, "%d", sum);
			//printf("%s\n", sumstr);
	        n = sendto(sock, sumstr, strlen(sumstr), 0,(struct sockaddr *)&from,fromlen);
	        //printf("%s\n", sumstr);
	    }

        while( strlen(sumstr) != 1){
    		int newsum = 0;
 			for(int i =0; i < n; i++){
 				newsum += (sumstr[i] - '0');
			}	
			bzero(sumstr,1024);
 			sprintf(sumstr,"%d",newsum);
 		
			n = sendto(sock, sumstr, strlen(sumstr), 0,(struct sockaddr *)&from,fromlen);
		
		}
   }
   return 0;
 }
