#ifndef __FILESYS_H__

#define __FILESYS_H__

typedef enum tagfiletype
{
	T_NONE = 0,
	T_JPG,
	T_GIF,
	T_TXT,
	T_RAR,
	T_ZIP,
	T_PLN,
	T_PNG,
}filetype;

extern int filesys_getcurrentdir(stbuff * pbuf);
extern int filesys_isafile(char * pstr);
extern int filesys_isdir(char * pstr);
extern filetype filesys_checkfiletype(char * pfilename);
extern int filesys_tolower(char * pfilename);

#endif

