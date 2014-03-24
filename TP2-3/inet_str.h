/////////////////////////////////////////////////////////////
// Nom du fichier : inet_str.h
// Auteur : D. Faucher
// Modif : 10/03/99
// Equipe pédagogique Réseaux UFR Sciences Nantes (Attiogbe; Hamma; Passard)
// Definition des structures reseaux courantes
// et des fonctions utiles pour leur interpretation
//////////////////////////////////////////////////////////////


#define MAX_PACKET_SIZE 1500

struct protocol {
	char util[50];
	unsigned short int type;
	};

struct eth_frame {
	char adr_dest[6];
	char adr_send[6];
	char type[2];
	char data[MAX_PACKET_SIZE - 6 - 6 - 2];
	};

struct arp_frame {
	char hard_type[2];
	char prot_type[2];
	char hlen;	
	char plen;
	char op[2];
	char data[1];
	};
	
struct ip4_frame {
	char vers_hlen;
	char tos;
	unsigned short length;
	unsigned short int ident;
	unsigned short int flag_offset;
	char ttl;
	char protocol;
	unsigned short int checksum;
	unsigned long int addr_send;
	unsigned long int addr_dest;
	char data[1];
	};

struct icmp4_echo_req_frame {
	char type;
	char code;
	unsigned short int checksum;
	unsigned short int identifier;
	unsigned short int seq_number;
	char data[1];
	};
struct udp_frame {
	unsigned short int port_src;
	unsigned short int port_dst;
	unsigned short int longueur;
	unsigned short int checksum;
	char data[1];
	};

