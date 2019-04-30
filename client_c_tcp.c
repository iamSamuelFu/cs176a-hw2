//Author: Zihao Zhang
//Date: 10.23.2018
// start code is from http://www.linuxhowtos.org/data/6/client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h> 

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
    }else if(msg[i] < '0'|| msg[i] > '9'){
      return 0;
    }
  }
  
  return 1;
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    int one = 1;
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Enter string: ");
    bzero(buffer,256);
    fgets(buffer,256,stdin);

    char *error_message = "Sorry, cannot compute!";

    do{
      // Write message
      n = write(sockfd,buffer,strlen(buffer));
      if (n < 0) 
  error("ERROR writing to socket");
      // Clear buffer
      bzero(buffer,256);
      // Read message
      n = read(sockfd,buffer,255);
      if (n < 0) 
  error("ERROR reading from socket");
      //Print response
      printf("From server: %s\n",buffer);
    }while(digit_test(buffer));

    close(sockfd);
    return 0;
}
