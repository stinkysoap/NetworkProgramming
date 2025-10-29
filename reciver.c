
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
  int sockfd;
  struct addrinfo hints, *res;
  char buffer[100];
  int bytes_received;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo("127.0.0.1", "3490", &hints, &res) != 0) {
    perror("getaddrinfo");
    exit(1);
  }

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd == -1) {
    perror("socket");
    exit(1);
  }

  if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
    perror("connect");
    close(sockfd);
    exit(1);
  }

  freeaddrinfo(res);

  bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
  if (bytes_received == -1) {
    perror("recv");
    close(sockfd);
    exit(1);
  }

  buffer[bytes_received] = '\0';
  fprintf(stdout, "Received from server: %s\n", buffer);

  close(sockfd);
  return 0;
}
