#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DICTAMMAX 60000
#define STRTAMMAX 1000

void descompacta (char **, unsigned short int *, FILE *, FILE *);
void compacta (char **, unsigned short int *, FILE *, FILE *);
unsigned short int busca_dicionario (char **, unsigned short int, char *);
void code2str (char **, unsigned short int , char *);
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
			char pontoout[]=".out";
			strcat(outname,pontoout);
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
/*	//Imprime dicionario na saida padrao

	printf("tamdic=%hd\nChave\tValor\n",tamdic);
	for (i = 0; i < tamdic; i++)
	{
		printf("%hd\t", i+128);
		puts(dicionario[i]);
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

void descompacta (char **dicionario, unsigned short int *tamdic, FILE *in, FILE *out)
{
	unsigned short int pW, cW;
	char scW[STRTAMMAX], P[STRTAMMAX], C;
	fread (&cW, sizeof(unsigned short int), 1, in);	// 2. cW <= primeira palavra código na sequência codificada (sempre é uma raiz);
	fputc ((char) cW, out);	// 3. Coloque a string(cW) na sequência de saída;
	pW = cW;	// 4. pW <= cW;
	fread (&cW, sizeof(unsigned short int), 1, in);	// 5. cW <= próxima palavra código da sequência codificada;
	code2str (dicionario, cW, scW);
	while (!feof(in))
	{
		if (busca_dicionario(dicionario,*tamdic,scW))	// 6. A string(cW) existe no dicionário ?
		{	// a. se sim,
			fputs (scW,out);	// i. coloque a string(cW) na sequência de saída;
			code2str (dicionario, pW,P);	// ii. P <= string(pW);
			C = scW[0];	// iii. C <= primeiro caracter da string(cW);
			P[strlen(P)+1]=0;
			P[strlen(P)]=C;
			if (*tamdic<DICTAMMAX)
				strcpy(dicionario[(*tamdic)++],P);	// iv. adicione a string P+C ao dicionário;
		}
		else	// b. se não,
		{
			code2str (dicionario, pW,P);	// i. P <= string(pW);
			C = P[0];	// ii. C <= primeiro caracter da string(pW);
			P[strlen(P)+1]=0;
			P[strlen(P)]=C;
			fputs (P,out);
			if (*tamdic<DICTAMMAX)
				strcpy(dicionario[(*tamdic)++],P);	// iii. coloque a string P+C na sequência de saída e adicione-a ao dicionário;
		}	// 7. Existem mais palavras código na sequência codificada ?
		pW = cW;	// 4. pW <= cW;
		fread (&cW, sizeof(unsigned short int), 1, in);	// 5. cW <= próxima palavra código da sequência codificada;
		code2str (dicionario, cW, scW);
	}
}

void code2str (char **dicionario, unsigned short int code, char *str)
{
	if (code<128)
	{
		str[0]=(char) code;
		str[1]=0;
	}
	else
		strcpy (str, dicionario[code-128]);
}

void compacta (char **dicionario, unsigned short int *tamdic, FILE *in, FILE *out)
{
	char I[STRTAMMAX];
	memset(I,0,STRTAMMAX);	// 1. No início o dicionário contém todas as raízes possíveis e I é vazio;
	char c;
	unsigned short int code, codeant, len=0;
	c=fgetc(in);	// 2. c <= próximo caractere da sequência de entrada;
	while(!feof(in))
	{
		I[len++]=c;		// I<=I+c
		if (code = busca_dicionario(dicionario, *tamdic, I))	// 3. A string I+c existe no dicionário?
		{	// se sim,
			codeant=code;	//i. I <= I+c;
		}
		else	// se não,
		{
			fwrite (&codeant, sizeof(unsigned short int), 1, out); // i. coloque a palavra código correspondente a I na sequência codificada;
			if (*tamdic<DICTAMMAX)
				strcpy(dicionario[(*tamdic)++],I);	// ii. adicione a string I+c ao dicionário;
			memset(I,0,STRTAMMAX);
			I[0]=c;	//iii. I <= c;
			codeant=c;
			len=1;
		}
		c=fgetc(in);
	} // 4. Existem mais caracteres na sequência de entrada ?
	code = busca_dicionario(dicionario, *tamdic, I);
	fwrite (&code, sizeof(unsigned short int), 1, out);	// ii. coloque a palavra código correspondente a I na sequência codificada;
}

unsigned short int busca_dicionario (char **dicionario, unsigned short int tamdic, char *str)
{
	unsigned short int i;
	if (strlen(str)==1)
		return str[0];
	else
	{
		for (i = 0; i < tamdic; i++)
			if (!strcmp(dicionario[i],str))
				return i+128;
	}
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