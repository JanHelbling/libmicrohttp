#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <shell_colors.h>

#ifndef DEBUG
	#define DEBUG 3
#endif

typedef struct {
	char scheme[16];
	char hostname[64];
	char path[256];
	char query[256];
	char fragment[64];
	char full_url[656];
} url;

url *parse_url(const char *unparsed_url);
