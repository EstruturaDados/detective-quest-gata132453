#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// ===============================
// NÍVEL NOVATO - Árvore Binária
// ===============================

typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para sala.\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

void conectarSalas(Sala *pai, Sala *esquerda, Sala *direita) {
    if (pai != NULL) {
        pai->esquerda = esquerda;
        pai->direita = direita;
    }
}

void explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);
        printf("Digite [e] esquerda, [d] direita, [s] sair: ");
        scanf(" %c", &opcao);

        if (opcao == 's') {
            printf("Exploração encerrada.\n");
            break;
        } else if (opcao == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há sala à esquerda.\n");
            }
        } else if (opcao == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há sala à direita.\n");
            }
        } else {
            printf("Opção inválida.\n");
        }
    }
}

void liberarSalas(Sala *raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

// ======================================
// NÍVEL AVENTUREIRO - BST de Pistas
// ======================================

typedef struct Pista {
    char texto[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

Pista* criarPista(const char *texto) {
    Pista *nova = (Pista*) malloc(sizeof(Pista));
    if (nova == NULL) {
        printf("Erro ao alocar memória para pista.\n");
        exit(1);
    }

    strcpy(nova->texto, texto);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

Pista* inserirBST(Pista *raiz, const char *texto) {
    if (raiz == NULL) {
        return criarPista(texto);
    }

    int cmp = strcmp(texto, raiz->texto);

    if (cmp < 0) {
        raiz->esquerda = inserirBST(raiz->esquerda, texto);
    } else if (cmp > 0) {
        raiz->direita = inserirBST(raiz->direita, texto);
    }

    return raiz;
}

void emOrdem(Pista *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        emOrdem(raiz->direita);
    }
}

void liberarPistas(Pista *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// ==========================================
// NÍVEL MESTRE - Tabela Hash de Suspeitos
// ==========================================

typedef struct ListaPista {
    char pista[100];
    struct ListaPista *prox;
} ListaPista;

typedef struct Suspeito {
    char nome[50];
    int contador;
    ListaPista *pistas;
    struct Suspeito *prox;
} Suspeito;

Suspeito *tabelaHash[TAM_HASH];

void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

int funcaoHash(const char *nome) {
    int soma = 0;
    for (int i = 0; nome[i] != '\0'; i++) {
        soma += nome[i];
    }
    return soma % TAM_HASH;
}

Suspeito* buscarSuspeito(const char *nome) {
    int indice = funcaoHash(nome);
    Suspeito *atual = tabelaHash[indice];

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    return NULL;
}

void inserirHash(const char *pista, const char *suspeitoNome) {
    int indice = funcaoHash(suspeitoNome);
    Suspeito *suspeito = buscarSuspeito(suspeitoNome);

    if (suspeito == NULL) {
        suspeito = (Suspeito*) malloc(sizeof(Suspeito));
        if (suspeito == NULL) {
            printf("Erro ao alocar memória para suspeito.\n");
            exit(1);
        }

        strcpy(suspeito->nome, suspeitoNome);
        suspeito->contador = 0;
        suspeito->pistas = NULL;
        suspeito->prox = tabelaHash[indice];
        tabelaHash[indice] = suspeito;
    }

    ListaPista *novaPista = (ListaPista*) malloc(sizeof(ListaPista));
    if (novaPista == NULL) {
        printf("Erro ao alocar memória para lista de pistas.\n");
        exit(1);
    }

    strcpy(novaPista->pista, pista);
    novaPista->prox = suspeito->pistas;
    suspeito->pistas = novaPista;

    suspeito->contador++;
}

void listarAssociacoes() {
    printf("\n=== SUSPEITOS E SUAS PISTAS ===\n");

    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito *atual = tabelaHash[i];

        while (atual != NULL) {
            printf("\nSuspeito: %s\n", atual->nome);
            printf("Quantidade de pistas: %d\n", atual->contador);
            printf("Pistas associadas:\n");

            ListaPista *p = atual->pistas;
            while (p != NULL) {
                printf("  - %s\n", p->pista);
                p = p->prox;
            }

            atual = atual->prox;
        }
    }
}

void mostrarSuspeitoMaisProvavel() {
    Suspeito *maisProvavel = NULL;

    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito *atual = tabelaHash[i];

        while (atual != NULL) {
            if (maisProvavel == NULL || atual->contador > maisProvavel->contador) {
                maisProvavel = atual;
            }
            atual = atual->prox;
        }
    }

    if (maisProvavel != NULL) {
        printf("\n=== SUSPEITO MAIS PROVÁVEL ===\n");
        printf("Nome: %s\n", maisProvavel->nome);
        printf("Número de pistas: %d\n", maisProvavel->contador);
    }
}

void liberarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito *atual = tabelaHash[i];

        while (atual != NULL) {
            Suspeito *tempSuspeito = atual;
            ListaPista *p = atual->pistas;

            while (p != NULL) {
                ListaPista *tempPista = p;
                p = p->prox;
                free(tempPista);
            }

            atual = atual->prox;
            free(tempSuspeito);
        }
    }
}

// ===============================
// MAIN
// ===============================

int main() {
    // -------- NÍVEL NOVATO --------
    Sala *hall = criarSala("Hall de Entrada");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sótão");
    Sala *jardim = criarSala("Jardim");

    conectarSalas(hall, biblioteca, cozinha);
    conectarSalas(biblioteca, sotao, NULL);
    conectarSalas(cozinha, jardim, NULL);

    printf("=== DETECTIVE QUEST ===\n");
    printf("\nExploração da mansão:\n");
    explorarSalas(hall);

    // -------- NÍVEL AVENTUREIRO --------
    Pista *arvorePistas = NULL;

    arvorePistas = inserirBST(arvorePistas, "Impressão digital");
    arvorePistas = inserirBST(arvorePistas, "Pegadas na cozinha");
    arvorePistas = inserirBST(arvorePistas, "Chave dourada");
    arvorePistas = inserirBST(arvorePistas, "Bilhete rasgado");
    arvorePistas = inserirBST(arvorePistas, "Luva preta");

    printf("\n=== PISTAS EM ORDEM ALFABÉTICA ===\n");
    emOrdem(arvorePistas);

    // -------- NÍVEL MESTRE --------
    inicializarHash();

    inserirHash("Impressão digital", "Carlos");
    inserirHash("Pegadas na cozinha", "Maria");
    inserirHash("Chave dourada", "Carlos");
    inserirHash("Bilhete rasgado", "João");
    inserirHash("Luva preta", "Carlos");

    listarAssociacoes();
    mostrarSuspeitoMaisProvavel();

    // Liberação de memória
    liberarSalas(hall);
    liberarPistas(arvorePistas);
    liberarHash();

    return 0;
}


