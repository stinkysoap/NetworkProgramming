
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BACKLOG 50

void *handle_client(void *arg) {
  int client_fd = *(int *)arg;
  free(arg);

  const char *message = "connections done";
  send(client_fd, message, strlen(message), 0);

  close(client_fd);
  pthread_exit(NULL);
}
int main() {
  int s;
  struct addrinfo hints, *result;
  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  pthread_t tid;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(NULL, "3490", &hints, &result) != 0) {
    perror("getaddrinfo");
    exit(1);
  }

  s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (s == -1) {
    perror("socket");
    exit(1);
  }

  int yes = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

  if (bind(s, result->ai_addr, result->ai_addrlen) != 0) {
    perror("bind");
    exit(1);
  }

  freeaddrinfo(result);

  if (listen(s, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  printf("Server: waiting for connections...\n");

  while (1) {
    addr_size = sizeof(their_addr);
    int *new_fd = malloc(sizeof(int));
    if (!new_fd) {
      perror("malloc");
      continue;
    }

    *new_fd = accept(s, (struct sockaddr *)&their_addr, &addr_size);
    if (*new_fd == -1) {
      perror("accept");
      free(new_fd);
      continue;
    }

    if (pthread_create(&tid, NULL, handle_client, new_fd) != 0) {
      perror("pthread_create");
      close(*new_fd);
      free(new_fd);
    } else {
      pthread_detach(tid); // no need to join
    }
  }

  close(s);
  return 0;
}
