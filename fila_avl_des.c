#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura de um nó para a lista encadeada
typedef struct Node {
    int valor;               // Valor armazenado no nó
    struct Node* proximo;    // Ponteiro para o próximo nó
} Node;

// Estrutura da lista encadeada
typedef struct Lista {
    Node* cabeca;           // Ponteiro para o primeiro nó (cabeça)
} Lista;

// Cria e inicializa uma nova lista
Lista* criar_lista() {
    Lista* lista = (Lista*)malloc(sizeof(Lista)); // Aloca memória para a lista
    lista->cabeca = NULL;                          // Inicializa a cabeça como NULL
    return lista;
}

// Adiciona um novo valor à lista
void adicionar_lista(Lista* lista, int valor) {
    Node* novo_nodo = (Node*)malloc(sizeof(Node)); // Aloca memória para o novo nó
    novo_nodo->valor = valor;                       // Define o valor do novo nó
    novo_nodo->proximo = lista->cabeca;            // O novo nó aponta para a antiga cabeça
    lista->cabeca = novo_nodo;                     // Atualiza a cabeça para o novo nó
}

// Conta e retorna o número de elementos na lista
int contar_lista(Lista* lista) {
    int contador = 0;
    Node* atual = lista->cabeca;                   // Começa pela cabeça
    while (atual != NULL) {                         // Percorre até o final da lista
        contador++;
        atual = atual->proximo;
    }
    return contador;
}

// Libera a memória ocupada pela lista
void liberar_lista(Lista* lista) {
    Node* atual = lista->cabeca;                   // Começa pela cabeça
    while (atual != NULL) {                         // Enquanto houver nós
        Node* temp = atual;                         // Guarda o nó atual
        atual = atual->proximo;                     // Move para o próximo nó
        free(temp);                                 // Libera o nó atual
    }
    free(lista);                                    // Libera a lista em si
}

// Estrutura de um nó para a árvore desbalanceada
typedef struct Nodo {
    int valor;                // Valor armazenado no nó
    struct Nodo* esquerda;    // Ponteiro para o filho esquerdo
    struct Nodo* direita;     // Ponteiro para o filho direito
} Nodo;

// Estrutura da árvore desbalanceada
typedef struct ArvoreDesbalanceada {
    Nodo* raiz;              // Ponteiro para a raiz da árvore
} ArvoreDesbalanceada;

// Cria e inicializa uma nova árvore desbalanceada
ArvoreDesbalanceada* criar_arvore() {
    ArvoreDesbalanceada* arvore = (ArvoreDesbalanceada*)malloc(sizeof(ArvoreDesbalanceada));
    arvore->raiz = NULL;   // Inicializa a raiz como NULL
    return arvore;
}

// Adiciona um novo valor à árvore desbalanceada de forma recursiva
Nodo* adicionar_nodo(Nodo* nodo, int valor) {
    if (nodo == NULL) {  // Se o nó é NULL, cria um novo
        Nodo* novo_nodo = (Nodo*)malloc(sizeof(Nodo));
        novo_nodo->valor = valor;
        novo_nodo->esquerda = NULL;
        novo_nodo->direita = NULL;
        return novo_nodo;
    }

    // Insere o valor na posição correta
    if (valor < nodo->valor) {
        nodo->esquerda = adicionar_nodo(nodo->esquerda, valor);
    } else {
        nodo->direita = adicionar_nodo(nodo->direita, valor);
    }

    return nodo; // Retorna o nó atualizado
}

// Função que adiciona um valor à árvore desbalanceada
void adicionar_arvore(ArvoreDesbalanceada* arvore, int valor) {
    arvore->raiz = adicionar_nodo(arvore->raiz, valor);
}

// Conta o número de nós na árvore
int contar_arvore(Nodo* nodo) {
    if (nodo == NULL) {
        return 0;  // Se o nó é NULL, retorna 0
    }
    return 1 + contar_arvore(nodo->esquerda) + contar_arvore(nodo->direita); // Conta recursivamente
}

// Função que conta nós na árvore desbalanceada
int contar_arvore_externa(ArvoreDesbalanceada* arvore) {
    return contar_arvore(arvore->raiz); // Chama a função de contagem a partir da raiz
}

// Libera a memória ocupada pela árvore recursivamente
void liberar_arvore(Nodo* nodo) {
    if (nodo != NULL) {
        liberar_arvore(nodo->esquerda); // Libera a subárvore esquerda
        liberar_arvore(nodo->direita);  // Libera a subárvore direita
        free(nodo);                      // Libera o nó atual
    }
}

// Libera a memória ocupada pela árvore desbalanceada
void liberar_arvore_externa(ArvoreDesbalanceada* arvore) {
    liberar_arvore(arvore->raiz); // Libera a árvore a partir da raiz
    free(arvore);                 // Libera a estrutura da árvore
}

// Estrutura de um nó para a árvore AVL
typedef struct NodoAVL {
    int valor;                // Valor armazenado no nó
    struct NodoAVL* esquerda; // Ponteiro para o filho esquerdo
    struct NodoAVL* direita;  // Ponteiro para o filho direito
    int altura;              // Altura do nó
} NodoAVL;

// Retorna a altura de um nó
int altura(NodoAVL* nodo) {
    return nodo ? nodo->altura : 0; // Retorna a altura ou 0 se o nó é NULL
}

// Retorna o valor máximo entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Realiza uma rotação à direita no nó y
NodoAVL* rotacionar_direita(NodoAVL* y) {
    NodoAVL* x = y->esquerda;        // Define x como o filho esquerdo de y
    NodoAVL* T2 = x->direita;        // T2 é o filho direito de x

    // Realiza a rotação
    x->direita = y;
    y->esquerda = T2;

    // Atualiza as alturas
    y->altura = 1 + max(altura(y->esquerda), altura(y->direita));
    x->altura = 1 + max(altura(x->esquerda), altura(x->direita));

    return x; // Retorna o novo nó raiz
}

// Realiza uma rotação à esquerda no nó x
NodoAVL* rotacionar_esquerda(NodoAVL* x) {
    NodoAVL* y = x->direita;         // Define y como o filho direito de x
    NodoAVL* T2 = y->esquerda;       // T2 é o filho esquerdo de y

    // Realiza a rotação
    y->esquerda = x;
    x->direita = T2;

    // Atualiza as alturas
    x->altura = 1 + max(altura(x->esquerda), altura(x->direita));
    y->altura = 1 + max(altura(y->esquerda), altura(y->direita));

    return y; // Retorna o novo nó raiz
}

// Retorna o fator de balanceamento de um nó
int obter_balanceamento(NodoAVL* nodo) {
    if (nodo == NULL) return 0;
    return altura(nodo->esquerda) - altura(nodo->direita);
}

// Adiciona um novo valor à árvore AVL de forma recursiva
NodoAVL* adicionar_avl(NodoAVL* nodo, int valor) {
    if (nodo == NULL) { // Se o nó é NULL, cria um novo
        NodoAVL* novo_nodo = (NodoAVL*)malloc(sizeof(NodoAVL));
        novo_nodo->valor = valor;
        novo_nodo->esquerda = NULL;
        novo_nodo->direita = NULL;
        novo_nodo->altura = 1; // Novo nó é inicialmente adicionado como folha
        return novo_nodo;
    }

    // Insere o valor na posição correta
    if (valor < nodo->valor) {
        nodo->esquerda = adicionar_avl(nodo->esquerda, valor);
    } else if (valor > nodo->valor) {
        nodo->direita = adicionar_avl(nodo->direita, valor);
    } else {
        return nodo; // Não permitir duplicatas
    }

    // Atualiza a altura do nó ancestral
    nodo->altura = 1 + max(altura(nodo->esquerda), altura(nodo->direita));

    // Verifica o balanceamento e realiza rotações se necessário
    int balanceamento = obter_balanceamento(nodo);

    // Rotação à direita
    if (balanceamento > 1 && valor < nodo->esquerda->valor)
        return rotacionar_direita(nodo);
    // Rotação à esquerda
    if (balanceamento < -1 && valor > nodo->direita->valor)
        return rotacionar_esquerda(nodo);
    // Rotação esquerda-direita
    if (balanceamento > 1 && valor > nodo->esquerda->valor) {
        nodo->esquerda = rotacionar_esquerda(nodo->esquerda);
        return rotacionar_direita(nodo);
    }
    // Rotação direita-esquerda
    if (balanceamento < -1 && valor < nodo->direita->valor) {
        nodo->direita = rotacionar_direita(nodo->direita);
        return rotacionar_esquerda(nodo);
    }

    return nodo; // Retorna o nó atualizado
}

// Estrutura da árvore AVL
typedef struct ArvoreAVL {
    NodoAVL* raiz;  // Ponteiro para a raiz da árvore
} ArvoreAVL;

// Cria e inicializa uma nova árvore AVL
ArvoreAVL* criar_arvore_avl() {
    ArvoreAVL* arvore = (ArvoreAVL*)malloc(sizeof(ArvoreAVL));
    arvore->raiz = NULL; // Inicializa a raiz como NULL
    return arvore;
}

// Adiciona um novo valor à árvore AVL
void adicionar_arvore_avl(ArvoreAVL* arvore, int valor) {
    arvore->raiz = adicionar_avl(arvore->raiz, valor);
}

// Conta o número de nós na árvore AVL
int contar_arvore_avl(NodoAVL* nodo) {
    if (nodo == NULL) {
        return 0; // Se o nó é NULL, retorna 0
    }
    return 1 + contar_arvore_avl(nodo->esquerda) + contar_arvore_avl(nodo->direita); // Conta recursivamente
}

// Função que conta nós na árvore AVL
int contar_arvore_avl_externa(ArvoreAVL* arvore) {
    return contar_arvore_avl(arvore->raiz); // Chama a função de contagem a partir da raiz
}

// Libera a memória ocupada pela árvore AVL
void liberar_arvore_avl(NodoAVL* nodo) {
    if (nodo != NULL) {
        liberar_arvore_avl(nodo->esquerda); // Libera a subárvore esquerda
        liberar_arvore_avl(nodo->direita);  // Libera a subárvore direita
        free(nodo);                          // Libera o nó atual
    }
}

// Libera a memória ocupada pela árvore AVL
void liberar_arvore_avl_externa(ArvoreAVL* arvore) {
    liberar_arvore_avl(arvore->raiz); // Libera a árvore a partir da raiz
    free(arvore);                      // Libera a estrutura da árvore
}

// Função principal
#define NUM_ELEMENTOS 50 // Define o número de elementos a serem inseridos

int main() {
    Lista* lista = criar_lista();
    ArvoreDesbalanceada* arvore_desbalanceada = criar_arvore();
    ArvoreAVL* arvore_avl = criar_arvore_avl();

    clock_t inicio, fim;
    double tempo_lista, tempo_arvore_desbalanceada, tempo_arvore_avl;

    // Adicionando elementos à lista e medindo o tempo
    inicio = clock();
    for (int i = 1; i <= NUM_ELEMENTOS; i++) {
        adicionar_lista(lista, i);
    }
    fim = clock();
    tempo_lista = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Contando elementos na lista
    inicio = clock();
    int contagem_lista = contar_lista(lista);
    fim = clock();
    tempo_lista += ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Adicionando elementos à árvore desbalanceada e medindo o tempo
    inicio = clock();
    for (int i = 1; i <= NUM_ELEMENTOS; i++) {
        adicionar_arvore(arvore_desbalanceada, i);
    }
    fim = clock();
    tempo_arvore_desbalanceada = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Contando elementos na árvore desbalanceada
    inicio = clock();
    int contagem_arvore_desbalanceada = contar_arvore_externa(arvore_desbalanceada);
    fim = clock();
    tempo_arvore_desbalanceada += ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Adicionando elementos à árvore AVL e medindo o tempo
    inicio = clock();
    for (int i = 1; i <= NUM_ELEMENTOS; i++) {
        adicionar_arvore_avl(arvore_avl, i);
    }
    fim = clock();
    tempo_arvore_avl = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Contando elementos na árvore AVL
    inicio = clock();
    int contagem_arvore_avl = contar_arvore_avl_externa(arvore_avl);
    fim = clock();
    tempo_arvore_avl += ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Exibindo resultados
    printf("Resultados:\n");
    printf("Lista: %d elementos (Tempo: %.6f segundos)\n", contagem_lista, tempo_lista);
    printf("Árvore Desbalanceada: %d elementos (Tempo: %.6f segundos)\n", contagem_arvore_desbalanceada, tempo_arvore_desbalanceada);
    printf("Árvore AVL: %d elementos (Tempo: %.6f segundos)\n", contagem_arvore_avl, tempo_arvore_avl);

    // Liberar memória
    liberar_lista(lista);
    liberar_arvore_externa(arvore_desbalanceada);
    liberar_arvore_avl_externa(arvore_avl);

    return 0;
}
