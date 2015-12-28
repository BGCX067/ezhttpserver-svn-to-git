#include "xstring.h"

int main()
{

	char sztemp[10];
	char sztemp2[10];
	sztemp[0] = 0xe7;
	sztemp[1] = 0x85;
	sztemp[2] = 0xa7;
	sztemp[3] = 'a';
	sztemp[4] = 0xe7;
	sztemp[5] = 0x85;
	sztemp[6] = 0xa7;
	sztemp[7] = 0;

	strutf8togbk(sztemp, sztemp2);
	//strcpy(sztemp, "’’∆¨");
	printf(sztemp2);
	getchar();
	httpserver_init();
	httpserver_main();
	getchar();
}