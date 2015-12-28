#include "global.h"
#include "httpserver.h"
#include "sockets.h"
#ifdef _T_WINDOWS_
	#include <winsock2.h>
	#include <process.h>
#endif	
#pragma comment(lib,"ws2_32.lib")


//////////////////////////////////////////////////////////////////////////
T_SOCKET g_socket_server;

//////////////////////////////////////////////////////////////////////////
int initsocket()
{
	T_SOCKADDR_IN addr_server;
#ifdef _T_WINDOWS_
	WSADATA sockdata;
	WSAStartup(0x0202,&sockdata);
#endif
	
	

	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = ADDR_ANY;
	addr_server.sin_port = ntohs(G_LOCAL_PORT);
	
	g_socket_server = socket(AF_INET, SOCK_STREAM, 0);
	bind(g_socket_server, (T_SOCKADDR *)&addr_server, sizeof(addr_server));
	listen(g_socket_server, G_LISTENING_COUNT);
	return T_OK;
}
T_SOCKET acceptsock(paceptfunc callbackfunc)
{
	T_SOCKADDR_IN * paddr_server;
 
	int addr_size = 0;
	sockinfo stsock[G_LISTENING_COUNT];
	sockinfo *psockinfo;

	//arrange sockinfo array for threads
	memset(&stsock, 0, sizeof(sockinfo) * G_LISTENING_COUNT);

	//begin to accept clients
	while(1)
	{
		GET_EMPTY_SOCKINFO(stsock, psockinfo);

		if(psockinfo == NULL)
		{
			//sleep(100);
			continue;
		}
		addr_size = sizeof(T_SOCKADDR_IN);
		paddr_server = &(psockinfo->sockaddr);
		psockinfo->sock = accept(g_socket_server, (T_SOCKADDR*)&paddr_server, &addr_size);
		//ppsockinfo->sock = accept(g_socket_server, (T_SOCKADDR *)&(ppsockinfo->sockaddr), &addr_size);


		f_beginthread(callbackfunc, 0, (void*)(psockinfo));
	}
	//
}
	



