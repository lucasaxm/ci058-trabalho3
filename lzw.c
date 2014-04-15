#include <stdio.h>
#include <stdlib.h>
#include "arvBin/arvBin.h"

void CriaDicionario (int);

int main(int argc, char const *argv[])
{
	Apontador dicionario;
	arv_inicializa(&dicionario);
	printf("ASCII:\n");
	printf("Chave\tValor\n");
	for (int i = 0; i < 127; ++i)
	{
		printf("%i\t%c\n", i,dicionario[i]);
	}
	return 0;
}

void CriaDicionario (Apontador dicionario)
{
	int i;
	Registro r;
	for (int i = 0; i < 127; i++)
	{
		r.Chave = i;
		*(r.Valor) = (char) i;
		arv_insere
	}
	return d;
}