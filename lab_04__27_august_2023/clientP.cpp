#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// std
#include <iostream>
#include <string>
#include <thread>
// user made
#include <print.hpp>
#include <utils.hpp>

using namespace std;

// Protocol
// N05Pablo


void thread_read(int socketCli)
{
  char buffer[256];
  int n;
  while (true) {
    n = read(socketCli, buffer, sizeof(buffer));
    buffer[n] = '\0';
    cout << buffer;
  }
  close(socketCli);
  exit(EXIT_FAILURE);
}

int main()
{
  int socketClient = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  handle_failure(socketClient, "[-]Socket error");

  sockaddr_in stSockAddr{ .sin_family = AF_INET, .sin_port = htons(1100) };

  int Res = inet_pton(AF_INET, "127.0.0.1", &(stSockAddr.sin_addr));// Cambiar la dirección IP según la del servidor.

  if (0 > Res) {
    perror("[-] first parameter is not a valid address family");
    close(socketClient);
    exit(EXIT_FAILURE);
  }
  if (0 == Res) {
    perror("[-] char string (second parameter does not contain valid ipaddress");
    close(socketClient);
    exit(EXIT_FAILURE);
  }

  int c = connect(socketClient, (const sockaddr *)&stSockAddr, sizeof(sockaddr_in));
  handle_failure(c, "[-] connect failed", socketClient);

  println(Style::reset, Style::reset);
  println(Style::reset, Style::reset);

  char message[256];
  fgets(message, sizeof(message), stdin);
  int n = write(socketClient, message, strlen(message));

  thread(thread_read, socketClient).detach();
  while (true) {
    fgets(message, sizeof(message), stdin);
    int n = write(socketClient, message, strlen(message));
  }

  shutdown(socketClient, SHUT_RDWR);

  close(socketClient);
  return 0;
}
