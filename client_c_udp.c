//Author: Zihao Zhang
//Date: 10.23.2018
//start code is from http://www.linuxhowtos.org/data/6/client_udp.c
/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int digit_test(const char *msg){
  for(int i = 0; i < 256; i++){
    if(msg[i] == 0){
      if(i < 2){
  return 0;
      }else{
  return 1;
      }
    }else if(msg[i] < '0'|| msg[i] > '9'){//not a digit
      return 0;
    }
  }
  return 1;
}

int main(int argc, char *argv[])
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256];
   
   if (argc != 3) { printf("Usage: server port\n");
                    exit(1);
   }
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   printf("Enter string: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);

   do{
     n=sendto(sock,buffer,
        strlen(buffer),0,(const struct sockaddr *)&server,length);
     if (n < 0) error("Sendto");
     bzero(buffer,256);
     n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
     if (n < 0) error("recvfrom");
     printf("From server: %s\n", buffer);
   }while(digit_test(buffer));

   close(sock);
   return 0;
}
