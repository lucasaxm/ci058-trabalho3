#ifndef _arvBin__
#define _arvBin_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
/* Objeto dentro de cada no da arvore                                        */ 
/*---------------------------------------------------------------------------*/
typedef int TipoChave;
typedef struct Registro {
  TipoChave Chave;
  char *valor;
  int bal;  
} Registro;

/*---------------------------------------------------------------------------*/
/* Estrutura de arvore binaria sem balanceamento                             */ 
/*---------------------------------------------------------------------------*/
typedef struct No *Apontador;
typedef struct No {
  Registro Reg;
  Apontador Esq, Dir;    /* filhos esquerdo e direito do no'*/
  int Cont;              /* quantidade de nodos na subarvore */
} No;

static Apontador nodoNull;

/*---------------------------------------------------------------------------*/
/*Funcao para arquivo							     */
/*---------------------------------------------------------------------------*/
FILE 	   *abre_arquivo    ();

/*---------------------------------------------------------------------------*/
/* Funcoes para trabalhar com os nos                                         */
/*---------------------------------------------------------------------------*/
Apontador  nodo_cria     (Registro r, Apontador esq, Apontador dir, int cont);
void       nodo_destroi  (Apontador p);


/*---------------------------------------------------------------------------*/
/*Funcoes que auxiliam as funcoes basicas da arvore binaria		     */
/*---------------------------------------------------------------------------*/
char 	   *ngets 	    (char *, int, FILE *);
void 	   LimpaBuffer 	    ();
Apontador  atualizaBal	    ( Apontador p);
int 	   altura	    (Apontador r);
Apontador  atualizaCont	    (Apontador p);

/*---------------------------------------------------------------------------*/
/* Funcoes basicas para utilizacao da arvore binaria de busca                */
/*---------------------------------------------------------------------------*/
void       arv_inicializa   (Apontador *raiz);
void       arv_imprime      (FILE *stream, Apontador raiz);
Apontador  arv_busca 	    (TipoChave k, Apontador p);
Apontador  RotacaoDireita   (Apontador p );
Apontador  RotacaoEsquerda  (Apontador p );
Apontador  arv_balanceia    (Apontador raiz);
Apontador  auxBalanceiaArv  (Apontador p);
Apontador  arv_insere       (Registro r, Apontador raiz);
Apontador  JuntaEsqDir      (Apontador e, Apontador d);
Apontador  arv_remove       (TipoChave k, Apontador p);
Apontador  particao 	    (Apontador raiz, int n);



#endif
