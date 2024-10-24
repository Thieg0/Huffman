#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define SIZE_ASCII 256

typedef struct node {
    unsigned char item;
    int frequency;
    struct node *left, *right, *next;
}node;

typedef struct list {
    node *start;
    int size;
}list;

void build_table_with_zero(unsigned int tab[]) {
    int i;
    for (i = 0; i < SIZE_ASCII; i++) {
        tab[i] = 0;
    }
}

void build_frequency_table(unsigned char text[], unsigned int tab[]) {
    int i = 0;

    while (text[i] != '\0') {
        tab[text[i]]++;
        i++;    
    }
}

void print_frequency_table(unsigned int tab[]) {
    for (int i = 0; i < SIZE_ASCII; i++) {
        if (tab[i] > 0){
            printf("\t%d = %u = %c\n", i, tab[i], i);
        }
    }
}

void create_list(list *list) {
    list->start = NULL;
    list->size = 0;
}

void ordered_insert(list *list, node *new_node) {
    node *temp;  // Mover a declaração de 'aux' para o início da função
    
    // Is the list empty?
    if (list->start == NULL) {
        list->start = new_node;
    } 
    // The frequency is lower than the list start
    else if (new_node->frequency < list->start->frequency) {
        new_node->next = list->start;
        list->start = new_node;
    }
    else {
        temp = list->start;
        while (temp->next && temp->next->frequency <= new_node->frequency) {
            temp = temp->next;
        }
        new_node->next = temp->next;
        temp->next = new_node;
    }
    list->size++;
}


void building_list(unsigned int tab[], list *list) {
    int i; 
    node *new;
    for (i = 0; i < SIZE_ASCII; i++) {
        if (tab[i] > 0) {
            new = malloc (sizeof(node));
            if (new) {
                new->item = i;
                new->frequency = tab[i];
                new->left = NULL;
                new->right = NULL;
                new->next = NULL;

                ordered_insert(list, new);
            } else {
                printf("\tError to alocate memory at building_list\n");
                break;
            }
        }
    }
}

void print_list(list *list) {
    node *aux = list->start;

    printf("\nLista ordenada: Tamanho: %d\n", list->size);
    while (aux) {
        printf("\tItem: %c Frequency: %d\n", aux->item, aux->frequency);
        aux = aux->next;
    }
}

node* remove_at_start(list *list) {
    node *aux = NULL;

    if(list->start) {
        aux = list->start;
        list->start = aux->next;
        aux->next = NULL;
        list->size--;
    }

    return aux;
}

node* build_tree(list *list)  {
    node *first, *second, *new;
    
    while (list->size > 1) {
        first = remove_at_start(list);
        second = remove_at_start(list);
        new = malloc(sizeof(node));

        if (new) {
            new->item = '*';
            new->frequency = first->frequency + second->frequency;
            new->left = first;
            new->right = second;
            new->next = NULL;

            ordered_insert(list, new);
        } else {
            printf("\tError to alocate memory at build_tree!\n");
            break;
        }
    }
    return list->start;
}

void print_tree(node *root, int size) {
    if (root->left == NULL && root->right == NULL) {
        printf("\tFolha: %c\tAltura: %d\n", root->item, size);
    } else {
        print_tree(root->left, size + 1);
        print_tree(root->right, size + 1);
    }
}

int trie_height(node *root) {
    int left, right;

    if (root == NULL) {
        return -1;
    } else {
        left = trie_height(root->left) + 1;
        right = trie_height(root->right) + 1;
        if (left > right) {
            return left;
        } else {
            return right;
        }
    }
}

char** allocate_dictionary(int columns) {
    char** dictionary;

    dictionary = malloc(sizeof(char*) * SIZE_ASCII);
    for (int i = 0; i < SIZE_ASCII; i++) {
        dictionary[i] = calloc(columns, sizeof(char));
    }

    return dictionary;
}

void create_dictionary(char **dictionary, node *root, char *path, int columns) {
    char left[columns], right[columns];
    if (root->left == NULL && root->right == NULL) {
        strcpy(dictionary[root->item], path);
    } else {
        strcpy(left, path);
        strcpy(right, path);

        strcat(left, "0");
        strcat(right, "1");

        create_dictionary(dictionary, root->left, left, columns);
        create_dictionary(dictionary, root->right, right, columns);
    }
}

void print_dictionary(char **dictionary) {
    printf("\n\tDictionary:\n");
    for (int i = 0; i < SIZE_ASCII; i++) {
        if (strlen(dictionary[i]) > 0) {
            printf("\t%3d: %s\n", i, dictionary[i]);
        }
    }
}

int calculate_size_string(char **dictionary, char *text) {
    int i = 0, size = 0;
    while (text[i] != '\0') {
        size = size + strlen(dictionary[text[i]]);
        i++;
    }
    return size + 1;
}

char* encode(char **dictionary, unsigned char *text) {

    int i = 0, size = calculate_size_string(dictionary, text);

    char *code = calloc(size, sizeof(char));

    while (text[i] != '\0') {
        strcat(code, dictionary[text[i]]);   
        i++; 
    }
    return code;
}

int main() {
    unsigned char text[] = "Teste de huffman";
    unsigned int frequency_table[SIZE_ASCII];
    list list;
    node *tree;
    int columns;
    char **dictionary;
    char *encoded;

    setlocale(LC_ALL, "Portuguese");

    build_table_with_zero(frequency_table);
    build_frequency_table(text, frequency_table);
    print_frequency_table(frequency_table);

    create_list(&list);
    building_list(frequency_table, &list);
    print_list(&list);

    tree = build_tree(&list);
    printf("\n\tÁrvore de huffman\n");
    print_tree(tree, 0);

    columns = trie_height(tree) + 1;
    dictionary = allocate_dictionary(columns);
    create_dictionary(dictionary, tree, "", columns);
    print_dictionary(dictionary);

    encoded = encode(dictionary, text);
    printf("\n\tTexto codificado: %s\n", encoded);

    return 0;
}