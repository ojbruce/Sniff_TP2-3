#include "convert.h"

char lettreToChar(char lettre)
{
	switch(lettre)
	{
		case '0':
			return 0;
			break;
		case '1':
			return 1;
			break;
		case '2':
			return 2;
			break;
		case '3':
			return 3;
			break;
		case '4':
			return 4;
			break;
		case '5':
			return 5;
			break;
		case '6':
			return 6;
			break;
		case '7':
			return 7;
			break;
		case '8':
			return 8;
			break;
		case '9':
			return 9;
			break;
		case 'a':
			return 10;
			break;
		case 'b':
			return 11;
			break;
		case 'c':
			return 12;
			break;
		case 'd':
			return 13;
			break;
		case 'e':
			return 14;
			break;
		case 'f':
			return 15;
			break;
	}
}

char charToLettre(char valeur)
{
	switch(valeur)
	{
		case 0:
			return '0';
			break;
		case 1:
			return '1';
			break;
		case 2:
			return '2';
			break;
		case 3:
			return '3';
			break;
		case 4:
			return '4';
			break;
		case 5:
			return '5';
			break;
		case 6:
			return '6';
			break;
		case 7:
			return '7';
			break;
		case 8:
			return '8';
			break;
		case 9:
			return '9';
			break;
		case 10:
			return 'a';
			break;
		case 11:
			return 'b';
			break;
		case 12:
			return 'c';
		case 13:
			return 'd';
			break;
		case 14:
			return 'e';
			break;
		case 15:
			return 'f';
			break;
	}
}

char* charToHexa(char valeur)
{
	char dizaine = ((unsigned char)valeur) / 16;
	char unite = ((unsigned char)valeur) % 16;

	char* hexa = malloc(2*sizeof(char));
	hexa[0] = charToLettre(dizaine);
	hexa[1] = charToLettre(unite);

	return hexa;
}

char hexaToChar(char hexa1, char hexa2)
{
	hexa1 = lettreToChar(hexa1);
	hexa2 = lettreToChar(hexa2);

	return hexa1*16 + hexa2;
}

void convertAdresse(char* adr, char* res)
{
	sprintf(res, "%s:%s:%s:%s:%s:%s", charToHexa(adr[0]), charToHexa(adr[1]), charToHexa(adr[2]), charToHexa(adr[3]), charToHexa(adr[4]), charToHexa(adr[5]));
}

void convertType(char* type, char* res)
{
	sprintf(res, "%s%s", charToHexa(type[0]), charToHexa(type[1]));
}
