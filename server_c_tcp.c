// Cited code from http://www.linuxhowtos.org/data/6/server.c
// It seems the code may have been written by Sasha Nitsch, although it is unclear
// Cited code is marked below, mostly to set up the basic parts of the tcp server

/* START CITED CODE */

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/* END CITED CODE */

int isNum(const char *msg){
  for(int i = 0; i < 256; i++){
    if(msg[i] == 10 || msg[i] == 0){//end of string characters
      if(i == 0){//empty string
	return 0;
      }else{//end of string
	return 1;
      }
    }else if(msg[i] < '0' || msg[i] > '9'){//not a digit
      return 0;
    }
  }
  //should not happen but just in case
  return 1;
}

int addDigits(char *msg){
  int sum = 0;
  //add digits
  for(int i = 0; i < 256; i++){
    if(msg[i] == 10 || msg[i] == 0){ //end of string character
      break;
    }
    sum += msg[i] - '0';
  }
      
  return sum;
}

/* START CITED CODE */

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
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
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     /*END CITED CODE */
     int done = 0;
     do{
       bzero(buffer,256);
       n = read(newsockfd,buffer,255);
       if (n < 0) error("ERROR reading from socket");
       if(isNum(buffer)){
	 int sum = addDigits(buffer);
	 sprintf(buffer, "%d",sum);
	 n = write(newsockfd, buffer, strlen(buffer));
	 if(sum < 10){
	   done = 1;
	 }
       }else{
	 n = write(newsockfd, "Sorry, cannot compute!",22);
	 done = 1;
       }
       if (n < 0) error("ERROR writing to socket");
     }while(!done);
     close(newsockfd);
     close(sockfd);
     return 0; 
}
