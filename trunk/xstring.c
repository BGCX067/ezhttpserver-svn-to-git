
#include "xstring.h"

//it ought not to be longer the G_MAX_LEN
int strutf8togbk(char * pinput, char * poutput)
{
	int lenint = 0;
	int lenout = 0;
	wchar_t szmid[G_MAX_LINE_LEN];

	memset(szmid, 0, G_MAX_LINE_LEN);
	
	lenint = MultiByteToWideChar(CP_UTF8, 0, pinput, -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, pinput, -1, szmid, lenint);

	lenout = WideCharToMultiByte(CP_ACP, 0, szmid, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0 , szmid, -1, poutput, lenout, NULL, NULL);
	poutput[lenout] = 0;

	return T_OK;

}
int urldecode(char * pinput, char * poutput)
{

}