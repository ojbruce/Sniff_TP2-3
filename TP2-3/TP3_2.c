#include "eth_lib.h"
#include <string.h>
#include "convert.h"

// lecture d'une trame
struct eth_frame lire_trame() 
{

 	// déclaration des variables
	char ch[20]; //  chaine de caractère "Byte rsc : " issue du sniff
	int nb; // taille de la trame à lire
	static struct eth_frame trame; // trame à lire

	// Lecture de la première ligne issue du sniffer "Byte rsc : xx"

	/****************************************
        A completer :                         
	On pourra utiliser la fonction fscanf(flux,type,var) pour
        lire les mots ou les entiers un par un.

        Exemple 1 : fscanf (stdin,"%s",ch); permet de lire un mot (de
        type string) et de l'enregistrer dans la variable ch

        Exemple 2 : fscanf (stdin,"%d",&nb); permet de lire un entier
        et de l'enregistrer dans la variable nb
	*****************************************/

	int i;
	
	
	fscanf (stdin,"%s",ch); //On lit  Byte
	fscanf (stdin,"%s",ch); //On lit rcv
	fscanf (stdin,"%s",ch); //On lit :

	fscanf (stdin,"%d",&nb); // on récupère la taille
	
	for(i = 0; i < 6; i++)
	{
		fscanf(stdin, "%s", ch);		
		trame.adr_dest[i] = hexaToChar(ch[0], ch[1]);
	}

	for(i = 0; i < 6; i++)
	{
		fscanf(stdin, "%s", ch);
		trame.adr_send[i] = hexaToChar(ch[0], ch[1]);
	}

	for(i = 0; i < 2; i++)
	{
		fscanf(stdin, "%s", ch);
		trame.type[i] = hexaToChar(ch[0], ch[1]);
	}

	for(i = 0; i < nb -6-6-2; i++)
	{
		fscanf(stdin, "%s", ch);
		trame.data[i] = hexaToChar(ch[0], ch[1]);
	}


	// lecture de la trame en hexa

	/****************************************
        A completer :                         
	On pourra utiliser la fonction get_buff de la librairie eth_lib.c
	*****************************************/


	// renvoie la trame lue

	return trame;
}

/**
 * Méthode qui va regarder si il existe déjà l'adresse source dans le tableau d'adresse unique
 * Renvoie -1  si l'adresse source n'est pas déjà présente
 **/
int findMessage(int MAXTAB, char* tMessage[MAXTAB], int nbMessage, char* mess){
	int i;
	for(i = 0; i < nbMessage; i++)
	{
		if(strcmp(tMessage[i], mess) == 0)
		{
			return i;
		}
	}

	return -1;
}


// Boucle principale
int main() 
{

	// Déclaration des variables

	struct eth_frame trame; // une trame lue
	int MAXTAB = 100; // taille des tableaux
	char* Mess[MAXTAB]; // tableau des adresses sources
	int nbMess = 0;	// Nombre d'adresses sources

	int i;
	int find;

	for(i = 0; i < MAXTAB; i++)
	{
		trame = lire_trame();

		if(charToHexa(trame.type[0]) == "90" && charToHexa(trame.type[1]) == "00")
		{
			find = findMessage(MAXTAB,Mess, nbMess, trame.data);

			if(find == -1)
			{
				strcpy(Mess[nbMess],trame.data);
				nbMess++;
			}
			else
			{
			
			}
		}
			
	}
	
	/*printf("Liste des postes connectés aux réseaux :\n");

	for(i = 0; i < nbUnique; i++)
	{
		printf("\tPoste %d : %s:%s:%s:%s:%s:%s\n", i+1,charToHexa(TadrSrcUnique[i][0]), charToHexa(TadrSrcUnique[i][1]), charToHexa(TadrSrcUnique[i][2]), charToHexa(TadrSrcUnique[i][3]), charToHexa(TadrSrcUnique[i][4]), charToHexa(TadrSrcUnique[i][5]));
	}


	printf("Liste des triplets :\n");

	for(i = 0; i < nbTriplet; i++)
	{
		printf("\tAdresse source : %s:%s:%s:%s:%s:%s\n", charToHexa(TadrSrc[i][0]), charToHexa(TadrSrc[i][1]), charToHexa(TadrSrc[i][2]), charToHexa(TadrSrc[i][3]), charToHexa(TadrSrc[i][4]), charToHexa(TadrSrc[i][5]));

		printf("\tAdresse destination : %s:%s:%s:%s:%s:%s\n", charToHexa(TadrDst[i][0]), charToHexa(TadrDst[i][1]), charToHexa(TadrDst[i][2]), charToHexa(TadrDst[i][3]), charToHexa(TadrDst[i][4]), charToHexa(TadrDst[i][5]));

		printf("\tType : %s%s\n", charToHexa(Ttype[i][0]), charToHexa(Ttype[i][1]));

		printf("\tCompteur : %d\n\n", Tcpt[i]);
	}*/

 	// traitement des trames et remplissage des tableaux

	/****************************************
        A completer :
	On pourra utiliser les fonctions
	 memcpy(char[] tab1,char[] tab2 ,int n) pour copier n entrées du tableau tab2
	 dans le tableau tab1
	
	 int memcmp(char[] tab1,char[] tab2,int n) pour comparer les n premières entrées
	 des tableaux tab1 et tab2. La fonction renvoie 0 si les entrées sont identiques
	 et 1 sinon
	
	 char* char_to_charhexa(unsigned char* data,int l) pour transformer une 
	 représentation hexadécimale de longueur l sous forme de chaine de caractères
	 A utiliser si vous voulez afficher les adresses enregistrées dans une eth_frame frame
	 en utilisant printf. Exemple :
	 printf("Adresse à afficher : %s\n", char_to_charhexa(frame.adr_dest,6))

	*****************************************/

	return 0;

} 
