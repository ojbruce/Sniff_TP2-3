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
void convertAdresse(char* adr, char* res);
void convertType(char* type, char* res);

#endif //CONVERT_H
