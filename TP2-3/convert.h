#ifndef CONVERT_H
#define CONVERT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

char lettreToChar(char lettre);
char charToLettre(char valeur);
char* charToHexa(char valeur);
char hexaToChar(char hexa1, char hexa2);
char* convertAdresse(char* adr);

#endif //CONVERT_H
