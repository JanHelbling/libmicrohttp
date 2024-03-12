#include <libmicrohttp.h>

int main(int argc,char *argv[]){
	if(argc < 2){
		printf("Usage: %s [[url1], url2, ...]\n",argv[0]);
		return 0;
	}
#if DEBUG > 0
	printf("%s[DEBUG]%s[%s][%s]: Running main(), execute http_func()...\n",REDBOLD,NOCOLOR,__FILE__,__func__);
#endif
	char *buf = (char *)malloc(8192*10 + 1);
	// Post-Request
	// int status_code = http_func(URL,buf,8192*10,POST,"abfrage=1");
	
	int num;
	for(num=1;num<argc;num++){
		// Get-Request
		int status_code = http_func(argv[num],buf,8192*10,GET,NULL);

	#if DEBUG > 0
		if(status_code == -1){
			printf("%s[DEBUG]%s[%s][%s]: http_func() has returned '%d'. %sRequest failed.%s\n",REDBOLD,NOCOLOR,__FILE__,__func__,status_code,RED,NOCOLOR);
		} else {
			printf("%s[DEBUG]%s[%s][%s]: http_func() has returned HTTP-CODE[%d]\n",REDBOLD,NOCOLOR,__FILE__,__func__,status_code);
			char resp[2];
			printf("%s[DEBUG]%s[%s][%s]: Do you want print the data in stdout? (y/n) ",REDBOLD,NOCOLOR,__FILE__,__func__);
			fgets(resp,2,stdin);
			if(resp[0] == 'y' || resp[0] == 'Y'){
				printf("%s\n",buf);
			}
		}
	#elif DEBUG < 1
		if(status_code != -1){
			printf("%s\n", buf);
			printf("URL: %s\nHTTP-Code: %d\n",argv[num],status_code);
		}
		else{
			printf("URL: %s\nRequest Failed!\n",argv[num]);
		}
	#endif
	}
	free(buf);
	return 0;
}
