#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

	#include <WinSock2.h>
	#include "global.h"
	#include "httpserver.h"
	#include "sockets.h"


	#define FLAG_GET "GET"
	#define FLAG_POST "POST"

	#define FLAG_LINE "\r\n"
	#define FLAG_HTTP "HTTP"


	//
	extern int get_geturl(char * precvbuff, char * purl);
	//extern int accept_client(sockinfo * psock);


	//////////////////////////////////////////////////////////////////////////

#endif
