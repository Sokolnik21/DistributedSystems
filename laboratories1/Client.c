#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include "Client.h"
#include "Scanner.h"

int main(int argc, char ** argv) {
  if(scan_arguments(argc, argv) == false) {
    fprintf(stderr, "Invalid arguments syntax\n");
    exit(-1);
  }

  char * client_id;
  char * next_client_ip;
  char * next_client_port;
  bool token_available;
  enum protocol protocol;

  printf("Hello world\n");
  return 0;
}
