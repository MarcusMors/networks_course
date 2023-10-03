#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
// std
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
// user made
#include <print.hpp>

using namespace std;

enum Actions : char {
  login = 'N',
  error_invalid_nickname,
  message = 'M',
  logout = 'Q',
  broadcast = 'W',
  list = 'L',// list
  error = 'E',
  file = 'F',// F | sz ||| destiny/origin | sz 5B | filename |||  sz 10B | content | hash 10B | ts 14B |
  // hash: sum of all ascii in (file and ts)
  // content +|+ timestamp | hash | : to avoid data duplication.
  ok_error_ok = 'O',
  ok_error_er,// e | size  | destiny | ok/error 2B
};

map<string, int> clients;


void thread_client(int socketCli)
{
  char buff[256];
  int n;
  // string message{ "Write your nickname:\n" };
  // n = write(socketCli, message.c_str(), message.size());

  // string message = buff + 3;
  // for (auto client : clients) {
  //   int clientSocket = client.second;
  //   n = write(clientSocket, message.c_str(), message.size());
  // }

  // write();
  do {
    n = read(socketCli, buff, 255);
    char a = buff[0];
    buff[0] = '0';
    buff[n] = '\0';
    int t = atoi(buff);

    switch (a) {
    case Actions::login: {
      string my_name;
      clients[buff + 3] = socketCli;
      my_name = buff + 3;
      println("new client: ", my_name)
      // cout << "new client: " << my_name << "\n";
    } break;
    case Actions::message: break;
    case Actions::logout: {
      string name;
      for (auto item : clients) {
        if (item.second == socketCli) name = item.first;
      }
      clients.erase(name);
      // clients.erase(socketCli);
      println("El cliente ", name, " se desconecto");
      // cout << "El cliente " << name << " se desconecto"<< "\n";
    } break;
    case Actions::broadcast: {
      string message = buff + 3;
      for (auto client : clients) {
        int clientSocket = client.second;
        n = write(clientSocket, message.c_str(), message.size());
      }
    } break;
    case Actions::list: {
      vector<string> names;
      for (auto client : clients) {
        string clientName = client.first;
        names.push_back(clientName);
      }
      string message;
      for (string name : names) message += name + "\n";
      n = write(socketCli, message.c_str(), message.size());
    } break;
    case Actions::error: break;
    case Actions::ok_error_ok: break;
    case Actions::ok_error_er: break;
    }

  } while (true);

  shutdown(socketCli, SHUT_RDWR);

  close(socketCli);
}


int main(void)
{
  int socket_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  handle_failure(socket_server, "[-]Socket error");

  sockaddr_in stSockAddr{ .sin_family = AF_INET, .sin_port = htons(1100), .sin_addr{ .s_addr = INADDR_ANY } };


  const int c = bind(socket_server, (const sockaddr *)&stSockAddr, sizeof(sockaddr_in));
  handle_failure(c, "[-]Error, Unable to bind");

  const int l = listen(socket_server, 10);
  handle_failure(l, "[-]Error, listen failed", socket_server);

  while (true) {
    int socketClient = accept(socket_server, NULL, NULL);
    // clients.push_back(socketClient);
    thread(thread_client, socketClient).detach();
  }


  close(socket_server);
  return 0;
}
