#include <stdio.h>
#include <stdlib.h>
#include "tp2.h"

Elemento *criaAluno(char* n, Chave mat)
{
    Elemento *aluno;
    aluno = (Elemento *)malloc(sizeof(Elemento));
    aluno->matricula = mat;
    aluno->nome = n;
    return aluno;
}

void apagaAluno(Aluno *a)
{
    free(a);
}

void imprimeAluno(Elemento *a, FILE *arq)
{
    fprintf(arq,"(%d) %s\n", a->matricula, a->nome);
}

Elemento *pesquisa(Arvore *t, Chave x)
{
    Elemento *aluno;
    aluno = malloc(sizeof(Elemento));
    if (t == NULL)
    {
        printf("Registro n�o esta presente na �rvore\n");
    }
    else if (x < t->reg->matricula)
        pesquisa(t->esq, x); /* busca no filho esquerdo*/
    else if (x > t->reg->matricula)
        pesquisa(t->dir, x); /* busca no filho direito*/
    else
        aluno = t->reg;
        return aluno;
}

void insereElemento(Arvore *t, Elemento *reg)
{
    if(t->reg == NULL)
    {
        Arvore *t = malloc(sizeof(Arvore));
        t->reg = reg;
        t->esq = NULL;
        t->dir = NULL;
    }
    else
    {
        if(reg->matricula < t->reg->matricula)
        { /* chave menor */
            if(t->esq)
            {
                insereElemento(t->esq, reg);
            }
            else
            { /* achou local de inser��o */
                Arvore *novo = criaArvore(reg);
                t->esq= novo;
            }
        }
        else
        { /* chave maior ou igual ao nodo atual */
            if(t->dir)
            {
                insereElemento(t->dir, reg);
            }
            else
            {
                Arvore *novo = criaArvore(reg);
                t->dir= novo;
            }
        }
        }
}

Arvore *removeDaArvore(Arvore *t, Chave chave)
{
    Arvore *aux;
    if(t == NULL)
    {
        printf("elemento ausente\n");
    }
    else if(chave < t->reg->matricula)
    {
        t->esq = removeDaArvore(t->esq, chave);
    }
    else if(chave > t->reg->matricula)
    {
        t->dir = removeDaArvore(t->dir, chave);
    }
    else if(t->esq == NULL && t->dir == NULL)
    {
        free(t);
        return NULL; /* zero filhos */
    }
    else if(t->esq== NULL)
    {
    aux=  t->dir;
    free(t);
    return aux; /* 1 filho direita */
    }
    else if(t->dir== NULL)
    {
        aux = t->esq;
        free(t);
        return aux; /* 1 filho esquerda */
    }
    else
    { /* 2 filhos */
        Arvore *suc = achaMenor(t->dir);
        t->reg= suc->reg;
        t->dir= removeDaArvore(t->dir, suc->reg->matricula);
        return t;
    }
    return t;
}

Arvore* achaMenor(Arvore *t)
{
    if(t->esq== NULL)
    {
        return t;
    }
    return achaMenor(t->esq);
}

Arvore *criaArvore(Elemento *r)
{
    Arvore *arvoreAlocada;
    arvoreAlocada = malloc(sizeof(Arvore));
    arvoreAlocada->reg = NULL;
    insereElemento(arvoreAlocada, r);
    return arvoreAlocada;
}

void apagaArvore(Arvore *t)
{
    free(t);
}

void imprimeArvore(Arvore *arvore, FILE *arq)
{
    if(arvore != NULL)
    {
        imprimeArvore(arvore->esq, arq);
        imprimeAluno(arvore->reg, arq);
        imprimeArvore(arvore->dir, arq);
    }
}

void imprimeArvoreSBB(ArvoreSBB **arvore, FILE *arq)
{
    if (arvore != NULL)
    {
        imprimeArvoreSBB(&(*arvore)->esq, arq);
        imprimeAluno((*arvore)->reg, arq);
        imprimeArvoreSBB(&(*arvore)->dir, arq);
    }
}

void apagaArvoreSBB(ArvoreSBB *t)
{
    free(t);
}

void criaArvoreSBB(ArvoreSBB **arvore, Elemento *r)
{
    *arvore = malloc(sizeof(ArvoreSBB));
    (*arvore)->reg = malloc(sizeof(Elemento));
    inicializa(arvore);
    insereElementoSBB(arvore, r);
}

void ee(ArvoreSBB** ptr)
{
    ArvoreSBB *no = *ptr;
    ArvoreSBB *esq = no->esq;
    no->esq= esq->dir; // rotD(ptr)
    esq->dir= no;
    esq->esqtipo = SBB_VERTICAL;
    no->esqtipo = SBB_VERTICAL;
    *ptr= esq;
}

void dd(ArvoreSBB** ptr)
{
    ArvoreSBB *no = *ptr;
    ArvoreSBB *dir= no->dir;
    no->dir= dir->esq;// rotE(ptr)
    dir->esq= no;
    dir->dirtipo= SBB_VERTICAL;
    no->dirtipo= SBB_VERTICAL;
    *ptr= dir;
}

void ed(ArvoreSBB** ptr)
{
    ArvoreSBB *no = *ptr;
    ArvoreSBB *esq = no->esq;
    ArvoreSBB *dir = esq->dir;
    esq->dir = dir->esq; // rotE(&(no->esq))
    dir->esq = esq;
    no->esq= dir->dir;// rotD(ptr)
    dir->dir = no;
    esq->dirtipo = SBB_VERTICAL;
    no->esqtipo = SBB_VERTICAL;
    *ptr= dir;
}

void de(ArvoreSBB **ptr)
{
    ArvoreSBB *no = *ptr;
    ArvoreSBB *dir= no->dir;
    ArvoreSBB *esq= dir->esq;
    dir->esq= esq->dir;// rotD(&(no->dir))
    esq->dir= dir;
    no->dir= esq->esq;// rotE(ptr)
    esq->esq= no;
    dir->esqtipo= SBB_VERTICAL;
    no->dirtipo= SBB_VERTICAL;
    *ptr= esq;
}

void iInsere(Elemento *reg, ArvoreSBB **ptr, int *incli, int *fim)
{
    /* adiciona, pois encontrou uma folha */
    if(*ptr == NULL)
    {
        iInsereAqui(reg, ptr, incli, fim);
        /* busca na sub-�rvoreesquerda */
    }
    else if(reg->matricula < (*ptr)->reg->matricula)
    {
        iInsere(reg, &(*ptr)->esq, &(*ptr)->esqtipo, fim);
        if(*fim)
            return;
        if((*ptr)->esqtipo == SBB_VERTICAL)
        {
            *fim = 1;
        }
        else if((*ptr)->esq->esqtipo == SBB_HORIZONTAL)
        {
            ee(ptr);
            *incli = SBB_HORIZONTAL;
        }
        else if((*ptr)->esq->dirtipo == SBB_HORIZONTAL)
        {
            ed(ptr);
            *incli = SBB_HORIZONTAL;
        }
    }
    /* busca na sub-�rvoredireita */
    else if(reg->matricula > (*ptr)->reg->matricula)
    {
        iInsere(reg, &((*ptr)->dir), &((*ptr)->dirtipo), fim);
        if(*fim)
            return;
    if((*ptr)->dirtipo== SBB_VERTICAL)
    {
        *fim = 1;
    }
    else if((*ptr)->dir->dirtipo== SBB_HORIZONTAL)
    {
        dd(ptr); *incli= SBB_HORIZONTAL;
    }
    else if((*ptr)->dir->esqtipo == SBB_HORIZONTAL)
    {
        de(ptr); *incli= SBB_HORIZONTAL;
    }
    /* chave j� existe */
    }
    else
    {
        printf("erro: chave j� est� na �rvore.\n");
        *fim = 1;
    }
}

void iInsereAqui(Elemento  *reg, ArvoreSBB **ptr, int *incli, int *fim)
{
    ArvoreSBB *no = malloc(sizeof(ArvoreSBB));
    no->reg = reg;
    no->esq = NULL;
    no->dir = NULL;
    no->esqtipo = SBB_VERTICAL;
    no->dirtipo = SBB_VERTICAL;
    *ptr = no;
    *incli = SBB_HORIZONTAL;
    *fim = 0;
}

void insereElementoSBB(ArvoreSBB **raiz, Elemento *reg)
{
    int fim = 0;
    int inclinacao = SBB_VERTICAL;
    iInsere(reg, raiz, &inclinacao, &fim);
}

void inicializa(ArvoreSBB **raiz)
{
    *raiz = NULL;
}

Hash* criaHash(int t) //inicializa com tamanho passado (aloca)
{
    int i;
    Hash *dicHash;
    dicHash = (Hash *) malloc(t * sizeof(Hash));
    dicHash->tam = t;
    for(i = 0; i < t; i++)
    {
        dicHash[i].nElem = i;
        dicHash[i].hashArv = NULL;
        dicHash[i].hashSBB = NULL;
    }
    return dicHash;
}

int funcaoHash(Elemento *aluno, Hash hash)
{
    int posicao;
    posicao = aluno->matricula%hash.tam;
    return posicao;
}

void apagaHash(Hash *hash)
{
    free(hash);
}

void insereNaHash(Hash* h, Elemento* x, int tipoArv)
{
    int posicao;
    posicao = funcaoHash(x, *h);
    if(tipoArv == 0)
    {
        insereElemento(*h[posicao].hashArv, x);
    }
    else
    {
        insereElementoSBB(h[posicao].hashSBB, x);
    }
}

void leArquivo(FILE *arq, Hash *hash, int tipoArv)
{
    int matricula, posicao;
    char *nome, *buffer;
    Elemento *pessoa;
    pessoa = (Elemento *)malloc(sizeof(Elemento));
    nome = (char *)malloc(500 * sizeof(char));
    buffer = (char *)malloc(500 * sizeof(char));
    if(arq == NULL)
    {
        printf("Erro ao abrir o arquivo");
        exit(0);
    }
    while(!feof(arq))
    {
        fgets(buffer, 500, arq);
        matricula = atoi(nome);
        fgets(buffer, 500, arq);
        nome = buffer;
        pessoa = criaAluno(nome, matricula);
        posicao = funcaoHash(pessoa, *hash);
        if(tipoArv == 0)// && hash[posicao].hashArv == NULL)
        {
            *hash[posicao].hashArv = criaArvore(pessoa);
            insereElemento(*hash[posicao].hashArv, pessoa);
        }
        else //if(tipoArv == 1 && hash[posicao].hashSBB == NULL)
        {
            hash[posicao].hashSBB = malloc(sizeof(ArvoreSBB));
            criaArvoreSBB(hash[posicao].hashSBB, pessoa);
        }
        //else
        //{
         //   insereNaHash(hash, pessoa, tipoArv);
        //}
    }
}
