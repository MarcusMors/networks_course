#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// std
#include <array>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

// user made header
#include <utils.hpp>
// #include "../ip/ip_types.hpp"


using namespace std;

vector<string> client_names;
vector<int> client_sockets;

void thread_client(int socket_client)
{
  bool new_user = true;
  char buff[100];
  bool nickname = false;

  do {
    int n = read(socket_client, buff, 255);

    if (n == 1 or n == 0) { continue; }// do nothing or error, empty content

    buff[n] = '\0';
    netcat_parse_c(buff, n);
    string msg{ buff };
    printf("Content: [%s]\n", msg.c_str());

    if (new_user) {
      client_sockets.push_back(socket_client);
      client_names.push_back(msg);
      new_user = false;
    } else {
      string destination_name = msg.substr(0, msg.find(','));
      string content = msg.substr(msg.find(',') + 1, msg.size());
      string origin{};

      bool destination_not_found = true;
      int destination_socket;

      for (int i = 0; i < client_names.size(); i++) {
        if (client_names[i] == destination_name) {
          destination_socket = client_sockets[i];
          destination_not_found = false;
        }
        if (socket_client == client_sockets[i]) { origin = client_names[i]; }
      }
      if (destination_not_found) {
        char missed_msg[] = "Destination not found\n";
        n = write(socket_client, missed_msg, sizeof(missed_msg));
      } else {
        string final_msg = { origin + ": " + content + "\n" };
        n = write(destination_socket, final_msg.c_str(), (final_msg.length() + 1) * sizeof(char));
      }
    }

  } while (true);

  shutdown(socket_client, SHUT_RDWR);
  close(socket_client);
}

int main(void)
{
  int socket_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  handle_failure(socket_server, "[-]Socket error");
  sockaddr_in stSockAddr;
  memset(&stSockAddr, 0, sizeof(sockaddr_in));

  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(1101);
  stSockAddr.sin_addr.s_addr = INADDR_ANY;

  const int c = bind(socket_server, (const sockaddr *)&stSockAddr, sizeof(sockaddr_in));
  handle_failure(c, "[-]Error, Unable to bind");

  const int l = listen(socket_server, 10);
  handle_failure(l, "[-]Error, listen failed", socket_server);

  while (true) {
    int socket_client = accept(socket_server, NULL, NULL);
    thread(thread_client, socket_client).detach();
  }

  close(socket_server);
  return 0;
}
