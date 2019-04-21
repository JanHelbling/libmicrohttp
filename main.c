#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <parse_url.h>
#include <http_func.h>

#define URL "http://www.pradop.ch/"
#define FAIL   -1

int main(int argc,char *argv[]){	
	#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: Running main(), execute http_func()...\n",REDBOLD,NOCOLOR,__FILE__,__func__);
	#endif
	char buf[8192*2];
	int status_code = http_func(URL,buf,8192*2,GET);
	
	#if DEBUG > 0
		if(status_code == -1){
			printf("%s[DEBUG]%s[%s][%s]: http_func() has returned '%d'. %sRequest failed.%s\n",REDBOLD,NOCOLOR,__FILE__,__func__,status_code,RED,NOCOLOR);
		} else {
			printf("%s[DEBUG]%s[%s][%s]: http_func() has returned HTTP-CODE[%d]\n",REDBOLD,NOCOLOR,__FILE__,__func__,status_code);
			char resp[2];
			printf("%s[DEBUG]%s[%s][%s]: Do you want print the data in stdout? (y/n) ",REDBOLD,NOCOLOR,__FILE__,__func__);
			fgets(resp,2,stdin);
			if(resp[0] == 'y' || resp[0] == 'Y'){
				puts(buf);
			}
		}
	#elif DEBUG < 1
		if(status_code != -1)
			printf("URL: %s\nHTTP-Code: %d\n",URL,status_code);
		else
			printf("URL: %s\nRequest Failed!\n",URL);
	#endif
	return 0;
}
