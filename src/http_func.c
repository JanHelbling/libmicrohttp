#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <errno.h>

#include <libmicrohttp.h>

#define FAIL -1

SSL_CTX* InitCTX(void)
{
	OpenSSL_add_all_algorithms();		/* Load cryptos, et.al. */
	SSL_load_error_strings();			/* Bring in and register error messages */
	SSL_METHOD *method = SSLv23_client_method();	/* Create new client-method instance */
	SSL_CTX *ctx = SSL_CTX_new(method);			/* Create new context */
	if ( ctx == NULL )
	{
		ERR_print_errors_fp(stderr);
		abort();
	}
	return ctx;
}

int do_connect(url *u)
{


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
	if(u->scheme[4] == 's')
		addr.sin_port           = (in_port_t)htons(443);
	else
		addr.sin_port			= (in_port_t)htons(80);

#if DEBUG > 0
	printf("%s[DEBUG]%s[%s][%s]: creating socket and connect to '%s'...\n",REDBOLD,NOCOLOR,__FILE__,__func__,ip_addr);
#endif

	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int rc = connect(sock, (struct sockaddr *)&addr, sizeof(addr));

	if(rc == -1){
#if DEBUG > 0
		char error[20];
		if(errno == ECONNREFUSED)
			memcpy(error,"Connection refused.",19);
		else if(errno == ECONNABORTED)
			memcpy(error,"Connection aborted.",19);
		printf("%s[DEBUG]%s[%s][%s]: connect('%s',80) failed: '%s'.\n",REDBOLD,NOCOLOR,__FILE__,__func__,ip_addr,error);
#endif
		if(u->scheme[4] == 's')
			perror("Could not connect to port 443");
		else
			perror("Could not connect to port 80");
		return -1;
	}
	return sock;
}
int ShowCerts(SSL* ssl)
{   
	X509 *cert;
	char *line;

	cert = SSL_get_peer_certificate(ssl);	/* get the server's certificate */
#if DEBUG > 0
	if ( cert != NULL )
	{
		printf("%s[DEBUG]%s[%s][%s]: Server certificates:\n",REDBOLD,NOCOLOR,__FILE__,__func__);
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("%s[DEBUG]%s[%s][%s]: Subject: %s\n",REDBOLD,NOCOLOR,__FILE__,__func__, line);
		free(line);							/* free the malloc'ed string */
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("%s[DEBUG]%s[%s][%s]: Issuer: %s\n",REDBOLD,NOCOLOR,__FILE__,__func__, line);
		free(line);							/* free the malloc'ed string */
		X509_free(cert);					/* free the malloc'ed certificate copy */
	}
	else{
		printf("%s[DEBUG]%s[%s][%s]: No certificates.\n",REDBOLD,NOCOLOR,__FILE__,__func__);
	}
#endif
}

int http_func(const char *fullurl, char *buffer, int num, int method,const char *post_string)
{
	SSL_CTX *ctx = NULL;
	int server;
	SSL *ssl = NULL;
	char buf[num];
	int bytes;

	url *u = parse_url(fullurl);
	server = do_connect(u);
	char mt[5] = {0x00};

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

	if(u->scheme[4] != 's'){
		//
		// HTTP-Request
		//
#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: %s-Request on url '%s' and save %d bytes...\n",REDBOLD,NOCOLOR,__FILE__,__func__,mt,fullurl,num);
#endif

#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: resolve ip-address from host '%s'...\n",REDBOLD,NOCOLOR,__FILE__,__func__,u->hostname);
#endif

		char *send_buffer = (char *)malloc(8193);
		memset(send_buffer,0x00,8192);

		if(post_string == NULL){
			snprintf(send_buffer,8192,"%s %s%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\n\r\n",mt,u->path,u->query,u->hostname);
		} else {
			snprintf(send_buffer,8192,"%s %s%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %zu\r\n\r\n%s",mt,u->path,u->query,u->hostname,strlen(post_string),post_string);
		}

#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: send http-request: [%d bytes]...\n",REDBOLD,NOCOLOR,__FILE__,__func__,strlen(send_buffer));
#endif

		int rc = send(server,send_buffer,strlen(send_buffer),0);

#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: [%d] from [%d] bytes sendt.\n",REDBOLD,NOCOLOR,__FILE__,__func__,rc,strlen(send_buffer));
#endif

		free(send_buffer);

		if(rc == -1){
			perror("Failed to send HTTP Request");
			return 0;
		}

		char buf[1] = {0x00};
		int y = 0;

#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: recving http-response in progress!\n",REDBOLD,NOCOLOR,__FILE__,__func__);
#endif

		while(1){
			rc = recv(server,buf,1,0);
			if(rc == -1 || rc == 0){
				break;
			}
			if(num == y)
				break;
			buffer[y] = buf[0];
			y++;
		}
		buffer[++y] = 0x00;

#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: http-respopnse received: exactly %d bytes received!\n",REDBOLD,NOCOLOR,__FILE__,__func__,y);
#endif
	} else {
		//
		// HTTPS-Request
		//
#if DEBUG > 0
		printf("%s[DEBUG]%s[%s][%s]: %s-Request on url '%s' and save %d bytes...\n",REDBOLD,NOCOLOR,__FILE__,__func__,mt,fullurl,num);
#endif

		// register the available SSL/TLS ciphers and digests
		SSL_library_init();
		ctx = InitCTX();

		ssl = SSL_new(ctx);			// create a new SSL connection state
		SSL_set_fd(ssl, server);	// attach the socket descriptor

		if (SSL_connect(ssl) == FAIL)
			ERR_print_errors_fp(stderr);
		else {
			char *send_buffer = (char *)malloc(8192);
			memset(send_buffer,0,8192);
			if(post_string == NULL){
				sprintf(send_buffer,"%s %s%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\n\r\n",mt,u->path,u->query,u->hostname);
			} else {
				sprintf(send_buffer,"%s %s%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %zu\r\n\r\n%s",mt,u->path,u->query,u->hostname,strlen(post_string),post_string);
			}
#if DEBUG > 0
			printf("%s[DEBUG]%s[%s][%s] Connected with %s encryption\n", REDBOLD,NOCOLOR,__FILE__,__func__, SSL_get_cipher(ssl));
#endif

			ShowCerts(ssl);
			SSL_write(ssl, send_buffer, strlen(send_buffer));       //encrypt and send message

			free(send_buffer);
			char b[1];
			strcpy(buffer,"");
			int i=0,n=0;
			while(++i){
				n = SSL_read(ssl, b, 1);      //get reply and decrypt
				if(n <= 0 || n == num - 1)
					break;
				bytes += n;
				strncat(buffer,b,1);
			}
			SSL_free(ssl);
		}
	}
	char code[3] = {0};
	memcpy(code,(char *)buffer+9,3);
	return atoi(code);
}
