#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 256
char huffman_arr[MAX] = {0};

typedef struct Node
{
	unsigned char data;
	int fw;
	struct Node *lchild, *rchild;
} Node;

typedef struct Trie
{
	int flag;
	struct Trie* next[2];
} Trie;

#define swap(x, y){   \
	__typeof(x) temp = x;\
	x = y;   \
	y = temp;   \
}

Trie* get_trieNode()
{
	Trie* trie = (Trie*)malloc(sizeof(Trie));
	trie->flag = 0;
	trie->next[0] = trie->next[1] = NULL;
	return trie;
}

Node* get_newNode()
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->rchild = node->lchild = NULL;
	return node;
}

Node* build(Node** node, int n)
{
#define hswap(x, y){ \
	swap(node[x], node[y])  \
}    

    Node* root = NULL;
	for (int i = 0; i < n - 1; i++)
	{
		if (node[i] == NULL) continue;
		int ind1 = i, ind2 = i + 1;
		if (node[ind1]->fw > node[ind2]->fw) hswap(ind1, ind2)
		for (int j = ind2 + 1; j < n; j++)
		{
			if (node[j]->fw < node[ind1]->fw)
			{
				hswap(j, ind1)
				hswap(j, ind2);
			}
			else if (node[j]->fw < node[ind2]->fw)
			{
				hswap(j, ind2);
			}
		}
		
		root = get_newNode();
		root->rchild = node[ind1];
		root->lchild = node[ind2];
		root->fw = node[ind1]->fw + node[ind2]->fw;
		root->data = 0;
		node[ind2] = root;
	}

    return root;
#undef hswap
}

void encode(Node* root, unsigned char str, int k, char* code, bool* flag)
{
	if (root == NULL) return;
	if (*flag) return;
	if (root->data == str)
	{
		code[k] = '\0';
		*flag = 1;
		return ;
	}
	code[k] = '0';
	encode(root->lchild, str, k + 1, code, flag);
	if (*flag) return;
	code[k] = '1';
	encode(root->rchild, str, k + 1, code, flag);
}

void insert(Node* root, unsigned char* str, Trie* trie)
{
	char code[256];
	bool flag = false;
	while (str[0])
	{
		flag = false;
		encode(root, str[0], 0, code, &flag);
		for (int i = 0; code[i]; i++)
		{
			if (!trie->next[code[i] - '0']) trie->next[code[i] - '0'] = get_trieNode(); 
			trie = trie->next[code[i] - '0'];
		}	
		str++;
	}
	trie->flag = 1;
}

int search(Node* root, unsigned char* str, Trie* trie)
{
	char code[256];
	bool flag = false;
	Trie* t = trie;
	while (str[0])
	{
		flag = false;
		encode(root, str[0], 0, code, &flag);
		for (int i = 0; code[i]; i++)
		{
			//if (trie->next[code[i] - '0']->flag) return 1;
			if (!trie->next[code[i] - '0'])
			{
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

int main()
{
	unsigned char str[1000];
	scanf("%[^\n]s", str);
	Node** node = (Node**)malloc(sizeof(Node*) * 256);
	for (int i = 0; str[i]; i++)
	{
		huffman_arr[str[i]]++;
	}

	int ind = 0;
	for (int i = 0; i < 256; i++)
	{
		if (huffman_arr[i] == 0) continue;
		node[ind] = get_newNode();
		node[ind]->fw = huffman_arr[i];
		node[ind]->data = (unsigned char)i;
		ind++;
	}

	Node *root = build(node, ind);
	Trie* trie = get_trieNode();
	int n, m;
	scanf("%d%d", &n, &m);
	while (n--) {
		getchar();
		scanf("%[^\n]s", str);
		insert(root, str, trie);
	}
	printf("m = %d \n", m);
	while (m--) {
		getchar();
		scanf("%[^\n]s", str);
		printf("result: %d\n", search(root, str, trie));
	}
	return 0;
}
