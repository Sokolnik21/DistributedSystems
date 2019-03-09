#ifndef CLIENT_H
#define CLIENT_H

enum protocol { TCP, UDP };

typedef struct client_t client_t;
struct client_t {
  char * client_name;
  char * client_ip;
  char * client_port;
  char * next_client_ip;
  char * next_client_port;
  bool token_available;
  enum protocol protocol;
};

int parse_arguments(client_t * client, char ** args);
void client_clean_up(client_t * client);

#endif
