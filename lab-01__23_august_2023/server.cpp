#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum protocols{
  TCP = SOCK_STREAM,
  UDP,
};

int main()
{
  sockaddr_in stSockAddr;

  int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(-1 == SocketFD)
  {
    perror("[-]Socket error\n");
    exit(EXIT_FAILURE);
  }
  printf("[+]TCP server socket created.\n");

  memset(&stSockAddr, 0, sizeof(sockaddr_in));

  int port = htons(1100);
  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = port;
  stSockAddr.sin_addr.s_addr = INADDR_ANY;

  if(-1 == bind(SocketFD,(const sockaddr *)&stSockAddr, sizeof(sockaddr_in)))
  {
    perror("[-]Error bind failed\n");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }

  if(-1 == listen(SocketFD, 10))
  {
    perror("[-]error listen failed\n");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }

  char buffer[256];
  for(;;) {
    int ConnectFD = accept(SocketFD, NULL, NULL);

    if(0 > ConnectFD)
    {
      perror("[-]Error accept failed\n");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }


    bzero(buffer,256);
    int n;
    n = read(ConnectFD,buffer,255);
    if (n < 0) {perror("[-]ERROR reading from socket");}
    printf("Here is the message: [%s]\n",buffer);
    n = write(ConnectFD,"I got your message",18);
    if (n < 0) {perror("[-]ERROR writing to socket");}

    /* perform read write operations ... */

    shutdown(ConnectFD, SHUT_RDWR);

    close(ConnectFD);
  }

  close(SocketFD);
  return 0;
}
