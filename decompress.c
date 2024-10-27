node* add_node(void *value, node *left, node *right) {
    node *new_node = (node*) malloc(sizeof(node));
    unsigned char *new_node_unsigned_char = (unsigned char*) malloc(sizeof(unsigned char));
    *new_node_unsigned_char = *(unsigned char*) value;
    new_node->item = new_node_unsigned_char;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}

node* build_tree(char *str_tree, int *pos) { // Reconstroe a árvore de Huffman a partir da string da árvore
    if (str_tree[*pos] == '*') {
        *pos = *pos + 1;
        node *left = build_tree(str_tree, pos);
        *pos = *pos + 1;
        return add_node(&character, left, build_tree(str_tree, pos));
    } else if (str_tree[*pos] == '\\') {
        *pos = *pos + 1;
        return add_node(&str_tree[*pos], NULL, NULL); // Se for uma folha com "*" ou "\"
    }
    return add_node(&str_tree[*pos], NULL, NULL); // Se for uma folha
}

int is_bit_i_set(unsigned char c, int i) {
    unsigned char mask = 1 << i;
    return mask & c;
} // Verifica se o bit 1 está setado (definido) na posição i

int bin_int(char *bin, int size) { // Tansforma binário em inteiro
    int value = 0;
    while (size > 0) {
        if (*bin == '1') {
            value += pow(2, (size - 1)); // Retorna o valor do espoente, no caso 2 ^ size - 1
        }
        size--;
        bin = bin + 1;
    }
    return value;
} 

void decompress(char *source_file_name, char* destination_file_name) {
    int i, j, pos;
    FILE *compressed_file, *new_file;
    int total_bytes, size_trash, size_tree;
    unsigned char *buffer;
    char header[17];

    compressed_file = fopen(source_file_name, "rb");
    new_file = fopen(destination_file_name, "wb");

    if (compressed_file == NULL || new_file == NULL) {
        printf("\n\tWrong file path.\n");
        return;
    }

    printf("\n\tDecompressing ...\n");

    fseek(compressed_file, 0, SEEK_END); // Ajusta o indicador para o fim do arquivo
    total_bytes = ftell(compressed_file); // Retorna o valor total de bytes do arquivo
    rewind(compressed_file); // Volta o indicador para o início do arquivo
    buffer = (unsigned char*) malloc(sizeof(char) * total_bytes); // Aloca memória com o valor do arquivo
    fread(buffer, 1, total_bytes, compressed_file); // Copia todos os caracteres de "file" para o "buffer"

    for (i = 0; i < 17; i++) {
        header[i] = '\0';
    }

    // Pega a primeira e segunda parte do header (os dois bytes iniciais)
    for (i = 0; i < 2; i++) {
        for (j = 7; j >= 0; j--) { // 7 é o último index do byte da direita para esquerda
            if (!is_bit_i_set(buffer[i], j)) {
                header[strlen(header)] = '0'; // O valor da string aumenta
            } else {
                header[strlen(header)] = '1';
            }
        }
    }

    size_trash = bin_int(header, 3);
    size_tree = bin_int(header + 3, 13);

    // Pegando a terceira parte do header (árvore em pré-ordem)
    char str_tree[size_tree + 1]; // O +1 é por conta do '\0' (final de string)

    // Pegando cada pré-ordem byte/character. i = 2 por conta dos 2 bytes iniciais que já foram lidos
    for (i = 2, pos = 0; i <= size_tree + 1; i++, pos++) {
        fseek(compressed_file, i, SEEK_SET);
        str_tree[pos] = getc(compressed_file);
    }
    str_tree[pos] = '\0';
    pos = 0;

    node *root_huff = build_tree(str_tree, &pos);

    node *aux_tree = root_huff; // Árvore reconstruida

    // O tamanho da árvore + 2; ele agora vai começar a ler o texto codificado
    fseek(compressed_file, size_tree + 2, SEEK_SET); // é +2 porque 2 é a quantidade de bytes reservados para a trash e tree value
    // Comecando a ir de bit em bit buscando uma folha na arvore
	// Ate (o total de bytes) - (o que eu ja li)) - (o byte de lixo)
	unsigned int bit_cur = 0; //bit atual
	for(i = 0; i < (total_bytes - (size_tree + 2) - 1); i++){

		bit_cur = getc(compressed_file);
		for(j = 7; j >= 0; j--){

			if(is_bit_i_set(bit_cur, j)){
				aux_tree = aux_tree->right;
			}
			else {
				aux_tree = aux_tree->left;
			}

			if(is_leaf(aux_tree)){

				fprintf(new_file, "%c", *(unsigned char*)aux_tree->item);
				aux_tree = root_huff;
			}
		}
	}

	// Ultimo byte (trabalhado com o lixo)
	bit_cur = getc(compressed_file);
	for(j = 7; j >= size_trash; j--){

		if(is_bit_i_set(bit_cur, j)){
			aux_tree = aux_tree->right;
		}else{
			aux_tree = aux_tree->left;
		}
		if(is_leaf(aux_tree)){

			fprintf(new_file, "%c", *(unsigned char*)aux_tree->item);
			aux_tree = root_huff;
		}
	}

	fclose(compressed_file);
	fclose(new_file);

	printf("\n\tSuccess!\n\n");
}
