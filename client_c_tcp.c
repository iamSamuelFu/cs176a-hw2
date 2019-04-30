// Cited code from http://www.linuxhowtos.org/data/6/client.c
// It seems the code may have been written by Sasha Nitsch, although it is unclear
// Cited code is marked below, mostly to set up the basic parts of the tcp client

/* START CITED CODE */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/* END CITED CODE */

int isDoubleDigitNum(const char *msg){
  for(int i = 0; i < 256; i++){
    if(msg[i] == 0){
      if(i < 2){//too short
	return 0;
      }else{//end of string
	return 1;
      }
    }else if(msg[i] < '0'|| msg[i] > '9'){//not a digit
      return 0;
    }
  }
  //backup
  
  return 1;
}

/* START CITED CODE */

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
    fgets(buffer,255,stdin);
    /* END CITED CODE */
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
    }while(isDoubleDigitNum(buffer));
    close(sockfd);
    return 0;
}
