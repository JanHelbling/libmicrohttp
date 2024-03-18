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
	// int status_code = http_post(URL,buf,8192*10,"abfrage=1");
	// Get-Request
	// int status_code = http_get(URL,buf,8192*10);
	// Head-Request
	// int status_code = http_head(URL,buf,8192*10);
	// Put-Request
	// int status_code = http_put(URL,buf,8192*10,"abfrage=1");
	// Delete-Request
	// int status_code = http_delete(URL,buf,8192*10);
	// Options-Request
	// int status_code = http_options(URL,buf,8192*10);

	int num;
	for(num=1;num<argc;num++){
		// Get-Request
		int status_code = http_get(argv[num],buf,8192*10);

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

	// Extract the Body
	char *body = malloc(8192*10);

	int r = extract_body(buf,body);
	free(body);

	// Extract all URLs
	char **urls = malloc(8192*10);
	int n = extract_all_urls(buf,urls);

	// Print all URLs
	int i;
	for(i=0;i<n;i++){
		printf("%s\n",urls[i]);
	}

	free(urls);
	free(buf);

	return 0;
}
