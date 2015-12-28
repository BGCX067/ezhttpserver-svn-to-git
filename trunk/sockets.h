#ifndef __SOCKETS_H__
#define __SOCKETS_H__

#include <WinSock2.h>

#include "global.h"
#include "httpserver.h"
#include "sockets.h"


	typedef void (*paceptfunc)(struct sockinfo * psockinfo);

	typedef struct tagsockinfo
	{
		T_SOCKET sock;
		T_SOCKADDR_IN sockaddr;
	}sockinfo;
		

	#define GET_EMPTY_SOCKINFO(psockarray,psockinfo)  \
	do{\
		   int i=0;\
		   for(i=0;i<G_LISTENING_COUNT;i++)\
		   {\
		   		if(psockarray[i].sock==0)\
		   		{\
		   			psockinfo=&(psockarray[i]);\
		   			break;\
		   		}\
		   }\
	   }while(0);

	//////////////////////
	extern int initsocket();
	extern T_SOCKET acceptsock(paceptfunc  callbackfunc);

#endif

