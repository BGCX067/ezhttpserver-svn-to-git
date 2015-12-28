#include "global.h"
#include "filesys.h"
#include "sockets.h"
#include "httpserver.h"
#include <Windows.h>
#include <sys/stat.h>

int filesys_getcurrentdir(stbuff * pbuf)
{
#ifdef _T_WINDOWS_
	GetCurrentDirectory( pbuf->bufsize,pbuf->pbuffer);
#else
	//
#endif
	return T_OK;
}

int filesys_isafile(char * pstr)
{
	struct stat l_st;
	_stat(pstr, &l_st);

	if ( l_st.st_mode & _S_IFDIR )
	{
		return T_ERR;
	}
	if ( l_st.st_mode & _S_IFREG )
	{
		return T_OK;
	}
	return T_ERR;
}
int filesys_isdir(char * pstr)
{
	struct stat l_st;
	_stat(pstr, &l_st);

	if ( l_st.st_mode & _S_IFDIR)
	{
		return T_OK;
	}
	return T_ERR;
}

filetype filesys_checkfiletype(char * pfilename)
{ 
	filesys_tolower(pfilename);
	if (strstr(pfilename, ".gif"))
	{
		return T_GIF;
	}
	if (strstr(pfilename, ".txt"))
	{
		return T_TXT;
	}
	if (strstr(pfilename, ".jpg"))
	{
		return T_JPG;
	}
	if (strstr(pfilename, ".rar"))
	{
		return T_RAR;
	}
	if (strstr(pfilename, ".png"))
	{
		return T_PNG;
	}
	if (strstr(pfilename, ".htm"))
	{
		return T_PLN;
	}

	return T_NONE;
}

int filesys_tolower(char * pfilename)
{
	int i = 0;
	for (i = 0;i < strlen(pfilename); i++)
	{
		if (pfilename[i] >= 'A' && pfilename[i] <= 'Z')
		{
			pfilename[i] = pfilename[i] + 'a' - 'A';
		}
	}
}








