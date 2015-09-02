#include <parse_url.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

#include <errno.h>

typedef enum {
	GET = 0,
	HEAD = 1,
	POST = 2
} req;

int http_func(const char *fullurl,char *buffer,int num, int method){
	char mt[8] = {NULL};

        switch(method){
                case GET:
                        memcpy(mt,"GET",3);
                        break;
                case HEAD:
                        memcpy(mt,"HEAD",4);
                        break;
                case POST:
                        memcpy(mt,"POST",4);
                        break;
                default:
			memcpy(mt,"GET",3);
                        break;
        }
	
	#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: %s-Request on url '%s' and save %d bytes...\n",REDBOLD,NOCOLOR,__FILE__,__func__,mt,fullurl,num);
	#endif
	
	url *u = parse_url(fullurl);
	
	#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: resolve ip-address from host '%s'...\n",REDBOLD,NOCOLOR,__FILE__,__func__,u->hostname);
	#endif
	
	struct hostent *hent = gethostbyname(u->hostname);
	
	if(!hent){
		#if DEBUG > 0
			printf("%s[DEBUG]%s[%s][%s]: gethostbyname('%s') failed: %s.\n",REDBOLD,NOCOLOR,__FILE__,__func__,u->hostname,hstrerror(h_errno));
		#endif
		herror("Can't resolve the address");
		return -1;
	}
	
	char ip_addr[16];
	memcpy(ip_addr,inet_ntoa(*(struct in_addr *)hent->h_addr_list[0]),16);
	
	struct sockaddr_in addr;
	addr.sin_addr.s_addr	= inet_addr(ip_addr);
	addr.sin_family		= AF_INET;
	addr.sin_port		= htons(80);
	
	#if DEBUG > 0
                printf("%s[DEBUG]%s[%s][%s]: creating socket and connect to '%s'...\n",REDBOLD,NOCOLOR,__FILE__,__func__,ip_addr);
        #endif
	
	int sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	int rc = connect(sock,(struct sockaddr *)&addr,sizeof(addr));
	
	if(rc == -1){
		#if DEBUG > 0
			char error[32];
			if(errno == ECONNREFUSED)
				memcpy(error,"Connection refused.",19);
			else if(errno == ECONNABORTED)
				memcpy(error,"Connection aborted.",19);
			printf("%s[DEBUG]%s[%s][%s]: connect('%s',80) failed: '%s'.\n",REDBOLD,NOCOLOR,__FILE__,__func__,ip_addr,error);
		#endif
		perror("Could not connect to port 80");
		return -1;
	}
	
	char *send_buffer = (char *)malloc(8192);
	memset(send_buffer,NULL,8192);
	
	sprintf(send_buffer,"%s %s%s HTTP/1.0\nHost: %s\nUser-Agent: Mozilla/5.0\n\n",mt,u->path,u->query,u->hostname);
	
	#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: send http-request: [%d bytes]...\n",REDBOLD,NOCOLOR,__FILE__,__func__,strlen(send_buffer));
	#endif
	
	rc = send(sock,send_buffer,strlen(send_buffer),0);
	
	#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: [%d] from [%d] bytes sendt.\n",REDBOLD,NOCOLOR,__FILE__,__func__,rc,strlen(send_buffer));
	#endif
	
	free(send_buffer);
	
	if(rc == -1){
		perror("Failed to send HTTP Request");
		return 0;
	}
	
	char buf[1] = {NULL};
	int y = 0;
	
	#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: recving http-response in progress!\n",REDBOLD,NOCOLOR,__FILE__,__func__);
	#endif
	
	while(rc != -1){
		rc = recv(sock,buf,1,0);
		if(rc == -1 || rc == 0){
			break;
		}
		if(num == y)
			break;
		buffer[y] = buf[0];
		y++;
	}
	buffer[++y] = NULL;
	
	#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: http-respopnse received: exactly %d bytes received!\n",REDBOLD,NOCOLOR,__FILE__,__func__,y);
	#endif
	
	buffer += 9;
	char code[3] = {NULL};
	memcpy(code,buffer,3);
	buffer -= 9;
	return atoi(code);
}
