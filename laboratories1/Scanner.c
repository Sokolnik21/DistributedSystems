#include <regex.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Scanner.h"

/**
 * args semantic: [client id] [next client's ip and port]
 *                [does the client has a token at start] [protocol]
 * args syntax:   (a-z|A-Z|0-9)* (0-255).(0-255).(0-255).(0-255):(0-32768)
                  (true|false) (TCP|UDP)
 * example:       r2d2 192.165.17.87:22 true TCP
 */
bool scan_arguments(int argc, char ** args) {
  if(argc != 5)
    return false;

  bool result;
  match(args[1], "^[a-zA-Z0-9]*$")  == false ? (result = false) :
  /** (0-255).(0-255).(0-255).(0-255):(1-9)*(0-9) */
  match(args[2], IP_PATTERN)        == false ? (result = false) :
  match(args[3], "^(true|false)$")  == false ? (result = false) :
  match(args[4], "^(TCP|UDP)$")     == false ? (result = false) :
  (result = true);
  return result;
}

bool match(char * text, const char * pattern) {
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED) != 0)
      return false;
    int status = regexec(&re, text, 0, NULL, 0);
    regfree(&re);
    if (status != 0)
      return false;
    return true;
}
