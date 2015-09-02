#include <parse_url.h>
#include <http_func.h>

int main(int argc,char *argv[]){	
	#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: Running main(), execute http_func()...\n",REDBOLD,NOCOLOR,__FILE__,__func__);
	#endif
	char buf[8192*2];
	int status_code = http_func("http://www.openbsd.org/",buf,8192*2,GET);
	
	#if DEBUG > 0
		if(status_code == -1)
			printf("%s[DEBUG]%s[%s][%s]: http_func() has returned '%d'. %sRequest failed.%s\n",REDBOLD,NOCOLOR,__FILE__,__func__,status_code,RED,NOCOLOR);
		else
			printf("%s[DEBUG]%s[%s][%s]: http_func() has returned HTTP-CODE[%d]\n",REDBOLD,NOCOLOR,__FILE__,__func__,status_code);
	#endif
	return 0;
}
