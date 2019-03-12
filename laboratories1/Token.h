#ifndef TOKEN_H
#define TOKEN_H

// #include "Client.h"

/** Later it is possible to add option that corresponds with add new client function */
enum message_type { SEND, ACK, IDLE };

typedef struct token_t token_t;
struct token_t {
  char * source_ip;
  char * source_port;
  char * destination_ip;
  char * destination_port;
  enum message_type message_type;
  char * message;
};

token_t create_token(
  char * source_ip,       char * source_port,
  char * destination_ip,  char * destination_port,
  enum message_type message_type,
  char * message);

char * token_to_string(token_t token);
token_t string_to_token(char * string);

char * message_type_to_char(enum message_type type);

void token_clean_up(token_t * token);

#endif
