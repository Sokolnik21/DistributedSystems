#ifndef SCANNER_H
#define SCANNER_H

/** (0-255).(0-255).(0-255).(0-255) */
static char * IP_PATTERN = "^(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$";

bool scan_arguments(int argc, char ** argv);
bool match(char * text, const char * pattern);

#endif
