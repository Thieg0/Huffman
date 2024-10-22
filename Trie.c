// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//definido o número de letras do alfabeto
#define ALFABETO 26

//estrtura de um nó da Trie

typedef struct no{
    struct no *filho[ALFABETO];
    bool final_da_palavra;
}No;

//Função para criar um novo nó

No* criarNo(void){
    No *novo_no = malloc(sizeof(No)); // alocando memoria no novo nó
    novo_no->final_da_palavra = false; // inicia com false , pois ainda nao é o fim de nenhuma pavara
    
    //Loop para inicializar os filhos com NULL
    
    for(int i = 0; i < ALFABETO; i++){
        novo_no->filho[i] = NULL;
    }
    return novo_no; // Retorna o novo nó recem criado para quem chamar.
    
}


//Procedimento para inserir uma palavra na Trie(1. Converte o caractere em um indice. Verifica se já existe um n[o filho para aquele cvaractere,Cria um novo nó, se necessario, Avamça para o nó filho])

void inserir(No *raiz, const char *palavra){
    No *no_atual = raiz; //cria um no aux para quando percorrer , não perder o nó.
    
    //Loop para percorre cada caracter da string, ate encontar o fim da string.
    for(int i = 0; palavra[i] != '\0'; i++){
        int indice = palavra[i] - 'a'; //calcula a posição no alfabeto(os caracteres são representados por valores numéricos baseados na tabela ASCII,O valor de 'a' em ASCII é 97. Então, subtraindo 'a', o código transforma as letras em índices de 0 a 25 (representando as letras minúsculas do alfabeto).)
        
        //Se não existe um nó para o caractere, cria um novo
        
        if(!no_atual->filho[indice]){
            no_atual->filho[indice] = criarNo(); //chama a função para para criar um novo nó e o associa ao filho correspondente ao índice do caractere.
        }
        
        //avança para o proximo nó(filho)
        
        no_atual = no_atual->filho[indice];
    }
    //Marca o final da palavra
    no_atual->final_da_palavra = true;
    
}

//função para buscar uma palavra na Trie

bool buscar(No *raiz, const char *palavra){
    No *no_atual = raiz;
    
    for (int i = 0; palavra[i] != '\0'; i++) {
        int indice = palavra[i] - 'a';  // Calcula a posição no alfabeto
        
        // Se o nó para o caractere não existir, a palavra não está presente
        if (!no_atual->filho[indice]) {
            return false;
        }
        
        // Avança para o próximo nó
        no_atual = no_atual->filho[indice];
    }
    
    // Retorna verdadeiro se o nó final marcar o fim de uma palavra
    return (no_atual != NULL && no_atual->final_da_palavra);
}

// Função para liberar a memória da Trie
void liberarTrie(No *no_atual) {
    for (int i = 0; i < ALFABETO; i++) {
        if (no_atual->filho[i]) {
            liberarTrie(no_atual->filho[i]);
        }
    }
    
    free(no_atual);  // Libera o nó
}

// Função auxiliar para imprimir as palavras armazenadas na Trie
void imprimir_trie(No *raiz, char *prefixo, int nivel) {
    if (raiz->final_da_palavra) {
        prefixo[nivel] = '\0';
        printf("%s\n", prefixo);
    }

    for (int i = 0; i < ALFABETO; i++) {
        if (raiz->filho[i]) {
            prefixo[nivel] = i + 'a';
            imprimir_trie(raiz->filho[i], prefixo, nivel + 1);
        }
    }
}

// Função para imprimir as palavras na Trie
void imprimir_palavras(No *raiz) {
    char prefixo[100];
    imprimir_trie(raiz, prefixo, 0);
}

// Função para remover uma palavra da Trie
bool remover_palavra(No *raiz, char *palavra, int profundidade) {
    if (!raiz) {
        return false;  // Se o nó atual é NULL, a palavra não pode ser removida
    }

    // Se chegamos ao final da palavra
    if (profundidade == strlen(palavra)) {
        // Se a palavra está marcada como final, desmarcamos
        if (raiz->final_da_palavra) {
            raiz->final_da_palavra = false;
            // Se não tem filhos, podemos deletar este nó
            for (int i = 0; i < ALFABETO; i++) {
                if (raiz->filho[i]) {
                    return false;  // Tem filhos, não pode ser deletado
                }
            }
            return true;  // O nó pode ser deletado
        }
        return false;  // A palavra não estava na Trie
    }

    int indice = palavra[profundidade] - 'a';  // Calcula o índice da letra

    // Chama recursivamente para o próximo nó
    if (remover_palavra(raiz->filho[indice], palavra, profundidade + 1)) {
        // Se a remoção retornou true, significa que o filho pode ser deletado
        free(raiz->filho[indice]);
        raiz->filho[indice] = NULL;

        // Verifica se este nó ainda é um prefixo de outra palavra
        if (raiz->final_da_palavra) {
            return false;  // Não podemos deletar este nó, pois é parte de outra palavra
        }
        
        // Verifica se não tem filhos, então pode deletar este nó
        for (int i = 0; i < ALFABETO; i++) {
            if (raiz->filho[i]) {
                return false;  // Tem filhos, não pode ser deletado
            }
        }
        return true;  // O nó pode ser deletado
    }

    return false;  // A palavra não foi encontrada
}


int main() {
    
    
    No *raiz = criarNo();  // Inicializa a Trie
    int opcao;
    char palavra[100];
    
    do {
        printf("\n\t0 - Sair\n\t1 - Inserir palavra\n\t2 - Buscar palavras\n\t3- Imprimir palavras\n\t4 - Remover palavras\n");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado
        
        switch(opcao) {
            case 1:
                printf("\n\tDigite uma palavra: ");
                scanf("%s", palavra);
                inserir(raiz, palavra);
                break;
            case 2:
                printf("\n\tDigite uma palavra para buscar: ");
                scanf("%s", palavra);
                if (buscar(raiz, palavra)) {
                    printf("\n\tA palavra '%s' foi encontrada na Trie.\n", palavra);
                } else {
                    printf("\n\tA palavra '%s' não está na Trie.\n", palavra);
                }
                break;
            case 3:
                printf("\n\tPalavras armazenadas na Trie:\n");
                imprimir_palavras(raiz);
                break;
                
            case 4:
                printf("\n\tDigite uma palavra para remover: ");
                scanf("%s", palavra);
                if (remover_palavra(raiz, palavra, 0)) {
                    printf("\n\tA palavra '%s' foi removida da Trie.\n", palavra);
                } else {
                    printf("\n\tA palavra '%s' não foi encontrada ou não pode ser removida.\n", palavra);
                }
                break;

            default:
                if(opcao != 0) {
                    printf("\n\tOpção inválida!!!\n");
                }
        }
    } while (opcao != 0);

    
}
