
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int s, new_fd;
  struct addrinfo hints, *result;
  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  char buffer[100];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // let getaddrinfo fill in IP

  // get address info
  if (getaddrinfo(NULL, "3490", &hints, &result) != 0) {
    perror("getaddrinfo");
    exit(1);
  }

  // make a socket
  s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (s == -1) {
    perror("socket");
    exit(1);
  }

  // allow reuse of port immediately after close()
  int yes = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

  // bind
  if (bind(s, result->ai_addr, result->ai_addrlen) != 0) {
    perror("bind");
    exit(1);
  }

  freeaddrinfo(result);

  // listen
  if (listen(s, 10) == -1) {
    perror("listen");
    exit(1);
  }

  printf("Server: waiting for connections...\n");

  addr_size = sizeof(their_addr);
  new_fd = accept(s, (struct sockaddr *)&their_addr, &addr_size);
  if (new_fd == -1) {
    perror("accept");
    exit(1);
  }

  char *recieved = "connections done ";
  int bytes_sent;

  // ye le the send system call
  // int send (int sockfd , const void *msg,int len , int flags )
  // The flags argument is the bitwise OR of zero or more of the following
  // flags.
  bytes_sent = send(new_fd, recieved, strlen(recieved), 0);
  close(new_fd);
}
