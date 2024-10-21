#include <stdio.h>
#include <stdlib.h>
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

int main() {
    unsigned char text[] = "Teste de huffman";
    unsigned int frequency_table[SIZE_ASCII];
    list list;
    node *tree;

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

    return 0;
}