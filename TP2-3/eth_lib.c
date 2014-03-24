 /////////////////////////////////////////////////////////////
// Nom du fichier : eth_lib.c
// Auteur : D. Faucher (sur source original Pascal Anelli)
// Modif : 25/03/99
//
// Corps des fonctions constituants une bibliotheque de 
// manipulation de trames ethernet
//////////////////////////////////////////////////////////////

#include "eth_lib.h"

////////////////////////////////////////
void p_error(char *s)
{
	fprintf(stderr,"%s: %s ", program_name, s);
	if (errno !=0) {
		fprintf(stderr,"(%s)", strerror(errno));
		errno=0;
	}
	fprintf(stderr,"\n");
} /* perror */

//////////////////////////////////////////////:
/* This version of checksum is a slight modification of the naive implementation
    presented in TCP/IP Illustrated vol2 
*/
unsigned short checksum( char * buf, int len, unsigned short init)
{
	unsigned long int sum;	/* 32 bits */

	sum = init;
	while (len>1) {
		//sum += *((unsigned short *) buf)++;
		sum += *( buf)++;
		if (sum & 0x80000000) /* high order bit set */
			sum = (sum & 0xFFFF) + (sum>>16);
		len -=2;
	}
	if (len) /* odd byte count */
		sum += (unsigned short) * (unsigned char *) buf;
	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);
	return(sum);
}


/* function eth_addr
   	converts an ASCII Ethernet address into a 6 bytes format
	in network byte order ready to go into an ethernet frame.
	The input format is 00:00:C0:F3:57:6B for example.
	The return value is a pointer to a static variable of the
	function.
*/
char *eth_addr(char * a)
{
	static char result[6]; /* the resulting ethernet address */
	char * p; /* pointer for string analysis */
	int i,j;
	char  adr[18],conv[10], * ad = adr;

	if (strlen(a) != 17) { /* address too short */
		p_error("eth_addr: bad address");
		return(NULL);
	}
	strncpy(ad, a, 17);ad[17]=':';
	for (i=0, p=ad; i<6; i++) {
		p = strchr(ad,':'); p++;
		if ((ad + 3) != p) {
			p_error("eth_addr: bad octet in address");
			return(NULL);
		}
		strcpy(conv,"0x"); 
		strncat(conv,ad,2); sscanf(conv,"%x",&j);
		result[i] = j;
		ad = p;
	}
	return(result);
} /* eth_addr */

char *eth_type(char *a)
{
	static char result[2]; /* the resulting ethernet type */
	char conv[10];
	int j;

	if (strlen(a) != 4) { /* address too short */
		p_error("eth_type: bad number");
		return(NULL);
	}
	strcpy(conv,"0x");strncat(conv,a,2);sscanf(conv,"%x",&j);
 	result[0]=j;
	strcpy(conv,"0x");strncat(conv,a+2,2);sscanf(conv,"%x",&j);
	result[1]=j;
	return(result);
}


void restore_interface()
{
	int fd; /* file descriptor for interface access */

	fd = socket(PF_INET, SOCK_PACKET, htons(ETH_P_ALL));
	if (fd < 0) {
		p_error("socket - could not restore interface to normal state");
		return;
	}
	/*
	if (ioctl(fd, SIOCSIFFLAGS, &ifr_orig) < 0) {
		p_error("ioctl - could not restore interface to normal state");
		return;
	}*/
} /* restore_interface */

int open_interface(char *device)
{
	int fd; /* file descripor for interface access */
	struct ifreq ifr; /* parameter for ioctl */

	fd =  socket(PF_INET, SOCK_PACKET, htons(ETH_P_ALL));
	if (fd < 0) {
		p_error("socket - could not open interface");
		return(-1);
	}
	if (strncmp(device, "eth", 3) != 0) {
		p_error("only eth devices supported");
		return(-1);
	}
	strcpy(ifr.ifr_name, device); /* the device to be used */
	if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) {
		p_error("could not get interface flags");
		return(-1);
	}
	ifr_orig = ifr;
	atexit(restore_interface);    /* for cleaning at program end */
	ifr.ifr_flags |= IFF_PROMISC; /* set promiscuous mode */
	
	if (ioctl(fd, SIOCSIFFLAGS, &ifr) < 0) {
		p_error("ioctl - could set promiscuous mode");
		return(-1);
	}
	strcpy(snoop_device, device);
	return(fd);
} /* open_interface */

void dump_hex(char * buf, int nb)
{
	int i=0, n=0;

	for ( ; nb>0; nb--) { 
		printf("%02hx ", (unsigned char) buf[i]); i++; n++;
		if (n == 16) {
			printf("\n"); n = 0;
		}
	}
	if (n != 0) {
		printf("\n");
	}
} /* dump_hex */


/* function read_eth_frame
	This function reads an ethernet frame of type type.
	For example, if type == htons(0x0806), ARP frames are read
	                              0x0800 , IP frames are read
	The number of bytes read is returned in n
*/
struct eth_frame * read_eth_frame(int fd, unsigned short int type, int * n)
{
	int nb; /* number of bytes read */
	struct sockaddr from; /* address from which the packet is coming */
	int from_len; /* length of the address */
	static struct eth_frame buf; /* buffer for data transfer */

	/*do {*/
		nb =  recvfrom( fd, &buf, sizeof(buf), 0, &from, &from_len);
	/*} while (memcmp(buf.type, &type, 2) != 0);*/
	*n = nb;
	return(&buf);
} /* read_eth_frame */



/* function write_eth_frame
	This function writes an ethernet frame
*/
int write_eth_frame(int fd, struct eth_frame *frame, int len)
{
	int nb;
	struct sockaddr to; /* address to send to */
	int to_len = sizeof(to); /* length of the address */

	strcpy(to.sa_data, snoop_device);
	nb = sendto(fd, frame, len, 0, &to, to_len); 
	return nb;
} /* write_eth_frame */



char * get_eth_addr(int fd)
{
	struct ifreq ifr;
	static char result[18];
	unsigned char *p;

	strcpy(ifr.ifr_name, snoop_device); /* the device to be used */
	if (ioctl(fd,SIOCGIFHWADDR, &ifr) <0 ) {
		p_error("get_eth_addr - error");
	}
	p = (unsigned char *) ifr.ifr_hwaddr.sa_data;
	sprintf(result,"%02x:%02x:%02x:%02x:%02x:%02x",
		p[0],p[1],p[2],p[3],p[4],p[5]);
	result[17]='\0';
	return(result);
} /* get_eth_addr */


char * char_to_charhexa(unsigned char* data,int l) {
	static char resultat[3000];
	int n;
	char ch[3];
	
	resultat[0] =  '\0';
	for (n=0; n<l; n++) {	
		sprintf(ch,"%02hx",(unsigned char)data[n]);
		strcat(resultat,ch);	
	}
	return resultat;
}

// char_to_charhexa
// retourne une representation hexadecimale sous forme de chaine
// terminee par \0 
   
int charhexa_to_char(char* chaine, unsigned char * resultat) {
	int i,j,n, l;
	char ch[5];
	
	i = 0;
	l = strlen(chaine);
	if(l > 3000) // patch contre l'overflow
		l = 3000;
	for (n=0; n<l; n=n+2) {	
		strcpy(ch,"0x");
		strncat(ch,&chaine[n],2);
		sscanf(ch,"%x",&j);
 		resultat[i]=j; i++;
	}
	return i;
}

int make_ping_request(char* eth_addr_dest, char* eth_addr_send
, char* type_frame, char* data, struct eth_frame* frame)
{
	unsigned short int len;
	
	memcpy(frame->adr_dest, eth_addr(eth_addr_dest), 6);
	memcpy(frame->adr_send, eth_addr(eth_addr_send), 6);
	memcpy(frame->type, eth_type(type_frame), 2);
	memcpy(frame->data, data, strlen(data));
	
	return(6+6+2+strlen(data));
	
} /* make_ping_request */

void dump_eth_header(struct eth_frame * f) {
	unsigned char* d = f->adr_dest;
	unsigned char* s = f->adr_send;
	fprintf(stdout,"-----------------------\n");
	fprintf(stdout,"ethernet destination =  %02hx:%02hx:%02hx:%02hx:%02hx:%02hx\n",
		d[0],d[1],d[2],d[3],d[4],d[5]);
	fprintf(stdout,"ethernet source      =  %02hx:%02hx:%02hx:%02hx:%02hx:%02hx\n",
		s[0],s[1],s[2],s[3],s[4],s[5]);
	fprintf(stdout,"type=%02hx%02hx\n", (unsigned char)f->type[0],(unsigned char)f->type[1]);
}

void dump_IP_header(struct ip4_frame * pIP) {

	const BITSDROITE = 0x0f;
	const BITSGAUCHE = 0xf0;
	unsigned char* adr;
	unsigned char* tabLength; 
	unsigned ihl = (pIP->vers_hlen & BITSDROITE) * 4;
	unsigned version = (pIP->vers_hlen & BITSGAUCHE) >> 4;
	
	tabLength = (unsigned char*)(&(pIP->length));
	
	fprintf(stdout,"Protocol IPv%d\n",version);
	fprintf(stdout,"IHL (mots/octets): %d/%d\n",ihl/4,ihl);
	
	fprintf(stdout,"ToS : %02hx\n",pIP->tos);
	fprintf(stdout,"Total length : %02hx:%02hx\n",tabLength[1],tabLength[0]);
	fprintf(stdout,"Identification : %02hx\n",pIP->ident);
 
	fprintf(stdout,"Flags/offset : %02hx\n",pIP->flag_offset);
	fprintf(stdout,"Time to live : %hu\n",(unsigned char)pIP->ttl);
	fprintf(stdout,"Protocol : %02hx\n",pIP->protocol);
	fprintf(stdout,"Checksum : %02hx\n",pIP->checksum);
	adr = (char*)&(pIP->addr_dest);
	fprintf(stdout,"Dest IP : %d.%d.%d.%d\n",adr[0],adr[1],adr[2],adr[3]);
	adr = (char*)&(pIP->addr_send);
	fprintf(stdout,"Send IP : %d.%d.%d.%d\n",adr[0],adr[1],adr[2],adr[3]);	
}

void dump_ARP_header(struct arp_frame * pARP) {
	unsigned char* adr;

	fprintf (stdout, "harware : %02hx,%02hx\n", pARP->hard_type[0], pARP->hard_type[1]);
	fprintf (stdout, "proto : %02hx,%02hx\n", pARP->prot_type[0], pARP->prot_type[1]);
	fprintf (stdout, "hlen : %d\n", pARP->hlen);
	fprintf (stdout, "plen : %d\n", pARP->plen);
	fprintf (stdout, "op : %02hx,%02hx\n", pARP->op[0], pARP->op[1]);
}

void dump_UDP_header(struct udp_frame * pUDP) {

	fprintf(stdout,"Message UDP");
	fprintf(stdout,"Port source : %u\n",pUDP->port_src);
	fprintf(stdout,"Port dest  : %d\n",pUDP->port_dst);
	fprintf(stdout,"Checksum : %u\n",pUDP->checksum);
	fprintf(stdout,"Length : %d\n",pUDP->longueur);
}

void analyse_trame(struct protocol* liste_protocol, struct eth_frame * frame) {
	int p;
	static int comptage[MAX_PROTO] = {0,0,0};
	
	for (p=0; p<MAX_PROTO; p++) {
		if ( memcmp(frame->type,&liste_protocol[p].type,2) == 0 ) {
			comptage[p]++; 
		}
		fprintf(stdout,"Trames %s : %d\n",liste_protocol[p].util,
			comptage[p]);
	}
		
}

	
/////////////////////////////////////////////////////	
// lits les bit compris entre les positions p1 et p2
// ces positions sont exprimees selon les indices des bits en partant 
// de 0 a droite : exemple,
//	x=10111011 p1=5, p2=2 renvoi 10(1110)11 soit 00001110
//
 
unsigned char lire_bits(unsigned char x, int p1, int p2) {
	unsigned char mx;
	// premiere etape : decaler les bits lus a droite
	mx = x >> (p2);
	// deuxieme etape : remplir avec des zeros
	return  (~(~0 << (p1-p2+1)))  & mx;
};

////////////////////////////////////////////////////////////
// Lecture sur le flot d'entree d'un nombre donne en parametre
// d'octets. Le resultat est retourne par le pointeur buf qui
// doit pointer sur une structure de donnee suffisante pour 
// recevoir les octets lus.

void get_buf (unsigned char* buf, int nb) {
	int top = nb;
	while (nb > 0) {
		fscanf(stdin,"%02hx",&buf[top-nb]);
		nb--;
	}
}



