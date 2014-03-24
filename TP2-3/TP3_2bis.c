/* Adresse MAC: 00:12:3f:3c:5e:6e */

#include "eth_lib.h"
#include <string.h>
#include "convert.h"

/**
 * Methode de lecture de latrame
 * Lit une succession d'hexa les transforme en char puis les stock dans la tram
 * @return trame la trame reconstituée
 **/
struct eth_frame lire_trame(int* taille) 
{

 	// déclaration des variables
	char ch[20]; //  chaine de caractère "Byte rsc : " issue du sniff
	int nb; // taille de la trame à lire
	static struct eth_frame trame; // trame à lire

	int i;
	
	
	fscanf (stdin,"%s",ch); //On lit  Byte
	fscanf (stdin,"%s",ch); //On lit rcv
	fscanf (stdin,"%s",ch); //On lit :

	fscanf (stdin,"%d",&nb); // on récupère la taille
	*taille = nb;
	
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

	return trame;
}


/*
 * Methode qui envoie une trame en specifiant adresse source et dest et un message
 * @param src		la source de la trame (nous)
 * @param dest		destinataire, celui à qui on envoie la trame
 * @param mess		le message à envoyer dans la trame
 * @param taille	la taille du message
 */
void envoie_trame(char* src, char* dest, char* mess, int taille){
	struct eth_frame trame;
	int i;
	
	for(i = 0; i < 6; i++)
	{
		trame.adr_send[i] = hexaToChar(src[3*i], src[3*i+1]);
		trame.adr_dest[i] = hexaToChar(dest[3*i], dest[3*i+1]);
	}
	
	trame.type[0] = hexaToChar('9', '0');
	trame.type[1] = hexaToChar('0', '0');	
	
	strcpy(trame.data,mess);
	
	//Envoie de la trame sur le reseau

	char ligne_appel[2000];

	strcpy(ligne_appel, "./write_eth_frame ");
	strcat(ligne_appel, char_to_charhexa((unsigned char*) &trame, taille));
	system(ligne_appel);

} 


/**
 * Méthode qui va regarder si il existe déjà l'adresse source dans le tableau d'adresse unique
 * Renvoie -1  si l'adresse source n'est pas déjà présente
 **/
int findMessage(int MAXTAB, char* tMessage[MAXTAB], int nbMessage, char* mess, int tailleMess){
	int i;
	for(i = 0; i < nbMessage; i++)
	{
		if(memcmp(tMessage[i], mess, tailleMess) == 0)
		{
			return i;
		}
	}

	return -1;
}

int main() 
{	

	// Déclaration des variables

	struct eth_frame trame; // une trame lue
	int MAXTAB = 100; // taille des tableaux
	char* Mess[MAXTAB]; // tableau des adresses sources
	char Source[MAXTAB][6];
	char Dest[MAXTAB][6];
	int nbMess = 0;	// Nombre d'adresses sources

	char type[2][2];
	type[0][0] = '9'; type[0][1] = '0'; type[1][0] = '0'; type[1][1] = '0';

	int i;
	int find;
	int tailleTrame;

	int interface = open_interface("eth0");
	// On récupère l'adresse de la machine (eth0)
	char* src = get_eth_addr(interface);

	for(i = 0; i < MAXTAB; i++)
	{
		trame = lire_trame(&tailleTrame);
		printf("%s%s - %s\n", charToHexa(trame.type[0]), charToHexa(trame.type[1]), trame.data);
		
		if(trame.type[0] == -112 && trame.type[1] == 0 && trame.adr_dest[0] == -1 && trame.adr_dest[1] == -1 && trame.adr_dest[2] == -1 && trame.adr_dest[3] == -1 && trame.adr_dest[4] == -1 && trame.adr_dest[5] == -1)
		{
			find = findMessage(MAXTAB,Mess, nbMess, trame.data, tailleTrame-6-6-2);

			if(find == -1)
			{
				Mess[nbMess] = (char*) malloc((tailleTrame-6-6-2) * sizeof(char));
				memcpy(Mess[nbMess],trame.data, tailleTrame-6-6-2);
				memcpy(Source[nbMess], trame.adr_send, 6);
				memcpy(Dest[nbMess], trame.adr_dest, 6);

				nbMess++;

				char dest[18];

				int nomlen= strlen(trame.data);
				char nom_emmet[nomlen];	//nom de l'emmeteur
	
				//On recupere le nom de la personne qui a envoye un bonjour
				strcpy(nom_emmet,&trame.data[11]);
	
				//On recupere la taille du nom
				nomlen= strlen(nom_emmet);
	
	
				//On construit le message
				char mess[100] = "Bonjour "; //taille : 8
				strcat(mess,nom_emmet);
				strcat(mess," bien reçu par Olivia et Matthieu."); //taille : 34
	
				printf("Message à envoyer: %s\n",mess);	
	
				//L'emmeteur devient le destinataire
				sprintf(dest, "%s:%s:%s:%s:%s:%s", charToHexa(trame.adr_send[0]), charToHexa(trame.adr_send[1]), charToHexa(trame.adr_send[2]), charToHexa(trame.adr_send[3]), charToHexa(trame.adr_send[4]), charToHexa(trame.adr_send[5]));
	
				//On précise la taille du message
				envoie_trame(src, dest , mess, nomlen +34+8);


				
				/*printf("\tAdresse source : %s:%s:%s:%s:%s:%s\n", charToHexa(Source[nbMess -1][0]), charToHexa(Source[nbMess -1][1]), charToHexa(Source[nbMess -1][2]), charToHexa(Source[nbMess -1][3]), charToHexa(Source[nbMess -1][4]), charToHexa(Source[nbMess -1][5]));

				printf("\tAdresse destination : %s:%s:%s:%s:%s:%s\n", charToHexa(Dest[nbMess-1][0]), charToHexa(Dest[nbMess-1][1]), charToHexa(Dest[nbMess-1][2]), charToHexa(Dest[nbMess-1][3]), charToHexa(Dest[nbMess-1][4]), charToHexa(Dest[nbMess-1][5]));

				printf("\tMessage : %s\n", Mess[nbMess-1]);*/

			}
		}	
	}

	return 0;
}


