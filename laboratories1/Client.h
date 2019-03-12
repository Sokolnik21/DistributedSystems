#ifndef CLIENT_H
#define CLIENT_H

#include <stdbool.h>
#include "Token.h"

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

char * create_message(char * client_name);

struct sockaddr_in create_TCP_socket(char * source_ip, char * source_port);

token_t create_TCP_SEND_token(client_t client);
bool isMessageForClient(token_t token, client_t client);

#endif
