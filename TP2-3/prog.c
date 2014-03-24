#include <stdio.h>
#include "eth_lib.h"

int main ()
{

char a[20];
//char b[20];
int i;

i=0;
while (i<3)
{
	fscanf(stdin,"%s",a);
	printf("%s\n",a);
	i++;
}
printf("Nb d'octets:\n");
fscanf(stdin,"%s",a);
printf("%s\n",a);
i=0;

printf("Adresse du destinataire:\n");
while (i<6)
{
	fscanf(stdin,"%s",a);
	printf("%s\n",a);
	i++;
}
printf("\n\n");
i=0;

printf("Adresse source:\n");
while (i<6)
{
	fscanf(stdin,"%s",a);
	printf("%s\n",a);
	i++;
}
printf("\n\n");
i=0;

printf("Type de la trame:\n");
while (i<2)
{
	fscanf(stdin,"%s",a);
	printf("%s\n",a);
	i++;
}
printf("\n");
return 0;
}
