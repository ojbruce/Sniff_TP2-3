#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_ether.h>  /* Protocol Type for Ethernet */
#include <stdlib.h>
#include <netinet/in.h>       /* structure de l'adresse INET */
#include <arpa/inet.h>        /* routines reseaux ntop*/
#include "inet_str.h"

char *program_name;
char snoop_device[30];
char eth_add_router[] = "08:00:20:0A:AC:96"; /* address of the default router */
char eth_add_local[18]; /* hw address of this machine */
char ip_add_local[18]; /* ip address of this machine */
extern int errno;

struct ifreq ifr_orig; /* for interface status storage */

char * get_ip_addr(int fd){
	struct ifreq ifr;
	static char result[18];
	unsigned char *p;
	int res;
		
	strcpy(ifr.ifr_name, snoop_device); /* the device to be used */
	if ((res=ioctl(fd, SIOCGIFADDR, &ifr)) <0 ) {
		p_error("get_ip_addr - error");
	}
	p = (unsigned char *) ifr.ifr_addr.sa_data; p+=2;
		
	sprintf(result,"%d.%d.%d.%d\n",
			p[0],p[1],p[2],p[3]);
	return(result);
} /* get_ip_addr */
		
		
int main(int argc, char **argv) {
		//char line [INET6_ADDRSTRLEN];
		char * cp;
		int nb;
		char * device = "eth0"; /* device to read by default */
		int fd ; /* descriptor to read from */
		struct eth_frame f, *g; /* ethernet frames */
		struct ip6_frame * ip6;
		struct ip4_frame * ip;
		unsigned int nbpacket = 0;
		
	if ((cp = strrchr(argv[0],'/')) != NULL)
		program_name = cp + 1;
	else
		program_name = argv[0];
	fd = open_interface(device);
	fprintf(stderr,"%s: hardware address: %s IP address: %s device: %s\n",
		program_name,get_eth_addr(fd), get_ip_addr(fd), device);
	strcpy(eth_add_local, get_eth_addr(fd)); /* local ethernet address */
	strcpy(ip_add_local, get_ip_addr(fd)); /* local ip address */
	while (1) {
		g=read_eth_frame(fd, 0x0008, &nb);
		ip = (struct ip4_frame *) g->data;
		if ((ip->protocol == 0x11) &&
		   ((((struct udp_frame *) ip->data)->port_src == 0xA100) ||
		   (((struct udp_frame *) ip->data)->port_dst == 0xA100)))  {
		 
//			 printf("\n---------------------------------------------------------------------\nPacket No: %i\n", nbpacket++);
		 	printf("Nombre octets recus      : \n%d\n", nb);
//		 	dump_hex((char *)g, 90); 
		 	dump_hex((char *)g, nb); 
		 	continue;
		}
		if ((ip->protocol == 0x01) &&
		        ((((struct icmp4_echo_req_frame*) ip->data)->type == 0X08) ||
		        	            (((struct icmp4_echo_req_frame*) ip->data)->type == 0X00))) {
//			printf("\n-icmp4------------------------------------------------------------------\nPacket No: %i\n", nbpacket++);
		        	            printf("Byte rcv      : \n%d\n", nb);
//		  	dump_hex((char *)g, 90);
		  	dump_hex((char *)g, nb);
		  	continue;
		}
		if ((ip->protocol ==IPPROTO_TCP)) {
		  
//		  printf("\n----------IPPROTO---------------------------------------------------------\nPacket No: %i\n", nbpacket++);
		  printf("Byte rcv      : \n%d\n", nb);
		  dump_hex((char *)g, nb);
		  continue;
		}
	} /*while */
} /* main */
		  
