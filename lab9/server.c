#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                                          \
  do {                                                                                             \
    perror(msg);                                                                                   \
    exit(EXIT_FAILURE);                                                                            \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  int cfd = client->cfd;
  int client_id = client->client_id;
  free(client);

  // TODO: print the message received from client

  printf("Client created. ID: %d Socket: %d\n", client_id, cfd);

  // TODO: increase total_message_count per message

  char buf[BUF_SIZE];
  ssize_t num_read;

  while ((num_read = read(cfd, buf, BUF_SIZE - 1)) > 0) {
    buf[num_read] = '\0';

    if (buf[num_read - 1] == '\n') {
      buf[num_read - 1] = '\0';
    }

    pthread_mutex_lock(&count_mutex);
    total_message_count++;
    printf("Msg # %3d; Client ID %d: %s\n", total_message_count, client_id, buf);
    pthread_mutex_unlock(&count_mutex);
  }

  if (num_read == -1) {
    perror("read");

    if (close(cfd) == -1) {
      perror("close");
    }

    printf("Thread ending for client %d\n", client_id);

    return NULL;
  }
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  for (;;) {
    // TODO: create a new thread when a new connection is encountered
    int cfd = accept(sfd, NULL, NULL);

    if (cfd == -1) {
      handle_error("accept");
    }

    // TODO: call handle_client() when launching a new thread, and provide
    struct client_info *client = malloc(sizeof(struct client_info));
    if (client == NULL) {
      handle_error("malloc");
    }

    client->cfd = cfd;
    pthread_mutex_lock(&client_id_mutex);
    client->client_id = client_id_counter;
    client_id_counter++;
    pthread_mutex_unlock(&client_id_mutex);

    pthread_t tid;
    int err = pthread_create(&tid, NULL, handle_client, client);
    if (err != 0) {
      errno = err;
      perror("pthread_create");
      free(client);
      close(cfd);
      continue;
    }

    pthread_detach(tid);
  }

  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
