#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 256
char huffman_arr[MAX] = {0};

typedef struct Node {
	unsigned char data;
	int fw;
	struct Node *lchild, *rchild;
} Node;

typedef struct Trie {
	int flag;
	struct Trie* next[2];
} Trie;

#define swap(x, y) {   \
	__typeof(x) temp = x;\
	x = y;   \
	y = temp;   \
}

Trie* get_trie_node() {
	Trie* trie = (Trie*)malloc(sizeof(Trie));
	trie->flag = 0;
	trie->next[0] = trie->next[1] = NULL;
	return trie;
}

Node* get_new_node() {
	Node* node = (Node*)malloc(sizeof(Node));
	node->rchild = node->lchild = NULL;
	return node;
}

Node* build(Node** node, int n) {
#define hswap(x, y){ \
	swap(node[x], node[y])  \
}    

    Node* root = NULL;
	for (int i = 0; i < n - 1; i++) {
		if (node[i] == NULL) continue;
		int ind1 = i, ind2 = i + 1;
		if (node[ind1]->fw > node[ind2]->fw) hswap(ind1, ind2)
		for (int j = ind2 + 1; j < n; j++) {
			if (node[j]->fw < node[ind1]->fw) {
				hswap(j, ind1)
				hswap(j, ind2);
			}
			else if (node[j]->fw < node[ind2]->fw)
			{
				hswap(j, ind2);
			}
		}
		
		root = get_new_node();
		root->rchild = node[ind1];
		root->lchild = node[ind2];
		root->fw = node[ind1]->fw + node[ind2]->fw;
		root->data = 0;
		node[ind2] = root;
	}

    return root;
#undef hswap
}

void encode(Node* root, int k, unsigned char* code, unsigned char** huffmanTable) {
	if (root == NULL) return;
	if (root->lchild == NULL && root->rchild == NULL) {
		code[k] = '\0';
		memcpy(huffmanTable[root->data], code, k);
		return;
	}
	code[k] = '0';
	encode(root->lchild, k + 1, code, huffmanTable);
	code[k] = '1';
	encode(root->rchild, k + 1, code, huffmanTable);
}

void DFS(Node* root, unsigned char** huffmanTable) {
	unsigned char* code = (unsigned char*)calloc(256, sizeof(char));
	encode(root, 0, code, huffmanTable);
	int cnt = 0;
	int total = 0;
	printf("Huffman code:\n");
	for (int i = 0; i < 256; i++) {
		if (!huffmanTable[i][0]) continue;
		printf("%d: ", i);
		total += printf("%s\n", huffmanTable[i]);
		cnt++;
	}
	printf("averge: %d\n", total / cnt);
	free(code);
}

void insert(unsigned char** HuffmanTable, unsigned char* str, Trie* trie) {
	char code[256];
	bool flag = false;
	while (str[0]) {
		flag = false;
		unsigned char* code = HuffmanTable[str[0]];
		for (int i = 0; code[i]; i++) {
			if (!trie->next[code[i] - '0']) trie->next[code[i] - '0'] = get_trie_node(); 
			trie = trie->next[code[i] - '0'];
		}	
		str++;
	}
	trie->flag = 1;
}

int search(unsigned char** HuffmanTable, unsigned char* str, Trie* trie) {
	bool flag = false;
	Trie* t = trie;
	while (str[0]) {
		flag = false;
		unsigned char* code = HuffmanTable[str[0]];
		for (int i = 0; code[i]; i++) {
			if (!trie->next[code[i] - '0']) {
				trie = t;
				break;
			}
			if (trie->next[code[i] - '0']->flag) return 1;
			trie = trie->next[code[i] - '0'];
		}
		str++;
	}
	return 0;
}

void clear_trie(Trie* trie) {
	if (trie == NULL) return;
	clear_trie(trie->next[0]);
	clear_trie(trie->next[1]);
	free(trie);
}

void clear_huffman_tree(Node **node) {
	for (int i = 0; i < MAX; i++) {
		if (!node[i]) continue;
		free(node[i]);
	}
	free(node);
}

void clear_huffman_table(unsigned char** table) {
	for (int i = 0; i < MAX; i++) {
		free(table[i]);
	}
	free(table);
}

int main() {
	unsigned char str[1000];
	scanf("%[^\n]s", str);
	Node** node = (Node**)malloc(sizeof(Node*) * MAX);
	for (int i = 0; str[i]; i++) {
		huffman_arr[str[i]]++;
	}

	int ind = 0;
	for (int i = 0; i < MAX; i++) {
		node[i] = NULL;
		if (huffman_arr[i] == 0) continue;
		node[ind] = get_new_node();
		node[ind]->fw = huffman_arr[i];
		node[ind]->data = (unsigned char)i;
		ind++;
	}

	Node *root = build(node, ind);
    unsigned char**HuffmanTable = (unsigned char**)malloc(sizeof(unsigned char*) * MAX);
	for (int i = 0; i < MAX; i++) {
		HuffmanTable[i] = (unsigned char*)calloc(MAX, sizeof(unsigned char));
	}
	DFS(root, HuffmanTable);
	clear_huffman_tree(node);
	Trie* trie = get_trie_node();
	int n, m;
	scanf("%d%d", &n, &m);
	while (n--) {
		getchar();
		scanf("%[^\n]s", str);
		insert(HuffmanTable, str, trie);
	}
	printf("m = %d \n", m);
	while (m--) {
		getchar();
		scanf("%[^\n]s", str);
		printf("result: %d\n", search(HuffmanTable, str, trie));
	}
	clear_trie(trie);
	//clear_huffman_tree(root);
	clear_huffman_table(HuffmanTable);
	return 0;
}
