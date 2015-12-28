#include "sockets.h"
#include "global.h"
#include "httpserver.h"
#include "filesys.h";
#include <stdio.h>


#define RESPONSE_HTML_HEADER "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" /><head><style type=\"text/css\">a {color:#0187C5;text-decoration:none;</style>"	
#define FILE_A_HREF_LINE     "<a href=\"%s\" class='link'>%s</a><br>"
#define FILE_A_HREF_LINE_DIR "<a href=\"%s/\" class='link'>%s</a><br>"
#define RESPONSE_HEADER_PLAINE "HTTP/1.1 200 OK\r\nDate: Sun, 27 Jun 2010 05:53:10 GMT\r\nContent-Type: text/html;charset=gbk\r\n\r\n"	
#define RESPONSE_HEADER_DOWN   "HTTP/1.1 200 OK\r\nDate: Sun, 27 Jun 2010 05:53:10 GMT\r\nContent-Type: application/x-msdownload\r\n\r\n" 
#define RESPONSE_HEADER_PNG  "HTTP/1.1 200 OK\r\nDate: Sun, 27 Jun 2010 05:53:10 GMT\r\nContent-Type: image/png\r\n\r\n" 
#define RESPONSE_HEADER_GIF  "HTTP/1.1 200 OK\r\nDate: Sun, 27 Jun 2010 05:53:10 GMT\r\nContent-Type: image/gif\r\n\r\n" 
#define RESPONSE_HEADER_JPG  "HTTP/1.1 200 OK\r\nDate: Sun, 27 Jun 2010 05:53:10 GMT\r\nContent-Type: image/jpeg\r\n\r\n" 


const char * c_http_err_msg = "<h1>sorry, no files exist</h1>";
char g_szcurrentdir[G_MAX_LINE_LEN];

void  accept_client(sockinfo * psock)
{
	//
	char recvbuff[G_MAX_BUFF_SIZE];
	char geturl[G_MAX_LINE_LEN];
	char szurlgbk[G_MAX_LINE_LEN];

	stbuff l_currentdirbuf;
	//
	T_SOCKET l_sock ;
	T_SOCKADDR_IN * l_psockaddr ;
	//
	l_sock = psock->sock;
	l_psockaddr = &(psock->sockaddr);

	//
	memset(recvbuff, 0, G_MAX_BUFF_SIZE);
	recv(l_sock, recvbuff, G_MAX_BUFF_SIZE, 0);

	memset(geturl, 0, G_MAX_LINE_LEN);
	get_geturl(recvbuff, geturl);

	l_currentdirbuf.bufsize = G_MAX_LINE_LEN;
	l_currentdirbuf.pbuffer = g_szcurrentdir;
	filesys_getcurrentdir(&l_currentdirbuf);
	//printf(stbuff.pbuffer);
	
	//
	strutf8togbk(geturl, szurlgbk);

	//determine the url
	
	//list dir
	if(strlen(szurlgbk) == 0)
	{
		httpserver_senddir(psock,"./");
	}
	else
	{
		if (filesys_isafile(szurlgbk) == T_OK)
		{
			printf("request file : %s\n", szurlgbk);
			httpserver_sendfile(psock, szurlgbk);
		}
		else
		if (filesys_isdir(szurlgbk) == T_OK)
		{
			printf("request dir  : %s\n", szurlgbk);
			httpserver_senddir(psock,szurlgbk);
		}
		else
		{
			httpserver_senderror(psock);
		}
	}

	closesocket(l_sock);
	
	memset(psock, 0, sizeof(sockinfo));	
}


int get_geturl(char * precvbuff, char * purl)
{
	char * phttpflag = NULL;

	if(NULL == precvbuff)
	{
		return T_ERR;
	}
	phttpflag = strstr(precvbuff, FLAG_HTTP);
	if(NULL == phttpflag)
	{
		return T_ERR;
	}
	//split
	 
	//GET /filename.txt HTTP1.1
	strncpy(purl , precvbuff + 5, phttpflag - precvbuff - 6);

	return T_OK;
	
}

///
int httpserver_send(sockinfo * psockinfo, stbuff * pbuff)
{
	return send(psockinfo->sock, pbuff->pbuffer, pbuff->bufsize, 0);
}
int httpserver_sendheader(sockinfo * psockinfo, char * purl)
{
	filetype l_type;

	l_type = filesys_checkfiletype(purl);
	switch (l_type)
	{
	case T_JPG:
		send(psockinfo->sock, RESPONSE_HEADER_JPG, strlen(RESPONSE_HEADER_JPG), 0);
		break;;
	case T_PLN:
		send(psockinfo->sock, RESPONSE_HEADER_PLAINE, strlen(RESPONSE_HEADER_PLAINE), 0);
		break;;
	case T_GIF:
		send(psockinfo->sock, RESPONSE_HEADER_GIF, strlen(RESPONSE_HEADER_JPG), 0);
		break;;
	case T_PNG:
		send(psockinfo->sock, RESPONSE_HEADER_PNG, strlen(RESPONSE_HEADER_PNG), 0);
		break;
	case T_RAR:
	case T_ZIP:
		send(psockinfo->sock, RESPONSE_HEADER_DOWN, strlen(RESPONSE_HEADER_DOWN), 0);
		break;
	default:
		send(psockinfo->sock, RESPONSE_HEADER_DOWN, strlen(RESPONSE_HEADER_DOWN), 0);
	}
	return T_OK;
}
int httpserver_sendfile(sockinfo * psockinfo, char * pfilename)
{
	FILE * pfile = NULL;
	char szsendbuff[G_MAX_BUFF_SIZE];
	char szfilefullpath[G_MAX_LINE_LEN];
	stbuff l_sdbuff;
	int rdlen = 0;
	int ifileallsize = 0;

	char szrealpath[G_MAX_LINE_LEN];
	

	sprintf(szrealpath, "./%s",pfilename);
	pfile = fopen(szrealpath, "rb");

	rdlen = GetLastError();
	if (pfile == NULL)
	{
		return T_ERR;
	}

	httpserver_sendheader(psockinfo, szrealpath);
	while (rdlen = fread(szsendbuff, 1, G_MAX_BUFF_SIZE,pfile))
	{
		//ifileallsize += rdlen;
		l_sdbuff.bufsize = rdlen;
		l_sdbuff.pbuffer = szsendbuff;
		ifileallsize += httpserver_send(psockinfo, &l_sdbuff);
		memset(szsendbuff, 0, G_MAX_BUFF_SIZE);
	}
	fclose(pfile);
}
int httpserver_senddir(sockinfo * psockinfo, char * pdir)
{
	WIN32_FIND_DATA finddata;
	HANDLE hfind = NULL;
	char sztemppath[G_MAX_LINE_LEN];
	char szsendbuff[G_MAX_BUFF_SIZE];
	char szfilepath[G_MAX_LINE_LEN];

	int ipos = 0;
	stbuff l_buff;

	//nullify
	memset(szsendbuff, 0, G_MAX_BUFF_SIZE);
	sprintf(sztemppath, "%s/*.*", pdir);


	send(psockinfo->sock, RESPONSE_HTML_HEADER, strlen(RESPONSE_HTML_HEADER), 0);	
	hfind = FindFirstFile(sztemppath, &finddata);

	do
	{	
		sprintf(sztemppath, "%s/%s", pdir, finddata.cFileName);
		if (filesys_isdir(sztemppath))
		{
			ipos += sprintf(szsendbuff + ipos, FILE_A_HREF_LINE_DIR, finddata.cFileName, finddata.cFileName);
		}
		else
		{
			ipos += sprintf(szsendbuff + ipos, FILE_A_HREF_LINE, finddata.cFileName, finddata.cFileName);
		}
	}
	while (FindNextFile(hfind, &finddata));

	l_buff.bufsize = G_MAX_BUFF_SIZE;
	l_buff.pbuffer = szsendbuff;
	httpserver_send(psockinfo, &l_buff);
}

	
int httpserver_senderror(sockinfo * psockinfo)
{ 
	stbuff stbf ;
	int sendsize = 0;
	int rt = -1;

	stbf.bufsize = strlen(c_http_err_msg);
	stbf.pbuffer = c_http_err_msg;
	
	rt = httpserver_send(psockinfo, &stbf);

	if(rt <= 0)
	{
		return T_ERR;
	}
	return T_OK;

}
int httpserver_init()
{
	initsocket();
}
int httpserver_main()
{
	acceptsock(accept_client);
}



