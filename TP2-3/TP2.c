/**
 * Auteur : Matthieu Riou - Bruce Olivia
 *
 * TP2 - But : Récupère la sortie du programme sniff et analyse les trames
 * afin de comptabiliser le nombre de triplets identiques ( @source ;@destination ; type )
 * 
 * Compilation:  gcc  tp2.c  eth_lib.c convert.c  -o  tp2.exe
 * Exécution:  ./sniff  |  ./tp2.exe
 */
 

#include "eth_lib.h"
#include <string.h>
#include "convert.h" //Nos méthodes de conversion

/**
 * Methode de lecture de latrame
 * Lit une succession d'hexa les transforme en char puis les stock dans la tram
 * @return trame la trame reconstituée
 **/
struct eth_frame lire_trame() 
{

 	// Déclaration des variables
	char ch[20]; 	//  chaine de caractère "Byte rsc : " issue du sniff
	int nb; 			// taille de la trame à lire
	static struct eth_frame trame; // trame à lire

	int i;			// indice de tour
	
	
	fscanf (stdin,"%s",ch); // On lit  Byte
	fscanf (stdin,"%s",ch); // On lit rcv
	fscanf (stdin,"%s",ch); // On lit :

	fscanf (stdin,"%d",&nb); // on récupère la taille de la trame
	
	// On lit l'adresse de destination et on la stock dans la trame
	for(i = 0; i < 6; i++)
	{
		fscanf(stdin, "%s", ch);		
		trame.adr_dest[i] = hexaToChar(ch[0], ch[1]);
	}
	
	// On lit l'adresse d'envoie et on la stock dans la trame
	for(i = 0; i < 6; i++)
	{
		fscanf(stdin, "%s", ch);
		trame.adr_send[i] = hexaToChar(ch[0], ch[1]);
	}
	
	// On lit le type de la trame et on le stock de la trame
	for(i = 0; i < 2; i++)
	{
		fscanf(stdin, "%s", ch);
		trame.type[i] = hexaToChar(ch[0], ch[1]);
	}
	
	// On lit les donnees en calculant la fin de la trame avec sa taille
	for(i = 0; i < nb -6-6-2; i++)
	{
		fscanf(stdin, "%s", ch);
		trame.data[i] = hexaToChar(ch[0], ch[1]);
	}


	// renvoie la trame lue
	return trame;
}

/**
 * Méthode qui va vérifier si ce destinataire a deja ete vu
 * @return -1 si
 * @return i si
 **/
int findTriplet(struct eth_frame trame, int MAXTAB, unsigned char TadrSrc[MAXTAB][6], unsigned char TadrDst[MAXTAB][6], unsigned char Ttype[MAXTAB][2], int nbTriplet)
{
	int fini = 0;
	int i = 0;

	while(!fini && i < nbTriplet)
	{
		if(memcmp(TadrSrc[i], trame.adr_send, 6) == 0) //Si c'est la même source
		{
			if(memcmp(TadrDst[i], trame.adr_dest, 6) == 0) //Et la même destination
			{
				if(memcmp(Ttype[i], trame.type, 2) == 0) //Et le même type
				{
					fini = 1;
				}
			}
		}

		i++;
	}

	if(fini)
		return i-1;
	else
		return -1;
}

/**
 * Méthode qui va regarder si il existe déjà l'adresse source dans le tableau d'adresse unique
 * Renvoie -1  si l'adresse source n'est pas déjà présente
 **/
int findUnique(int MAXTAB,unsigned char tUnique[MAXTAB][6], int nbUnique, unsigned char adr_trame[6]){
	int fini = 0;
	int i = 0;

	while(!fini && i < nbUnique)
	{
		if(memcmp(tUnique[i], adr_trame, 6) == 0)
		{
			fini = 1;
		}

		i++;
	}

	if(fini)
		return i-1;
	else
		return -1;
}


// Boucle principale
int main() 
{

	// Déclaration des variables

	struct eth_frame trame; // une trame lue
	int MAXTAB = 100; // taille des tableaux 
	unsigned char TadrSrc[MAXTAB][6]; // tableau des adresses sources
	unsigned char TadrDst[MAXTAB][6]; // tableau des adresses destinataires
	unsigned char Ttype[MAXTAB][2]; //tableau des types
	int Tcpt[MAXTAB]; //tableau des compteurs des triplets (@source; @destinataire; type)

	unsigned char TadrSrcUnique[MAXTAB][6]; // tableau des adresses sources uniques
	int nbUnique = 0;	// Nombre d'adresses sources


	int i;

	int nbTriplet = 0;

	char str_adr[18];
	char str_type[5];

	// Lecture de de 0 jusqu'à 100 trames
	for(i = 0; i < MAXTAB; i++)
	{
		trame = lire_trame();
		
		//Verifie si trame déjà présente dans les tableaux
		int find = findTriplet(trame, MAXTAB, TadrSrc, TadrDst, Ttype, nbTriplet);
		
		if(find == -1)
		{
			// La trame n'a pas déjà été traitéé, on la stocke dans les trois tableaux
			memcpy(TadrSrc[nbTriplet], trame.adr_send, 6);
			memcpy(TadrDst[nbTriplet], trame.adr_dest, 6);
			memcpy(Ttype[nbTriplet], trame.type, 2);
			Tcpt[nbTriplet] = 1;

			nbTriplet++; // On augmente le nombre de triplet
		}
		else{
			// La a déjà été rencontrée, on augmente son compteur
			Tcpt[find]++; 
		}
		
		// Verifie si l'adresse source nous a deja contactee
		find = findUnique(MAXTAB,TadrSrcUnique, nbUnique, trame.adr_send);

		if(find == -1)
		{
			// Si elle n'est pas présente on la stocke
			memcpy(TadrSrcUnique[nbUnique],trame.adr_send, 6);
			nbUnique++;
		}
			
	}
	
	
	/** Affichage **/
	
	// Noms des postes connectés
	printf("Liste des postes connectés aux réseaux :\n");

	for(i = 0; i < nbUnique; i++)
	{
		convertAdresse(TadrSrcUnique[i], str_adr);
		printf("\tPoste %d : %s\n", i+1, str_adr);
	}

	// Triplets des postes: adresse source, adresse destination, type, compteur
	printf("Liste des triplets :\n");

	for(i = 0; i < nbTriplet; i++)
	{
		convertAdresse(TadrSrc[i], str_adr);
		printf("\tAdresse source : %s\n", str_adr);

		convertAdresse(TadrDst[i], str_adr);
		printf("\tAdresse destination : %s\n", str_adr);

		convertType(Ttype[i], str_type);
		printf("\tType : %s\n", str_type);

		printf("\tCompteur : %d\n\n", Tcpt[i]);
	}

	return 0;

} 
