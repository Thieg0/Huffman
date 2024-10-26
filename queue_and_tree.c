unsigned char character = '*';

typedef struct node {
    void *item;
    int frequency;
    struct node *next;
    struct node *left;
    struct node *right;
}

node* create_empty_queue() {
    return NULL;
}

node* create_empty_tree() {
    return NULL;
}

int is_empty(node *pq) {
    return pq == NULL;
}

node* insert_node(node *pq, node *new_node) {
    if (is_empty(pq)) { // se pq for NULL, a lista está vazia, então o new_node se torna o primeiro nó da fila.
        return new_node;
    } else if (new_node->frequency <= pq->frequency) { // se a frequência do new_node for menor que a frequência do pq (nó inicial), new_node se torna o primeiro nó da fila.
        new_node->next = pq;

        return new_node;
    } else { // Inserção no meio ou final da lista
        node *current = pq;
        while ((current->next != NULL) && (current->next->frequency < new_node->frequency)) { // O while percorre a fila até encontrar a posição onde o frequency do próximo nó (current->next) é maior ou igual ao de new_node.
            current = current->next;
        }
        new_node->next = current->next; // Ao final do loop, current aponta para o nó após o qual new_node deve ser inserido.
        current->next = new_node;

        return pq;
    }
}

node* enqueue(node *pq, void *data, int freq) { // Cria um novo nó, inicializa os valores dele e o insere na fila ordenada
    unsigned char *new_node_unsigned_char = (unsigned char*) malloc(sizeof(unsigned char));

    *new_node_unsigned_char = *(unsigned char*) data;

    node *new_node = (node*) malloc(sizeof(node));
    new_node->item = new_node_unsigned_char;
    new_node->frequency = freq;
    new_node->next = NULL;
    new_node->left = NULL;
    new_node->right = NULL;

    pq = insert_node(pq, new_node);

    return pq;
}

node* merge(node *pq) {
    node *new_node = (node*) malloc(sizeof(node));
    new_node->item = &character;
    new_node->frequency = (pq->frequency) + (pq->next->frequency);
    new_node->next = NULL;
    new_node->left = pq;
    new_node->right = pq->next;

    if(new_node->right->next != NULL) { // Se tiver mais que 2 nós na fila
        pq = new_node->right->next;

        new_node->left->next = NULL;
        new_node->right->next = NULL;

        pq = insert_node(pq, new_node);
    } else { // Se apenas tiver 2 nós na fila.  
        pq = new_node;
    }

    return pq;
}

node* huffman_tree(node *pq) {
    if (pq->next == NULL) { // Se a fila tiver apenas um nó
        node* new_node = (node*) malloc(sizeof(node));
        new_node->item = &character;
        new_node->frequency = pq->frequency;
        new_node->next = NULL;
        new_node->left = pq;
        new_node->right = NULL;

        pq = new_node;
    } else {
        while (pq->next != NULL) { 
            pq = merge(pq);
        }
    }
    return pq;
}

int is_leaf(node *pq) {
    return (pq->left == NULL && pq->right == NULL);
}

int size_huff(node *huff) {
    if (huff == NULL) {
        return 0;
    } else {
        return 1 + size_huff(huff->left) + size_huff(huff->right);
    }
}

void print_tree_with_node_attributes(node *pq) {
    printf("\n");
    if (!is_empty(pq)) { // Não estiver vazio
        printf("Node:\n\taddress[%p]\n\titem[%c]\n\tfrequency[%d]\n\tnext[%p]\n\tleft[%p]\n\tright[%p]\n\n", pq,*(unsigned char*) pq->item, pq->frequency, pq->next, pq->left, pq->right);
        print_tree_with_node_atributes (pq->left);
        print_tree_with_node_atributes (pq->right);
    }
}

void print_tree_prison(node *pq) {
    if (!is_empty(pq)) {
        printf("[%c]", *(unsigned char*) pq->item);
        print_tree_prison(pq->left);
        print_tree_prison(pq->right);
    }
}

void print_tree(node *pq) {
    if(!is_empty) {
        printf("%c", *(unsigned char*) pq->item);
        print_tree (pq->left);
        print_tree (pq->right);
    }
}

void print_priority_queue(node *pq) {
    if (is_empty(pq)) {
        printf("It is empty.\n");
    } else {
        printf("print_priority_queue ---- Start\n");

        node *current = pq;
        while (current != NULL) {
            printf("Node:\n\taddress[%p]\n\titem[%c]\n\tfrequency[%d]\n\tnext[%p]\n\tleft[%p]\n\tright[%p]\n", current,*(unsigned char*) current->item, current->frequency, current->next, current->left, current->right);

            current = current->next;
        }

        printf("print_priority_queue ---- End\n");
    }
}