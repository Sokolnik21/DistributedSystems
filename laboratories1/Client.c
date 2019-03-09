#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Client.h"
#include "Scanner.h"
#include "Token.h"

int main(int argc, char ** argv) {
  if(scan_arguments(argc, argv) == false) {
    fprintf(stderr, "Invalid arguments syntax\n");
    exit(-1);
  }

  client_t client;
  parse_arguments(&client, argv);

  // USED FOR TESTING. NEEDS TO BE DELETED IN NEXT ITERATION
  // token_t token = string_to_token("127.0.0.1|32|127.0.0.1|42|S|Hello network");
  //
  // printf("%s\n", token.source_ip);
  // printf("%s\n", token.source_port);
  // printf("%s\n", token.destination_ip);
  // printf("%s\n", token.destination_port);
  // printf("%s\n", token.message);
  //
  // char * token2 = token_to_string(token);
  // printf("%s\n", token2);
  // free(token2);

  // token_clean_up(&token);
  client_clean_up(&client);
  return 0;
}

int parse_arguments(client_t * client, char ** args) {
  client -> client_name       = malloc(sizeof(char) * ( strlen(args[1]) + 1 ));
  client -> client_ip         = malloc(sizeof(char) * ( strlen(args[2]) + 1 ));
  client -> client_port       = malloc(sizeof(char) * ( strlen(args[3]) + 1 ));
  client -> next_client_ip    = malloc(sizeof(char) * ( strlen(args[4]) + 1 ));
  client -> next_client_port  = malloc(sizeof(char) * ( strlen(args[5]) + 1 ));

  strcpy(client -> client_name, args[1]);
  strcpy(client -> client_ip, args[2]);
  strcpy(client -> client_port, args[3]);
  strcpy(client -> next_client_ip, args[4]);
  strcpy(client -> next_client_port, args[5]);
  (strcmp("true", args[6]) == 0) ?
    (client -> token_available = true) : (client -> token_available = false);
  (strcmp("TCP", args[7]) == 0) ?
    (client -> protocol = TCP) : (client -> protocol = UDP);
}

void client_clean_up(client_t * client) {
  free(client -> client_name);
  free(client -> client_ip);
  free(client -> client_port);
  free(client -> next_client_ip);
  free(client -> next_client_port);
}
