#include "arvBin.h"
/*---------------------------------------------------------------------------*/
/*                  Cria Arquivo                                             */
/*---------------------------------------------------------------------------*/
FILE *abre_arquivo()
{ 
        FILE *fp; 
        
        fp = fopen("log.txt","w+");// Abre o arquivo para leitura e escrita
                
        if (fp == NULL) // se o arquivo nao existir exibe a mensagem de erro.
        {
		fprintf(fp,"Houve um erro ao abrir o arquivo\n");
                  printf("Houve um erro ao abrir o arquivo\n");
                exit(1);
        }             
        return fp;
}

/*---------------------------------------------------------------------------*/
/* Cria e aloca um novo nodo                                                 */
/*---------------------------------------------------------------------------*/
Apontador nodo_cria( Registro r, Apontador esq, Apontador dir, int cont ){
Apontador p;

	p = malloc(sizeof(*p));
	p->Reg = r;
	p->Esq = esq;
	p->Dir=dir;
	p->Cont = cont;
	return p;
}

/*---------------------------------------------------------------------------*/
/* Libera a memoria alocada para um nodo                                     */
/*---------------------------------------------------------------------------*/
void nodo_destroi( Apontador p ){

	free(p);
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

/*---------------------------------------------------------------------------*/
/*Atualiza a variavel de balanceamento do registrador que esta dentro do nodo*/
/*---------------------------------------------------------------------------*/
Apontador atualizaBal( Apontador p){

	if (p == NULL)
		return p;

	p->Esq = atualizaBal( p->Esq);
	p->Dir = atualizaBal( p->Dir);
	p->Reg.bal = altura(p->Esq)-altura(p->Dir); 

}	

/*___________________________________________________________________________*/
/*Calcula a altura da arvore binaria					     */
/*---------------------------------------------------------------------------*/
int altura (Apontador r) {
   if (r == NULL) 
      return -1; // altura de árvore vazia é -1
   else {
      int he = altura (r->Esq);
      int hd = altura (r->Dir);
      if (he < hd) return hd + 1;
      else return he + 1;
   }
}
/*---------------------------------------------------------------------------*/
/* Atualiza o contador dos nodos da arvore binaria                           */
/*---------------------------------------------------------------------------*/
Apontador atualizaCont(Apontador p){
Apontador r;
	if(p==nodoNull)
	   return p;
	p->Cont=1;
	r=atualizaCont(p->Esq);
	if(r!=nodoNull)
	   p->Cont=(p->Cont)+(r->Cont);
	r=atualizaCont(p->Dir);
	if(r!=nodoNull)
	   p->Cont=(p->Cont)+(r->Cont);	
	return p;
}

/*---------------------------------------------------------------------------*/
/* Cria o nodoNull e a arvore vazia                                          */
/*---------------------------------------------------------------------------*/
void arv_inicializa(Apontador *raiz){
Registro r;
	r.Chave = 0;	
	nodoNull = nodo_cria(r,NULL,NULL,0);
	*raiz = nodoNull;
	return;
} 
    
/*---------------------------------------------------------------------------*/
/* Imprime a arvore com visita pre-ordem                                     */
/*---------------------------------------------------------------------------*/
void arv_imprime(FILE *fp,Apontador p){ 
		
	if ( p == nodoNull){
		printf("()");
		fprintf(fp,"()");
		return;
	}
	printf("(");
	fprintf(fp,"(");
	if (p==NULL){
		printf(")");
		fprintf(fp,")");
		return;	
	}
	printf ( "%ld[%d]{%d}", p->Reg.Chave, p->Cont, p->Reg.bal);
	fprintf (fp,"%ld[%d]{%d}", p->Reg.Chave, p->Cont, p->Reg.bal);
	arv_imprime(fp,p->Esq);
	arv_imprime(fp,p->Dir);
	printf(")");
	fprintf(fp,")");
}
  
/*---------------------------------------------------------------------------*/
/* Busca um nodo na arvore.                                                  */ 
/* Retorna um ponteiro para o nodo ou nodoNull se nao encontrar              */
/*---------------------------------------------------------------------------*/

Apontador arv_busca (TipoChave k, Apontador p){
	if(p==NULL) return NULL;
	if (k == p->Reg.Chave)return p;
	if (k < p->Reg.Chave)
		return arv_busca(k,p->Esq);
	else return arv_busca(k,p->Dir);
} 

/*---------------------------------------------------------------------------*/
/* Faz a rotacao a Direita, acertando os contadores dos nodos                */ 
/*---------------------------------------------------------------------------*/
Apontador RotacaoDireita( Apontador p ){
  Apontador q;
	q = p->Esq;
	p->Esq = q -> Dir;
	q->Dir = p;
	q = atualizaCont(q);

return q;

}

/*---------------------------------------------------------------------------*/
/* Faz a rotacao a Esquerda, acertando os contadores dos nodos               */ 
/*---------------------------------------------------------------------------*/
Apontador RotacaoEsquerda( Apontador p ){
Apontador q;
	
	q = p->Dir;
	p->Dir = q-> Esq;
	q->Esq = p;
	q = atualizaCont(q);

return q;

}

/*---------------------------------------------------------------------------*/
/* Balanceia a arvore baseado no contador de nodos de forma que              */ 
/* a diferenca de altura das subarv. esq e direita de todos os nodos seja    */ 
/* no maximo 1.                                                              */ 
Apontador arv_balanceia( Apontador p ){

Apontador f,r,n;	
	if (p == nodoNull)
	return p;
	p = atualizaBal(p);
	p->Esq = arv_balanceia(p->Esq);
	p = atualizaBal(p);	
	p->Dir = arv_balanceia(p->Dir);
	p = atualizaBal(p);
	if (p->Reg.bal == 1 || p->Reg.bal == -1 || p->Reg.bal == 0)
	return p;
 	
	if (p->Reg.bal >= 2){ 
			f = p->Esq;
			if (f->Reg.bal == 1){ //esquerda esquerda
				r = RotacaoDireita(p);
				r = atualizaBal(r);
				p = atualizaBal(p);				
				return r;
			}
			else if (f->Reg.bal == -1){ //esquerda direita
				n = f->Dir;
				p->Esq = RotacaoEsquerda(f);
				r = RotacaoDireita(p);
				r = atualizaBal(r);
				p = atualizaBal(p);
				return r;
			}
			else if (f->Reg.bal == 0){ //rotacao para a direita e depois chama a funcao novamento para balancear o nodo que falta
				r = RotacaoDireita(p);
				r->Dir = arv_balanceia(r->Dir);
				r = atualizaBal(r);
				p = atualizaBal(p);
				return r;
			
			}
			else {
				r = arv_balanceia(f); //se nodo filho ainda estiver desbalanceado chama a funcao novamente
				r = atualizaBal(r);
				p = atualizaBal(p);
				
				return r;
			     }	
			
		}
		else if (p->Reg.bal <= -2) { //direita direita
			f = p->Dir;
			if ( f->Reg.bal == -1){
				r = RotacaoEsquerda(p);
				r = atualizaBal(r);
				p = atualizaBal(p);
				return r;
			}
			else if ( f->Reg.bal == 1){ //direita esquerda	
				n = f->Esq;
				p->Dir = RotacaoDireita(f);
				r = RotacaoEsquerda(p);
				r = atualizaBal(r);
				p = atualizaBal(p);
				return r;			

			}
			else if (f->Reg.bal == 0){ //rotacao para a esquerda e depois chama a funcao novamento para balancear o nodo que falta
				r = RotacaoEsquerda(p);
				r->Esq = RotacaoEsquerda(r->Esq);
				r = atualizaBal(r);
				p = atualizaBal(p);
				return r;
			}
				else {
				r = arv_balanceia(f); //se nodo filho ainda estiver desbalanceado chama a funcao novamente
				r = atualizaBal(r);
				p = atualizaBal(p);
				return r;
			        }
				
		}//dir-dir

	
}//FIM DA FUNCAO
Apontador auxBalanceiaArv( Apontador p){

            p = arv_balanceia(p);
            p = arv_balanceia(p);
            return p;
            
}

/*---------------------------------------------------------------------------*/
/* Insere um novo registro na arvore                                         */
/*---------------------------------------------------------------------------*/
Apontador arv_insere(Registro r, Apontador p){

	if (p == nodoNull) return nodo_cria(r, nodoNull, nodoNull, 1 );
		
	if (r.Chave < p->Reg.Chave)
		p->Esq =  arv_insere (r,p->Esq);
	
	else	
		p->Dir=  arv_insere(r,p->Dir);
		p->Reg.bal = altura(p->Esq)-altura(p->Dir); 
		p->Cont= p->Esq->Cont+ p->Dir->Cont+ 1;
	return p;

}


/* Remove o registro com chave k da arvore                                   */
/* Considere que a chave k pode ou nao existir na arvore.                    */
/* Caso nao exista, a funcao simplesmente nao altera a arvore.               */
/*---------------------------------------------------------------------------*/
Apontador JuntaEsqDir ( Apontador e, Apontador d){

	if ( d == nodoNull) return e;
	if ( e == nodoNull) return d;
 	d = particao(d,1);
	d->Esq = e;
	return d;
}


Apontador arv_remove (TipoChave k, Apontador p){
Apontador q;
	if ( p == nodoNull) return nodoNull;
	if ( k < p->Reg.Chave){ 
		p->Esq = arv_remove(k,p->Esq);
		p->Cont= p->Esq->Cont+ p->Dir->Cont+ 1;	
	}	
	else if ( k > p->Reg.Chave){ 
		p->Dir = arv_remove(k,p->Dir);
	if (p != nodoNull)
		p->Cont= p->Esq->Cont+ p->Dir->Cont+ 1;
			
	}
	if ( k == p->Reg.Chave) {
		q=p;	
		p = JuntaEsqDir(p->Esq, p->Dir);
		nodo_destroi(q);
	}
	p = atualizaBal(p);	
	return p;
	
}



/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Coloca o n-esimo nodo na raiz                                             */ 
/*---------------------------------------------------------------------------*/
Apontador particao( Apontador p, int n ){
int t;
	 int contador;
        if(n > p->Cont)
                return nodoNull;
        contador = p->Esq->Cont;
        if(n == contador + 1)
                return p;
        else if(n <= contador){
                p->Esq= particao(p->Esq, n);
                return RotacaoDireita(p);
             
        }
        else{
                p->Dir= particao (p->Dir, n-(contador- 1));
                return RotacaoEsquerda(p);
              
        }
        return p;
}


