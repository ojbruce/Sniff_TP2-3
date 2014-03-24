#include "eth_lib.h"
/*
	Fac. sciences Nantes
	TP Reseaux - 1998, ...
	Equipe pedagogique Reseaux  Attiogbe; Faucher;Hamma; Passard
	
	Cette application permet aux etudiants d'envoyer des trames sans etre root 
*/ 
main(int argc, char **argv)
{
	char ch[] = "Bonjour";
	char* chaine; 
	char *cp;
	int nb, nbsend;
	char * device = "eth0"; /* device to read by default */
	int fd ; /* descriptor to read from */
	struct eth_frame f; /* ethernet frames */
	char* chaine_frame;
	
	if ((cp = strrchr(argv[0],'/')) != NULL)
		program_name = cp + 1;
	else
		program_name = argv[0];
	if (argc == 2) {
		nbsend = charhexa_to_char(argv[1], (unsigned char *) &f);
		fd = open_interface(device);
		nb = write_eth_frame(fd, &f, nbsend);
	} 
} /* main */
