/*---------------------------------------------------------------------------*/
/* Universidade Federal do Parana                                            */
/* CI057: Algoritmos e Estruturas de dados III, 2sem/2012                    */
/* Primeiro Trabalho                                                         */
/* Alunos:  Evelim Carla Ribeiro 	- GRR 20101622                       */
/*          Florence Pereira da Silva -   GRR 20101934                       */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include "arvBin.h"

int main(int argc, char *argv[]){
char opc[5000];	
TipoChave chave;
Registro r;
Apontador busca,raiz;

	arv_inicializa(&raiz);
	FILE *fp = abre_arquivo();
	ngets (opc, 4999, stdin);
	while (opc[0] != 'f'){	
	switch(opc[0]){
		case 'i': //insere chave
			r.Chave = atoi(&opc[2]);
			raiz = arv_insere(r,raiz);
		break;
		case 'p': //busca chave
			chave = atoi(&opc[2]);
			busca = arv_busca (chave,raiz);
			if (busca == nodoNull) printf("nao achou\n"); 
				else printf("Achou\n");
		break;		
		case 'r': //removendo chave
			chave = atoi(&opc[2]);
			raiz = arv_remove(chave,raiz);
		break;
		case 'b':
			//balanceamento
			raiz = auxBalanceiaArv(raiz);
		break;		
		case 'e': //imprimindo a arvore
			arv_imprime(fp,raiz);
			printf("\n");
		break;
		
		default : printf("Entrada Invalida \n");
		}
	ngets (opc, 4999, stdin);
	}
	fclose(fp);
}

  /* implementar o programa principal que le as instrucoes da entrada padrao */
  /* e escreve os resultados na saida padrao.                                */
  /* O programa principal nao deve fazer nenhuma alteracao na arvore, mas    */
  /* chamar as funcoes que voce implementou na sua biblioteca arvBin.c       */



