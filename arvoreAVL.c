#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define true 1
#define false 0
#define erro -1

typedef int TIPOCHAVE;
typedef int bool;

typedef struct aux{
    int chave;
    struct aux* esq; // menor ou igual que a chave
    struct aux* dir; // maior que a chave
    int bal; // balanceamento -> -1 esquerda, 0 empatado, 1 direita -> mais do que isso é desbalanceado e a árvore corrige
} NO, *PONT;

PONT criarNovoNo(TIPOCHAVE ch){
	PONT novoNo = (PONT)malloc(sizeof(NO));
	novoNo->esq = NULL;
	novoNo->dir = NULL;
	novoNo->chave = ch;
	novoNo->bal = 0;
	return novoNo;
}

int maximo(int a, int b){
	if (a>b) return a;
	return b;
}

int altura(PONT p){
	if (!p) return -1;
	else return 1 + maximo(altura(p->esq),altura(p->dir));
}

bool ehAVL(PONT p){
	int e,d;
	bool ok = true;
	if(p){
		ok = ehAVL(p->esq);
		if(ok) ok = ehAVL(p->dir);
		if(ok){
			e = altura(p->esq);
			d = altura(p->dir);
			if(e-d > 1 || e-d < -1) ok = false;  
			else ok = true;
		}
	}
	return(ok);
}

void exibirArvoreEmOrdem(PONT raiz){
	if (raiz == NULL) return;
	exibirArvoreEmOrdem(raiz->esq);
	printf("%i ",raiz->chave);
	exibirArvoreEmOrdem(raiz->dir);
}

void exibirArvore(PONT raiz, TIPOCHAVE chavePai){
	if (raiz == NULL) return;
	printf("(%i,%i) ",chavePai,raiz->chave);
	exibirArvore(raiz->esq,raiz->chave);
	exibirArvore(raiz->dir,raiz->chave);
}

PONT rotacaoL(PONT p){
    PONT u, v;
    u = p->esq;
    if(u->bal == -1){ // LL
        v = u->esq; // v é o menor dos 3
                    // u é a mediana dos 3 -> u vai pro meio
        p->esq = u->dir;
        u->dir = p;
        p->bal = 0;
        u->bal = 0;
        // o bal de v nao da pra saber
        return u;
    }
    if(u->bal == 1){  // LR
        v = u->dir; // v é a mediana dos 3 -> vai pro meio

        u->dir = v->esq;    // filhos de v vão pros filhos de u e p
        p->esq = v->dir;
        v->esq = u;
        v->dir = p;

        if(v->bal == -1)    // como u e p receberam filhos de v, verifica o balanceamento q o v tinha e acerta o balanceamento de u e p
            p->bal = 1;
        else
            p->bal = 0;

        if(v->bal == 1)
            u->bal = -1;
        else
            u->bal = 0;

        v->bal = 0;     
        return v;
    }
    if(u->bal == 0){  // L0 -> só acontece na exclusão -> não tem v, pq u eh balanceado (mas p não é)
        p->esq = u->dir;
        u->dir = p;
        u->bal = 1;
        return u;
    }
}

PONT rotacaoR(PONT p){
    PONT u, v;
    u = p->dir;
    if(u->bal == 1){ // RR
        v = u->dir; 
        p->dir = u->esq;
        u->esq = p;
        u->bal = 0;
        p->bal = 0;
        // o bal de v nao da pra saber
        return u;
    }
    if(u->bal == -1){  // RL
        v = u->esq;
        p->dir = v->esq;
        u->esq = v->dir;
        v->esq = p;
        v->dir = u;

        if(v->bal == 1)
            p->bal = -1;
        else
            p->bal = 0;

        if(v->bal == -1)
            u->bal = 1;
        else
            u->bal = 0;

        v->bal = 0;
        return v;
    }
    if(u->bal == 0){  // R0 -> só acontece na exclusão
        p->dir = u->esq;
        u->esq = p;
        u->bal = -1;
        return u;
    }
}

// os switches de inserção e de exclusão são diferentes em algumas coisinhas ent nao da pra reutilizar a mesma função
int SwitchEsqInsercao(PONT* p){
    int res = 0;
    PONT no = *p;
    switch (no->bal){
        case 1:
            no->bal = 0;
            res = 0;    // nao pode mais dar problema
            break;
        case 0:
            no->bal = -1;
            res = 1;    // ainda pode dar problema
            break;
        case -1:
            *p = rotacaoL(no);
            res = 0;    // corrigido então nao pode mais dar problema
            break;
    }
    return res;
}

int SwitchDirInsercao(PONT* p){
    int res = 0;
    PONT no = *p;
    switch (no->bal){
        case 1:
            *p = rotacaoR(no);
            res = 0;    //  nao pode mais dar problema
            break;
        case 0:
            no->bal = 1;
            res = 1;    // ainda pode dar problema
            break;
        case -1:
            no->bal = 0;
            res = 0;    // nao pode mais dar problema
            break;
    }
    return res;
}

// inserir deu certo!!!!! e eu consegui tirar da minha cabeça!!! 
// eh igual a inserção normal dai vc pensa nos switch cases dps que conseguir inserir, 
// e uma vez q algo retorna falso vc retorna tudo falso pq a arvore ja esta AVL. 
// Lembra q tem q passar ponteiro de ponteiro pq se nao algumas coisas nao modificam do jeito q deveriam
int inserir(PONT* p, int chave){
    PONT no = *p;
    if(no == NULL){
        *p = criarNovoNo(chave);
        return true;
    }
    
    if(no->chave == chave){     // ? nao pode inserir chave igual?
        return false;
    }
    else{
        if(chave <= no->chave){
            if(inserir(&(no->esq), chave)){
                int res = SwitchEsqInsercao(p);
                if(res)
                    return true;
                else
                    return false;
            }
        }
        if(chave > no->chave){ 
            if(inserir(&(no->dir), chave)){
                int res = SwitchDirInsercao(p);
                if(res)
                    return true;
                else
                    return false;
            }
        }
    }
}

PONT maiorAEsquerda(PONT p){
    PONT no = p->esq;
    while(no->dir != NULL){
        no = no->dir;
    }
    return no;
}

// os switches da exclusão são diferentes da inserção: na exclusão eu tirei de um lado então eh o outro lado q vai pesar; além disso na exclusão uma única rotação não garante q a árvore virou avl
// ja q eh assim, as rotações tb são invertidas, uma exclusão a esquerda q causa problema tem q corrigir à direita, pq eh pra lá q desbalanceou
int SwitchEsqExclusao(PONT* p){
    PONT no = *p;
    int res = 0;
    switch (no->bal)
    {
        case 1:
            *p = rotacaoR(no);
            if(no->bal != 0)
                res = 0;    // nao pode mais dar problema
            else
                res = 1;    // ainda pode dar problema
            break;
        case 0:
            no->bal = 1;    
            res = 0;    // nao pode mais dar problema
            break;
        case -1:
            no->bal = 0;
            res = 1;    // ainda pode dar problema    
            break;
    }
    return res;
}

int SwitchDirExclusao(PONT* p){
    PONT no = *p;
    int res = 0;
    switch (no->bal)
    {
        case 1:
            no->bal = 0;
            res = 1;        // ainda pode dar problema
            break;
        case 0:
            no->bal = -1;
            res = 0;        // nao pode mais dar problema
            break;
        case -1:                // vai ficar -2 -> corrige
            *p = rotacaoL(no);
            if(no->bal != 0)
                res = 0;        // nao pode mais dar problema 
            else
                res = 1;        // ainda pode dar problema
            break;
    }
    return res;
}

int excluir(PONT* p, int chave){       // eh bem importante q seja PONT* pq se não na hora de substituir o nó excluido pelo filho dele ngm vai apontar pra ele e ele vai sumir. o ponteiro pra ponteiro eh tipo quem aponta pra esse cara, que seria o campo filho do nó pai dele, por isso q dá certo (eh por isso q na recursão a gnt passa o endereço de no->esq ou dir, eh isso q significa essa linha).
    PONT no = *p;
    int res = -1;
    if(no == NULL){
        return erro; // nó q eu quero excluir não existe
    }

    if(chave == no->chave){ // achei a chave -> verifica quantos filhos ela tem
        PONT substituto = NULL;
        if(no->dir != NULL && no->esq != NULL){ // tem dois filhos
            substituto = maiorAEsquerda(no);
            no->chave = substituto->chave;  // aqui ele meio q ja excluiu a chave, ela nao existe mais, mas nenhum nó foi excluido ainda, por isso q a gnt vai descer até excluir essa cópia do substituto agr q ta la embaixo 
            chave = substituto->chave;
        }
        else{                                   // tem zero ou um filhos
            if(no->dir != NULL){
                substituto = no->dir;
            }
            else{
                substituto = no->esq;
            }
            *p = substituto;
            free(no);
            return true;
        }
    }
    if(chave <= no->chave){
        res = excluir(&(no->esq), chave);
        if(res == 1){
            return SwitchEsqExclusao(p);
        }
    }
    if(chave > no->chave){
        res = excluir(&(no->dir), chave);
        if(res == 1){
            return SwitchDirExclusao(p);
        }
    }

    return res;
}

void inicializar(PONT * raiz){
	*raiz = NULL;
}

void displayTree(PONT raiz) {
    if (raiz == NULL) {
        printf(" "); // Representa um nó vazio
        return;
    }
    // Exibe o valor do nó atual
    printf("%d(", raiz->chave);
    // Chama recursivamente para os filhos esquerdo e direito
    displayTree(raiz->esq);
    printf(",");
    displayTree(raiz->dir);
    printf(")");
}

int main(){
    PONT raiz;
    inicializar(&raiz);
    inserir(&raiz, 1);
    inserir(&raiz, 2);
    inserir(&raiz, 3);
    inserir(&raiz, 4);
    inserir(&raiz, 5);
    inserir(&raiz, 6);
    inserir(&raiz, 7);
    inserir(&raiz, 8);
    inserir(&raiz, 9);
    inserir(&raiz, 10);
    inserir(&raiz, 11);
    inserir(&raiz, 12);
    inserir(&raiz, 13);
    inserir(&raiz, 14);
    inserir(&raiz, 15);
    inserir(&raiz, 20);
    inserir(&raiz, 17);
    inserir(&raiz, 19);
    inserir(&raiz, 18);
    inserir(&raiz, 30);

    if(excluir(&raiz, 30) == -1){
        printf("não foi possível excluir esse valor \n");
    }

    if(excluir(&raiz, 7) == -1){
        printf("não foi possível excluir esse valor \n");
    }

    if(excluir(&raiz, 17) == -1){
        printf("não foi possível excluir esse valor \n");
    }

    if(excluir(&raiz, 18) == -1){
        printf("não foi possível excluir esse valor \n");
    }

    if(excluir(&raiz, 20) == -1){
        printf("não foi possível excluir esse valor \n");
    }

    inserir(&raiz, 30);
    inserir(&raiz, 22);
    inserir(&raiz, 01);
    inserir(&raiz, -1);
    inserir(&raiz, 24);
    inserir(&raiz, 57);

    if(ehAVL(raiz))
        printf("eh AVL!!! \n");

    // exibirArvore(raiz, 1);
    printf("\n");
    displayTree(raiz);

    return 0;
}