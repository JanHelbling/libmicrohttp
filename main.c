#include <parse_url.h>
#include <http_func.h>
#include <debug.h>

#define URL "https://www.ccczh.ch/"


int main(int argc,char *argv[]){	
	#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: Running main(), execute http_func()...\n",REDBOLD,NOCOLOR,__FILE__,__func__);
	#endif
	char **buf = (char **)malloc(8192*10 + 1);
	int status_code = http_func(URL,buf,8192*10,GET);
	
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
		if(status_code != -1){
			printf(buf);
			printf("URL: %s\nHTTP-Code: %d\n",URL,status_code);
		}
		else{
			printf("URL: %s\nRequest Failed!\n",URL);
		}
	#endif
	free(buf);
	return 0;
}
