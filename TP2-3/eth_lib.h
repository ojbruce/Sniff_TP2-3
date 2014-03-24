/////////////////////////////////////////////////////////////
// Nom du fichier : eth_lib.h
// (version originale 1998....via P. Anelli)
// Modif : 25/03/99  D. Faucher
// Equipe pédagogique Réseaux UFR Sciences Nantes (Attiogbe; Hamma; Passard)
// Bibliotheque de fonctions de manipulation de trames ethernet
//////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

#include <sys/param.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <linux/if_ether.h> /* Ether protocol */
#include <net/if.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "inet_str.h"


#define MAX_PACKET_SIZE 1500
#define MAX_PROTO 3

char *program_name; // utilise pour afficher les erreurs
char snoop_device[30]; // par defaut = eth0 le port de la carte ethernet

struct ifreq ifr_orig; /* for interface status storage */


//////////////////////////////////////////////////
// Fonctions utilisables par tous
//////////////////////////////////////////////////

// affichage d'une erreur avec le message s et le code systeme
void p_error(char *s);

// conversion d'une chaine contenant une adresse xx:xx:xx:xx:xx:xx
// en une suite de 6 octets
char *eth_addr(char * a);

// conversion d'une chaine contenant un type en une suite de 2 
// octets (inversion de l'ordre des deux octets)
char *eth_type(char *a);

// conversion d'une suite d'octets de longueur "l" en une suite de caracteres
// representant la meme suite en notation hexadecimale
// la chaine retournee se termine par \0
char* char_to_charhexa(unsigned char* data, int l);

// fonction inverse de la precedente
// la fonction renvoi le nombre d'octets contenus dans resultat
// chaine est une suite de caractere se terminant par \0 
int charhexa_to_char(char * chaine, unsigned char * resultat);

// construction d'une trame ethernet a partir de ses composants
// donnes sous forme de chaines
// renvoi la longueur de la trame
int make_ping_request(char* eth_addr_dest, char* eth_addr_send,
		      char* type_frame, char* data, struct eth_frame* frame);

// affichage en clair du debut d'une trame ethernet (sauf data)
void dump_eth_header(struct eth_frame * f);

// affichage du contenu d'un datagramme IP
void dump_IP_header(struct ip4_frame * pIP); 

// comptage des trames en fonction de leur type
void analyse_trame(struct protocol* liste_protocol, struct eth_frame * frame);

// lits les bit compris entre les positions p1 et p2
// ces positions sont exprimees selon les indices des bits en partant 
// de 0 a droite : exemple,
//	x=10111011 p1=5, p2=2 renvoi 10(1110)11 soit 00001110
unsigned char lire_bits(unsigned char x, int p1, int p2);

// obtention de "nb" octets dans buf, a partir de l'entree std
// Le resultat est retourne par le pointeur buf qui
// doit pointer sur une structure de donnee suffisante pour 
// recevoir les octets lus.
void get_buf (unsigned char* buf, int nb);



/////////////////////////////////////////////////////////////
// Fonctions utilisables avec les droits du root uniquement
/////////////////////////////////////////////////////////////

// remise en place
void restore_interface();

// ouverture d'une socket sur un port donne
int open_interface(char *device);

// lecture des trames ethernet du type "type"
// n contient le nombre d'octets lus
// la trame est retournee via un pointeur sur une variable static
// propre a la fonction
struct eth_frame * read_eth_frame(int fd, unsigned short int type, int * n);

// ecriture d'une trame ethernet de longueur "len"
// la fonction retourne le nombre d'octets transmis
int write_eth_frame(int fd, struct eth_frame *frame, int len);
char * get_eth_addr(int fd);

 


