#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
/** Network communication */
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "Client.h"
#include "Scanner.h"
#include "Token.h"

#define BUFFER 1024

int main(int argc, char ** argv) {
  if(scan_arguments(argc, argv) == false) {
    fprintf(stderr, "Invalid arguments syntax\n");
    exit(-1);
  }

  client_t client;
  parse_arguments(&client, argv);

  char * sendline = create_message(client.client_name);
  token_t token = create_token(
    client.client_ip,       client.client_port,
    client.next_client_ip,  client.next_client_port,
    SEND, sendline);
  free(sendline);

  struct sockaddr_in client_addr;
  memset(&client_addr, '0', sizeof(client_addr));
  client_addr.sin_family      = AF_INET;
  client_addr.sin_addr.s_addr = inet_addr(token.source_ip);
  client_addr.sin_port        = htons(atoi(token.source_port));

  int client_fd, next_client_fd;
  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  bind(client_fd, (struct sockaddr *) &client_addr , sizeof(client_addr));
  listen(client_fd , 5);

  while(true) {
    if(client.token_available == true) {
      struct sockaddr_in next_client_addr;
      memset(&next_client_addr, '0', sizeof(next_client_addr));
      next_client_addr.sin_family       = AF_INET;
      next_client_addr.sin_addr.s_addr  = inet_addr(token.destination_ip);
      next_client_addr.sin_port         = htons(atoi(token.destination_port));

      int next_client_fd = 0;
      next_client_fd = socket(AF_INET, SOCK_STREAM, 0);
      connect(next_client_fd, (struct sockaddr *) &next_client_addr, sizeof(next_client_addr));

      char * message_to_send = token_to_string(token);
      write(next_client_fd, message_to_send, sizeof(char) * (strlen(message_to_send) + 1));
      free(message_to_send);

      close(next_client_fd);
      client.token_available = false;
    }
    next_client_fd = accept(client_fd, (struct sockaddr *)NULL, NULL);
    client.token_available = true;

    char dataReceived[BUFFER];
    memset(dataReceived, '0', sizeof(dataReceived));
    int n;
    while((n = read(next_client_fd, dataReceived, sizeof(dataReceived) - 1)) > 0) {
        dataReceived[n] = 0;
        token_t received_token = string_to_token(dataReceived);
        printf("%s", dataReceived);
        printf("%s", received_token.message);
    }
    if( n < 0) {
        printf("Standard input error \n");
    }

    close(next_client_fd);
    sleep(1);
  }

  token_clean_up(&token);
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

  return 0;
}

char * create_message(char * client_name) {
  char * message = malloc(sizeof(char) *
    (strlen("Hi! It's ") + strlen(client_name) + strlen(" \n")) + 1);
  strcpy(message, "");
  strcat(message, "Hi! It's ");
  strcat(message, client_name);
  strcat(message, " \n");

  return message;
}

void client_clean_up(client_t * client) {
  free(client -> client_name);
  free(client -> client_ip);
  free(client -> client_port);
  free(client -> next_client_ip);
  free(client -> next_client_port);
}
