#include <stdio.h>
#include <stdlib.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#define RED		"\x1b\x5b\x33\x31\x6d"
#define REDBOLD		"\x1b\x5b\x31\x3b\x33\x31\x6d"
#define GREEN		"\x1b\x5b\x33\x32\x6d"
#define GREENBOLD	"\x1b\x5b\x31\x3b\x33\x32\x6d"
#define YELLOW		"\x1b\x5b\x30\x3b\x33\x33\x6d"
#define	NOCOLOR		"\x1b\x5b\x30\x6d"

typedef enum {
	GET = 0,
	HEAD = 1,
	POST = 2
} req;

typedef struct {
	char scheme[16];
	char hostname[64];
	char path[256];
	char query[256];
	char fragment[64];
	char full_url[656];
} url;

url *parse_url(const char *unparsed_url);

int http_func(const char *fullurl,char *buffer,int num, int method,const char *post_string);
