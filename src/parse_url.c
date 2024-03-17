#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libmicrohttp.h>

url *parse_url(const char *unparsed_url){
	url *u = (url *)malloc(8192);

#if DEBUG > 0
	printf("%s[DEBUG]%s[%s][%s]: parse url '%s'...\n",REDBOLD,NOCOLOR,__FILE__,__func__,unparsed_url);
#endif

	memset(u->scheme,0,16);
	memset(u->hostname,0,64);
	memset(u->path,0,256);
	memset(u->query,0,256);
	memset(u->fragment,0,64);
	memset(u->full_url,0,656);

	int i;

	if(unparsed_url[0] == 'h' && unparsed_url[6] == '/'){
		for(i=0;i<strlen(unparsed_url);i++){
			u->scheme[i] = unparsed_url[i];
			if(i != 0 && u->scheme[i - 1] == '/' && unparsed_url[i] == '/'){
				break;
			}
		}
		unparsed_url += strlen(u->scheme);
	} else {
		memcpy(u->scheme,"http://",7);
	}

	for(i=0;unparsed_url[i] != '/';i++){
		u->hostname[i] = unparsed_url[i];
	}

	unparsed_url += strlen(u->hostname);

	if(strlen(unparsed_url) > 1){
		if(strstr(unparsed_url,"?") != NULL){
			for(i=0;unparsed_url[i] != '?';i++){
				u->path[i] = unparsed_url[i];
			}
		} else {
			for(i=0;i<strlen(unparsed_url);i++){
				u->path[i] = unparsed_url[i];
			}
		}
	} else if((strlen(unparsed_url) == 1 && unparsed_url[0] == '/')){
		u->path[0] = '/';
	} else {
		u->path[0] = '/';
		return u;
	}

	if(u->path != NULL){
		unparsed_url += strlen(u->path);
	}	
	if(strstr(unparsed_url,"#") == NULL){
		for(i=0;i<strlen(unparsed_url);i++){
			u->query[i] = unparsed_url[i];
		}
	} else {
		for(i=0;unparsed_url[i] != '#';i++){
			u->query[i] = unparsed_url[i];
		}
		unparsed_url += strlen(u->query);
		for(i=0;i<strlen(unparsed_url);i++){
			u->fragment[i] = unparsed_url[i];
		}
	}

	unparsed_url -= (strlen(u->scheme) + strlen(u->hostname) + strlen(u->path) + strlen(u->query));

	strncpy(u->full_url,u->scheme,strlen(u->scheme));
	strncat(u->full_url,u->hostname,strlen(u->hostname));
	strncat(u->full_url,u->path,strlen(u->path));
	strncat(u->full_url,u->query,strlen(u->query));

	return u;
}
