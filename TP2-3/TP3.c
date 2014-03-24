/**
 * Auteur : Matthieu Riou - Bruce Olivia
 *
 * TP3 - But : 
 */

#include "eth_lib.h"
#include <string.h>
#include "convert.h"


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
		
	char ligne_appel[2000]; //stock de la commande a executer

	//Envoie de la trame sur le reseau
	strcpy(ligne_appel, "./write_eth_frame ");
	strcat(ligne_appel, char_to_charhexa((unsigned char*) &trame, taille));
	system(ligne_appel);
	
	sleep(1);

} 


int main() 
{
	// On envoie en broadcast à toutes les personnes du réseau 
	char* dest ="ff:ff:ff:ff:ff:ff";
	
	int interface = open_interface("eth0");

	// On récupère l'adresse de la machine (eth0)
	char* src = get_eth_addr(interface);
	
	
	
	while(1){
	//Cree de la trame Bonjour et l'envoie
		envoie_trame(src, dest , "Bonjour de Olivia", 6 + 6 + 2 + 18);
		
	}

	return 0;
}


