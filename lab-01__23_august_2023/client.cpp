/* Client code in C */

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


enum protocols {
  TCP = SOCK_STREAM,
  UDP,
};

void handle_failure(int code, string error_msg)
{
  if (code == -1) {
    cerr << error_msg << endl;
    exit(EXIT_FAILURE);
  }
}

void handle_failure_2(string error_msg)
{
  cerr << error_msg << endl;
  exit(EXIT_FAILURE);
}

void handle_failure_3(int code, string error_msg, int FD)
{
  if (code == -1) {
    close(FD);
    cerr << error_msg << endl;
    exit(EXIT_FAILURE);
  }
}

int main()
{
  int socket_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  handle_failure(socket_server, "[-]Socket error");
  printf("[+]TCP client socket created.\n");

  sockaddr_in stSockAddr;
  memset(&stSockAddr, 0, sizeof(sockaddr_in));

  int port = htons(1100);
  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = port;
  // stSockAddr.sin_addr.s_addr = inet_addr(ip);


  int Res = inet_pton(AF_INET, "192.168.1.33", &stSockAddr.sin_addr);

  if (0 > Res) {
    close(socket_server);
    handle_failure_2("[-]first parameter is not a valid address family");
  } else {
    handle_failure_3(Res - 1, "[-]char string (second parameter does not contain valid ipaddress", socket_server);
  }

  handle_failure_3(connect(socket_server, (const sockaddr *)&stSockAddr, sizeof(sockaddr_in)), "[-]connect failed\n");

  printf("[+]Bind to the port number: %d.\n", port);

  int n = write(socket_server, "Hi, this is Julio.", 18);
  printf("%d\n", n);
  /* perform read write operations ... */

  shutdown(socket_server, SHUT_RDWR);

  close(socket_server);
  return 0;
}
