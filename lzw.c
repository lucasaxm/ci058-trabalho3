#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DICTAMMAX 60000
#define STRTAMMAX 11

void descompacta (char **, unsigned short int *, FILE *, FILE *);
void compacta (char **, unsigned short int *, FILE *, FILE *);
void acha_nova_string_d (char **, unsigned short int *, FILE *, char *);
unsigned short int acha_nova_string_c (char **, unsigned short int *, FILE *, char *);
unsigned short int busca_dicionario (char **, unsigned short int, char *);
char *insere_no_dicionario (int);
FILE *abre_arquivo(const char *, const char *);
char *ngets(char *, int, FILE *);
void LimpaBuffer ();
char **GeraMatrizVazia (unsigned short int, unsigned short int);

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
	char **dicionario = GeraMatrizVazia(DICTAMMAX,STRTAMMAX);	//vetor com o dicionario
	for (t = 0; (argv[2][t]!='.') && (argv[2][t]!='\0') ; t++);	//descobre tamanho do nome do arquivo sem extensao
	char arq_semext[t+1]; // "nome_do_arquivo_sem_extensao"+"\0"
	for (i = 0; i < t; i++)
		arq_semext[i]=argv[2][i];
	arq_semext[i]='\0';
	unsigned short int tamdic;
	switch (argv[1][1])
	{
		case 'c':
		{
			FILE *infile = abre_arquivo(argv[2],"r");
			char outname[t+5];
			strcpy(outname,arq_semext);
			char pontolzw[]=".lzw";
			strcat(outname,pontolzw);
			FILE *outfile = abre_arquivo(outname,"wb");
			compacta (dicionario,&tamdic,infile,outfile);
			fclose (infile);
			fclose (outfile);
			break;
		}
		case 'd':
		{
			FILE *infile = abre_arquivo(argv[2],"rb");
			char outname[t+5];
			strcpy(outname,arq_semext);
			char pontotxt[]=".txt";
			strcat(outname,pontotxt);
			FILE *outfile = abre_arquivo(outname,"w");
			descompacta (dicionario,&tamdic,infile,outfile);
			fclose (infile);
			fclose (outfile);
			break;
		}
		default:
		{
			fputs ("Argumento Invalido.\n",stderr);
			return 1;	//sai do programa caso haja parametro invalido.
			break;
		}
	}
	printf("tamdic=%hd\nChave\tValor\n",tamdic);
	for (i = 0; i < tamdic; i++)
	{
		printf("%hd\t", i+128);
		puts(dicionario[i]);
	}
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

void descompacta (char **dicionario, unsigned short int *tamdic, FILE *in, FILE *out)
{
	char str[STRTAMMAX];
	while (!feof(in))
	{
		acha_nova_string_d (dicionario, tamdic, in, str);
		strcpy(dicionario[(*tamdic)++],str);
		str[strlen(str)-1]=0;
		if (!feof(in))
		{
			fputs(str,out);
			fseek (in,-sizeof(unsigned short int),SEEK_CUR);
		}
	}
	(*tamdic)--;
}

void acha_nova_string_d (char **dicionario, unsigned short int *tamdic, FILE *in, char *str)
{
	int i, achei=0;
	unsigned short int code, temp;
	for (i = 0; i < STRTAMMAX; i++)
		str[i]=0;
	i=0;
	while ( (!achei) && (i<11) )
	{
		// fscanf (in,"%hd",&code);
		fread (&code, sizeof(unsigned short int), 1, in);
		if (code<128)
			str[i]= (char)code;
		else if ( (code-128) < (*tamdic) )
			strcat(str,dicionario[code-128]);
		if (i!=0)
			achei=1;
		i++;
	}
}

void compacta (char **dicionario, unsigned short int *tamdic, FILE *in, FILE *out)
{
	char str[STRTAMMAX];
	unsigned short int code;
	while (!feof(in))
	{
		code = acha_nova_string_c(dicionario, tamdic, in, str);
		strcpy(dicionario[(*tamdic)++],str);
		if (!feof(in))
		{
			// fprintf(out, "%hd", code);
			fwrite (&code, sizeof(unsigned short int), 1, out);
			fseek (in,-1,SEEK_CUR);
		}
	}
	(*tamdic)--;
}

unsigned short int acha_nova_string_c (char **dicionario, unsigned short int *tamdic, FILE *in, char *str)
{
	int i, achei=0;
	unsigned short int code, temp;
	for (i = 0; i < STRTAMMAX; i++)
		str[i]=0;
	i=0;
	while ( (!achei) && (i<11) )
	{
		str[i]=fgetc(in);
		if (strlen(str)==1)
		{
			code=str[0];
			i++;
		}
		else
		{
			temp = busca_dicionario(dicionario, *tamdic, str);
			if (temp!=0)
				code = temp;
			else
				achei=1;
			i++;
		}
	}
	return code;
}

unsigned short int busca_dicionario (char **dicionario, unsigned short int tamdic, char *str)
{
	unsigned short int i;
	for (i = 0; i < tamdic; i++)
		if (!strcmp(dicionario[i],str))
			return i+128;
	return 0;
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
	str = fgets (str, n, fp);
	if (str[strlen(str)-1]=='\n')
		str[strlen(str)-1]='\0';
/*	else
		LimpaBuffer();*/
	return str;
}

char **GeraMatrizVazia (unsigned short int linhas, unsigned short int colunas)
/* Gera matriz identidade 'linhas'x'colunas' */
{
	unsigned short int i,j;
	char **matriz = (char **) malloc (sizeof(char *)*linhas);
	for (i=0; i<linhas; i++)
	{
		matriz[i] = (char *) malloc (sizeof(char)*colunas);
		for (j=0; j<colunas; j++)
				matriz[i][j]=0;
	}
	return matriz;
}