#include <string.h>
#include <stdlib.h>
#include "Token.h"
// #include "Client.h"

token_t create_token(
    char * source_ip,       char * source_port,
    char * destination_ip,  char * destination_port,
    enum message_type message_type,
    char * message) {
  token_t token;
  token.source_ip         = malloc(sizeof(char) * (strlen(source_ip) + 1));
  token.source_port       = malloc(sizeof(char) * (strlen(source_port) + 1));
  token.destination_ip    = malloc(sizeof(char) * (strlen(destination_ip) + 1));
  token.destination_port  = malloc(sizeof(char) * (strlen(destination_port) + 1));
  token.message           = malloc(sizeof(char) * (strlen(message) + 1));
  strcpy(token.source_ip,         source_ip);
  strcpy(token.source_port,       source_port);
  strcpy(token.destination_ip,    destination_ip);
  strcpy(token.destination_port,  destination_port);
  token.message_type =            message_type;
  strcpy(token.message,           message);

  return token;
}

// token_t create_TCP_SEND_token(client_t client) {
//   char * sendline = create_message(client.client_name);
//   token_t token = create_token(
//     client.client_ip,       client.client_port,
//     client.next_client_ip,  client.next_client_port,
//     SEND, sendline);
//   free(sendline);
//
//   return token;
// }

/** Also allocates memory - needs to be freed at the end */
char * token_to_string(token_t token) {
  long int result_length = 0;
  result_length += sizeof(char) * strlen(token.source_ip);
  result_length += sizeof(char) * strlen(token.source_port);
  result_length += sizeof(char) * strlen(token.destination_ip);
  result_length += sizeof(char) * strlen(token.destination_port);
  result_length += sizeof(char(enum message_type));
  result_length += sizeof(char) * strlen(token.message);
  result_length += sizeof(char) * 5;

  char * result = malloc(sizeof(char) * (result_length + 1));
  strcpy(result, "");
  strcat(result, token.source_ip);
  strcat(result, "|");
  strcat(result, token.source_port);
  strcat(result, "|");
  strcat(result, token.destination_ip);
  strcat(result, "|");
  strcat(result, token.destination_port);
  strcat(result, "|");
  strcat(result, message_type_to_char(token.message_type));
  strcat(result, "|");
  strcat(result, token.message);
  return result;
}

/** ex. string: "127.0.0.1|32|127.0.0.1|42|S|Hello network" */
token_t string_to_token(char * string) {
  char * string_copy = malloc(strlen(string) + 1);
  strcpy(string_copy, string);

  token_t token;
  char * lex_token = NULL;
  lex_token = strtok(string_copy, "|");
  token.source_ip = malloc(sizeof(char) * (strlen(lex_token) + 1));
  strcpy(token.source_ip, lex_token);
  lex_token = strtok(NULL, "|");
  token.source_port = malloc(sizeof(char) * (strlen(lex_token) + 1));
  strcpy(token.source_port, lex_token);
  lex_token = strtok(NULL, "|");
  token.destination_ip = malloc(sizeof(char) * (strlen(lex_token) + 1));
  strcpy(token.destination_ip, lex_token);
  lex_token = strtok(NULL, "|");
  token.destination_port = malloc(sizeof(char) * (strlen(lex_token) + 1));
  strcpy(token.destination_port, lex_token);
  lex_token = strtok(NULL, "|");
  switch(lex_token[0]) {
    case 'A'  : token.message_type = ACK; break;
    case 'S'  : token.message_type = SEND; break;
    case 'I'  : token.message_type = IDLE; break;
  }
  lex_token = strtok(NULL, "|");
  token.message = malloc(sizeof(char) * (strlen(lex_token) + 1));
  strcpy(token.message, lex_token);

  free(string_copy);
  return token;
}

char * message_type_to_char(enum message_type type) {
  switch(type) {
    case SEND : return "S";
    case ACK  : return "A";
    case IDLE : return "I";
  }
}

void token_clean_up(token_t * token) {
  free(token -> source_ip);
  free(token -> source_port);
  free(token -> destination_ip);
  free(token -> destination_port);
  free(token -> message);
}
