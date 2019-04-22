typedef enum {
        GET = 0,
        HEAD = 1,
        POST = 2
} req;

int http_func(const char *fullurl,char *buffer,int num, int method,const char *post_string);
