#include <stdio.h>
#include <stdlib.h>
#include "arvBin/arvBin.h"

char *CriaDicionario (int);

int main(int argc, char const *argv[])
{
	char *dicionario = CriaDicionario(255);
	printf("ASCII:\n");
	printf("Chave\tValor\n");
	for (int i = 0; i < 127; ++i)
	{
		printf("%i\t%c\n", i,dicionario[i]);
	}
	return 0;
}

char *CriaDicionario (int tam)
{
	int i;
	char *d = (char *) malloc (sizeof(char)*tam);
	for (int i = 0; i < 127; i++)
		d[i]=(char) i;
	return d;
}