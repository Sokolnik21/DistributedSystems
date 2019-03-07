#ifndef SCANNER_H
#define SCANNER_H

/**
 * That is soooo bad but I've got no time
 * better implementation:
 * specify (0-255) regex then use it with strcat - reduces that monster 4 times
 */
static char * IP_PATTERN = "^([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]|[0-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]):[1-9][0-9]*$";

bool scan_arguments(int argc, char ** argv);
bool match(char * text, const char * pattern);

#endif
