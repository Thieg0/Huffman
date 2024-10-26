void freq_count(void *file, hash_table *ht) {
    // Esta função lê o arquivo byte a byte e conta a frequência de cada caractere, armazenando os resultados na tabela de hash.
    unsigned char *buffer;
    int i;
    fseek(file, 0, SEEK_END); // Move o indicador para o final do arquivo.
    int file_size = ftell(file); // Obtém o tamanho do arquivo.
    rewind(file); // Reposiciona o indicador para o início do arquivo.
    buffer = (unsigned char*) malloc(sizeof(char) * file_size); // Aloca memória para o buffer. o buffer serve para facilitar a manipulação e transferência dos dados.
    fread(buffer, 1, file_size, file); // Lê o arquivo

    for (i = 0; i < file_size; i++) {
        ht->table[buffer[i]]->frequency++;
    }
    free(buffer);
}

int escape(node *huffman_tree, int escapes) {
    // Esta função conta o número de folhas de escape na árvore de Huffman (representadas por * ou \*) e retorna a contagem.
    if (!is_empty(huffman_tree)) {
        if ((*(unsigned char*) huffman_tree->item == 42 || *(unsigned char*) huffman_tree->item == '\\') && is_leaf(huffman_tree)) { // 42 é o '*' na tabela ASCII
            ++escapes;
        }
        escapes = escape(huffman_tree->left, escapes);
        escapes = escape(huffman_tree->right, escapes);
    }
    return escapes;
}

void fprint_tree_bytes_header(void *file, node *huffman_tree) {
    // Esta função escreve a representação pré-ordem da árvore de Huffman no arquio, usando \* para folhas de escape.
    if (!is_empty(huffman_tree)) {
        if ((*(unsigned char*) huffman_tree->item == 42 || *(unsigned char*) huffman_tree->item == '\\') && is_leaf(huffman_tree)) {
            fprintf(file, "\\%c", (*(unsigned char*) huffman_tree->item));
        } else {
            fprintf(file, "%c", (*(unsigned char*) huffman_tree->item));
        }
        fprint_tree_bytes_header(file, huffman_tree->left);
        fprint_tree_bytes_header(file, huffman_tree->right);
    }
}

int int_bin_converter(unsigned int c, int i) {
    // Esta função converte um número inteiro em sua representação binária no índice i.
    unsigned char mask = c >> i;
    return mask & i;
}

void int_bin(char *bin, int num, int bits) {
    // Esta função converte um número decimal em sua representatividade binária em sua string de bits.
    int i;
    for (i = 0; i < bits; i++) {
        bin[bits - i - 1] = int_bin_converter(num, i) + '0'; // Converte de inteiro para caractere.
    }
    bin[strlen(bin)] = '\0';
}

int set_bit(unsigned char c, int i) {
    // Esta função define o bit 1 (um) na posição i.
    unsigned char mask = 1 << i;
    return mask | c;
}

char* get_char_new_map(hash_table *ht, unsigned char c) {
    // Esta função retorna o novo mapeamento de bits para um byte específico na tabela de hash.
    return ht->table[(int)c]->bits;
}

int write_compressed_file(void *source_file, void *compressed_file, hash_table *ht) {
    // Essa função escreve o arquivo comprimido usando o mapeamento de bits da tabela de hash.
    unsigned char byteread; // Armazena cada byte lido do arquivo de origem (source_file).
    unsigned char byte = 0; // Armazena temporariamente um byte que será preenchido com os bits codificados.
    char *bitshuff = NULL; // Aponta para a string de bits correspondente a byteread na tabela de hash.
    int bit_index = 7; // Controla a posição dos bits em byte (de 7 a 0, correspondente a cada bit do byte).
    int i;
    rewind(source_file); // Garante que o indicador esteja no início do arquivo.

    while (fscanf(source_file, "%c", &byteread) > 0) {
        bitshuff = get_char_new_map(ht, byteread); // A função lê cada caractere (byteread) do source_file e obtém o mapeamento de bits correspondente de ht usando get_char_new_map.
        for (i = 0; i < strlen(bitshuff); ++i) {
            if (bit_index == -1) {
                fprintf(compressed_file, "%c", byte);
                byte = 0;
                bit_index = 7;
            }

            if (bitshuff[i] == '1') {
                byte = set_bit(byte, bit_index);
            }
            bit_index--;
        }
    }
    if (bit_index <= 7) { // Se tiver bits pendentes, ele é escrito.
        fprintf(compressed_file, "%c", byte);
    }
    bit_index++;
    return bit_index;
}

void fprint_trash_and_tree_size(FILE *file, char *bits) { // char *bits: string de 16 caracteres (0s e 1s) que representa o número de bits de lixo e o tamanho da árvore
    // Esta função escreve os bits de lixo e o tamanho da árvore no cabeçalho do arquivo comprimido.
    int i;
    unsigned char byte = 0;
    int bit_index = 7;

    for (i = 0; i < 16; i++) {
        if (bits[i] != '0') {
            byte = set_bit(byte, bit_index);
        }
        bit_index--;
        if (bit_index < 0) {
            bit_index = 7;
            fprintf(file, "%c", byte);
            byte = 0;
        }
    }
}

void compress(char *source_file_name, char *destination_file_name) {
    // Esta é a função principal que realiza a compressão do arquivo.
    FILE *source_file, *compressed_file;
    hash_table *hasht = create_hash_table();
    node  *queue = create_empty_queue();
    node  *huffman_tree = create_empty_tree();
    unsigned int i;

    source_file = fopen(source_file_name, "rb");
    compressed_file = fopen(destination_file_name, "wb");

    if (source_file == NULL || compressed_file == NULL){
        printf("\n\tCaminho do arquivo incorreto.\n");
        return;
    }

    printf("\n\tComprimindo ...\n\n");

    freq_count(source_file, hasht); // Conta a frequência de cada caractere e armazena na tabela de hash.

    for(i = 0; i < 256; i++){ // Enfileira cada item com frequência maior que zero na fila de prioridade.
        if(hasht->table[i]->frequency > 0){
            queue = enqueue(queue, &i, hasht->table[i]->frequency);
        }
    }

    huffman_tree = huff_tree(queue); // Transforma a fila de prioridade em uma árvore de Huffman.

    unsigned char bit_string[256];
    pass_through_edges_and_add_characters(hasht, huffman_tree, bit_string, -1, '0'); // Atualiza o "bit_string" na tabela de hash com o novo mapeamento de bytes.

    fprintf(compressed_file, "00"); // Reserva os primeiros 16 bits.

    unsigned int tree_size = size_huff(huffman_tree) + escape(huffman_tree, 0); // Total de nós + total de folhas de escape (representadas por * ou \*).
    fprint_tree_bytes_header(compressed_file, huffman_tree); // Escreve a representação pré-ordem da árvore no arquivo.

    char *tree_header_size = (char*)malloc(13*sizeof(char));
    int_bin(tree_header_size, tree_size, 13);

    unsigned int trash = write_compressed_file(source_file, compressed_file, hasht);

    char *trash_size = (char*)malloc(4*sizeof(char));
    int_bin(trash_size, trash, 3);
    trash_size[3] = '\0';

    // Construção do cabeçalho: 3 bits (tamanho do lixo) e 13 bits (tamanho da árvore de Huffman)
    char header[17] = "";
    strcpy(header, trash_size);
    header[3] = '\0';
    strcat(header, tree_header_size);
    header[16] = '\0';

    rewind(compressed_file);
    fprint_trash_and_tree_size(compressed_file, header);

    fclose(source_file);
    fclose(compressed_file);

    printf("\n\tFim da compressão!\n\n");
}