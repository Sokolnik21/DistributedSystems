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
  enum message_type previous_sent_message = IDLE; // do I need this?

  token_t token = create_TCP_SEND_token(client);

  struct sockaddr_in client_addr =
    create_TCP_socket(client.client_ip, client.client_port);

  int client_fd;
  client_fd = socket(AF_INET, SOCK_STREAM, 0);

  bind(client_fd, (struct sockaddr *) &client_addr , sizeof(client_addr));
  listen(client_fd , 5);

  while(true) {
    if(client.token_available == true) {
      struct sockaddr_in next_client_addr =
        create_TCP_socket(client.next_client_ip, client.next_client_port);

      int next_client_fd;
      next_client_fd = socket(AF_INET, SOCK_STREAM, 0);
      connect(next_client_fd, (struct sockaddr *) &next_client_addr, sizeof(next_client_addr));

      char * message_to_send = token_to_string(token);
      write(next_client_fd, message_to_send, sizeof(char) * (strlen(message_to_send) + 1));
      free(message_to_send);

      close(next_client_fd);
      client.token_available = false;
    }
    int prev_client_fd;
    prev_client_fd = accept(client_fd, (struct sockaddr *)NULL, NULL);
    client.token_available = true;

    char dataReceived[BUFFER];
    memset(dataReceived, '0', sizeof(dataReceived));
    read(prev_client_fd, dataReceived, sizeof(dataReceived) - 1);

    token_t received_token = string_to_token(dataReceived);
    if(isMessageForClient(received_token, client)) {
      char * sendline;
      switch(received_token.message_type) {
        case SEND:
          printf("%s", received_token.message);
          sendline = create_message(client.client_name);
          free(token.source_ip);
          free(token.source_port);
          free(token.destination_ip);
          free(token.destination_port);
          free(token.message);
          token.source_ip         = malloc(strlen(received_token.destination_ip) + 1);
          token.source_port       = malloc(strlen(received_token.destination_port) + 1);
          token.destination_ip    = malloc(strlen(received_token.source_ip) + 1);
          token.destination_port  = malloc(strlen(received_token.source_port) + 1);
          token.message           = malloc(strlen(sendline) + 1);

          strcpy(token.source_ip,         received_token.destination_ip);
          strcpy(token.source_port,       received_token.destination_port);
          strcpy(token.destination_ip,    received_token.source_ip);
          strcpy(token.destination_port,  received_token.source_port);
          strcpy(token.message, sendline);
          free(sendline);
          token.message_type = ACK;
          break;
        case ACK:
          printf("Message delivered\n");
          token.message_type = IDLE;
          break;
        case IDLE:
          printf("Sending message...\n");
          sendline = create_message(client.client_name);
          free(token.destination_ip);
          free(token.destination_port);
          free(token.message);
          token.destination_ip    = malloc(strlen(client.next_client_ip) + 1);
          token.destination_port  = malloc(strlen(client.next_client_port) + 1);
          token.message           = malloc(strlen(sendline) + 1);

          strcpy(token.destination_ip,    client.next_client_ip);
          strcpy(token.destination_port,  client.next_client_port);
          strcpy(token.message, sendline);
          free(sendline);
          token.message_type = SEND;
          break;
        }
    } else {
      token = received_token;
    }

    close(prev_client_fd);
    sleep(1);
  }

  token_clean_up(&token);
  client_clean_up(&client);
  return 0;
}

token_t create_TCP_SEND_token(client_t client) {
  char * sendline = create_message(client.client_name);
  token_t token = create_token(
    client.client_ip,       client.client_port,
    client.next_client_ip,  client.next_client_port,
    SEND, sendline);
  free(sendline);

  return token;
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

struct sockaddr_in create_TCP_socket(char * source_ip, char * source_port) {
  struct sockaddr_in socket_addr;
  memset(&socket_addr, '0', sizeof(socket_addr));
  socket_addr.sin_family      = AF_INET;
  socket_addr.sin_addr.s_addr = inet_addr(source_ip);
  socket_addr.sin_port        = htons(atoi(source_port));

  return socket_addr;
}

bool isMessageForClient(token_t token, client_t client) {
  if( strcmp(token.destination_port,  client.client_port) == 0 &&
      strcmp(token.destination_ip,    client.client_ip) == 0)
    return true;
  else
    return false;
}
