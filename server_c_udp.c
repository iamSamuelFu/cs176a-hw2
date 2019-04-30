#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>

int digits_only(const char *s){
  while (*s) {
    if(isdigit(*s++) == 0) 
      return 0;
  }
  return 1;
}

void error(const char *msg)
{
  perror(msg);
  exit(0);
}

int addDigits(char *msg){
  int sum = 0;
  for(int i = 0; i < 256; i++){
    if(msg[i] == 10 || msg[i] == 0){ //end of string character
      break;
    }
    sum += msg[i] - '0';
  }
  return sum;
}

int main(int argc, char *argv[])
{
 int sock, length, n;
 socklen_t fromlen;
 struct sockaddr_in server;
 struct sockaddr_in from;
 char buf[1024];

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
  n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
  if (n < 0) 
    error("recvfrom");

  if(digits_only(buf)){
    int sum = addDigits(buf);
    sprintf(buf, "%d", sum);
    printf("Sending: %s\n",buf);
    n = sendto(sock, buf, strlen(buf), 0,(struct sockaddr *)&from,fromlen);
  }else{
    n = sendto(sock, "Sorry, cannot compute!", 22, 0,(struct sockaddr *)&from,fromlen);
  }
  if (n  < 0) 
    error("sendto");
}

return 0;
}
