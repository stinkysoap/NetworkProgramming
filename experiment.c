#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
int main(int argc, char *argv[]) {
  int s;
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // fill in ip for me
  getaddrinfo(NULL, "3490", &hints, &result);

  // make a socket
  s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (bind(s, result->ai_addr, result->ai_addrlen) != 0) {
    fprintf(stderr, "Couldnt bind");
  }
}
