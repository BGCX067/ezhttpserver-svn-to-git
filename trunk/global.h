#ifndef __GLOBAL_H__

#define __GLOBAL_H__
#define _T_WINDOWS_

#include <WinSock2.h>

enum
{
	T_ERR = 0,
	T_OK  = 1,
};

#ifdef _T_WINDOWS_
	#define T_SOCKET SOCKET
	#define f_beginthread _beginthread
	//#define T_SOCKADDR_IN SOCKADDR_IN
	//#define T_SOCKADDR    SOCKADDR

	typedef struct sockaddr_in T_SOCKADDR_IN;
	typedef struct sockaddr	T_SOCKADDR;
#else
	
#endif


typedef struct tagbuff
{
	int    bufsize;
	char * pbuffer;
}stbuff;

//////////////////////////////////////////////////////////////////////////
//
// global configuration
#define G_LOCAL_PORT 10086

#define G_LISTENING_COUNT 10

#define G_MAX_BUFF_SIZE 10240 * 4
#define G_MAX_LINE_LEN 512

#endif
