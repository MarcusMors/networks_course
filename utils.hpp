#ifndef __UTILS_H__
#define __UTILS_H__

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

//  std
#include <iostream>
#include <std::string>


#define USING_NETCAT

#ifdef USING_NETCAT
// #define netcat_parse(x) x.pop_back();
#define netcat_parse(x) \
  if (true)             \
    x.pop_back();       \
  else                  \
    (void)0

// #define netcat_parse_c(buff, n) buff[n - 1] = '\0' else(void) 0
#define netcat_parse_c(buff, n) \
  if (true)                     \
    buff[n - 1] = '\0';         \
  else                          \
    (void)0

#else
#define netcat_parse(x) \
  if (true) {           \
    ;                   \
  } else                \
    (void)0
#define netcat_parse_c(x) \
  if (true) {             \
  } else                  \
    (void)0
#endif


void handle_failure(int code, std::string error_msg);
void handle_failure(int code, std::string error_msg, int FD);


void handle_failure(int code, std::string error_msg)
{
  if (code == -1) {
    std::cerr << error_msg << std::endl;
    exit(EXIT_FAILURE);
  }
}

void handle_failure(int code, std::string error_msg, int FD)
{
  if (code == -1) {
    close(FD);
    std::cerr << error_msg << std::endl;
    exit(EXIT_FAILURE);
  }
}

#endif// __UTILS_H__
