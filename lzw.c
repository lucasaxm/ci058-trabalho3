#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DICTAMMAX 60000
#define STRTAMMAX 11


char **abre_dicionario (char **, unsigned short int, FILE *);
char *insere_no_dicionario (int);
FILE *abre_arquivo(const char *, const char *);
char *ngets(char *, int, FILE *);
void LimpaBuffer ();

int main(int argc, char const *argv[])
{
	if (argc!=3)
	{
		printf 	("Numero de argumentos invalido.\n\n"
				 "SINTAXE:\n"
				 "\t./lzw opcao arquivo\n"
				 "OPCOES:\n"
				 "\t-c\tCompactar arquivo\n"
				 "\t-d\tDescompactar arquivo\n");
		return 0;
	}
	int i,t;
	char dicionario[DICTAMMAX][STRTAMMAX];	//vetor com o dicionario

	for (t = 0; (argv[2][t]!='.') && (argv[2][t]!='\0') ; t++);	//descobre tamanho do nome do arquivo sem extensao
	char arq_semext[t+1]; // "nome_do_arquivo_sem_extensao"+"\0"
	char dicname[t+5]; // "nome_do_arquivo_sem_extensao"+".dic"+"\0"
	for (int i = 0; i < t; i++)
	{
		arq_semext[i]=argv[2][i];
		dicname[i]=argv[2][i];
	}
	arq_semext[i]='\0';
	dicname[i]='\0';
	char pontodic[]=".dic";
	dicname = strcat(dicname,pontodic);

	FILE *dicfile = abre_arquivo(dicname,"a+");
	unsigned short int tamdic;
	if (!fscanf(dicfile, "%hd",&tamdic))
	{	// novo dicionario
		tamdic = 0;
	}
	else
	{	// dicionario já existe
		fgetc (dicfile);
		dicionario = abre_dicionario(dicionario,tamdic,dicfile);
	}
/*	printf("Chave\tValor\n");
	for (int i = 0; i < tamdic; ++i)
	{
		printf("%i\t", i,dicionario[i]);
	}*/
	return 0;
}

int conta_linhas(FILE *arq)
{
	int cont=0;
	while (!feof(arq))
		if (fgetc(arq)=='\n')
			cont++;
	return cont;
}

char **abre_dicionario (char **d, unsigned short int tam, FILE *dicfile)
{
	int i;
	for (!feof(dicfile))
	{
		if (!fscanf (dicfile, "%hd", &i)) // le indicie
		{
			printf("Erro ao ler tamanho do dicionario do arquivo\n");
			exit(1);
		}
		fgetc(dicfile); // le separador

		d[i] = ngets(d[i],10,dicfile);
	}
	return d;
}

char *insere_no_dicionario (int n)
{
	return d;
}

FILE *abre_arquivo(const char *path, const char *mode)
{ 
	FILE *fp; 
	fp = fopen(path,mode);// Abre o arquivo para leitura e escrita
	if (fp == NULL) // se o arquivo nao existir exibe a mensagem de erro.
	{
		printf("Houve um erro ao abrir o arquivo\n");
		exit(1);
	}
	return fp;
}

/*---------------------------------------------------------------------------*/
/*Descarta caracteres que estao no buffer de entrada ate que encontre um '\n'*/
/*---------------------------------------------------------------------------*/
void LimpaBuffer ()
{
	while (getchar()!='\n');
}
/*---------------------------------------------------------------------------*/
/*Criadas para ler strings						     */
/*---------------------------------------------------------------------------*/
char *ngets (char *str, int n, FILE *fp)
{
	str = fgets (str, n, stdin);
	if (str[strlen(str)-1]=='\n')
		str[strlen(str)-1]='\0';
	else
		LimpaBuffer();
	return str;
}


/*3
A_
_A
AS*/