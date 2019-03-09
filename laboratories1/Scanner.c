#include <regex.h>
#include <stdlib.h>   // NULL
#include <stdbool.h>  // true, false
#include "Scanner.h"

/**
 * args semantic: [client id] [client ip] [client port] [next client's ip]
 *                [next client's port] [does the client has a token at start]
 *                [protocol]
 * args syntax:   (a-z|A-Z|0-9)+ (0-255).(0-255).(0-255).(0-255) \d
 *                (0-255).(0-255).(0-255).(0-255) \d (true|false) (TCP|UDP)
 * example:       r2d2 127.0.0.1 32 127.0.0.1 42 true TCP
 */
bool scan_arguments(int argc, char ** args) {
  if(argc != 8)
    return false;

  bool result;
  match(args[1], "^[a-zA-Z0-9]*$")  == false ? (result = false) :
  match(args[2], IP_PATTERN)        == false ? (result = false) :
  match(args[3], "^[1-9][0-9]*$")   == false ? (result = false) :
  match(args[4], IP_PATTERN)        == false ? (result = false) :
  match(args[5], "^[1-9][0-9]*$")   == false ? (result = false) :
  match(args[6], "^(true|false)$")  == false ? (result = false) :
  match(args[7], "^(TCP|UDP)$")     == false ? (result = false) :
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
