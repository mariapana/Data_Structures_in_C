#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
	/* Value associated with key (set if end_of_word = 1) */
	void* value;

	/* 1 if current node marks the end of a word, 0 otherwise */
	int end_of_word;

	trie_node_t** children;
	int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
	trie_node_t* root;
	
	/* Number of keys */
	int size;

	/* Generic Data Structure */
	int data_size;

	/* Trie-Specific, alphabet properties */
	int alphabet_size;
	char* alphabet;

	/* Callback to free value associated with key, should be called when freeing */
	void (*free_value_cb)(void*);

	/* Optional - number of nodes, useful to test correctness */
	int nNodes;
};

trie_node_t* trie_create_node(trie_t * trie) {
	trie_node_t *node = malloc(sizeof(trie_node_t));
	DIE(!node, "malloc() failed\n");

	node->end_of_word = 0;
	node->n_children = 0;

	node->children = calloc(trie->alphabet_size, sizeof(trie_node_t *));
	DIE(!node->children, "calloc() failed\n");

	return node;
}

trie_t* trie_create(int data_size, int alphabet_size, char* alphabet,
					void (*free_value_cb)(void*)) {
	trie_t *trie = malloc(sizeof(trie_t));
	DIE(!trie, "malloc() failed\n");

	trie->size = 1;
	trie->data_size = data_size;
	trie->alphabet_size = alphabet_size;

	trie->alphabet = malloc(alphabet_size * sizeof(char) + 1);
	DIE(!trie->alphabet, "malloc() failed\n");
	strcpy(trie->alphabet, alphabet);

	trie->free_value_cb = free_value_cb;
	trie->nNodes = 1;

	trie->root = trie_create_node(trie);
	trie->root->value = calloc(1, trie->data_size);
	DIE(!trie->root->value, "calloc() failed\n");

	int marker = -1;
	memcpy(trie->root->value, &marker, data_size);

	trie->root->end_of_word = 1;
	trie->root->n_children = 0;

	return trie;
}

void trie_insert_recursion(trie_t* trie, trie_node_t* node, char* key,
						   void* value) {
	if (*key == '\0') {
		if (!node->end_of_word) {
			node->value = calloc(1, trie->data_size);
			DIE(!node->value, "calloc() failed\n");
		}

		node->end_of_word = 1;
		memcpy(node->value, value, trie->data_size);
		return;
	}

	int index = *key - 'a';
	if (!node->children[index]) {
		node->children[index] = trie_create_node(trie);
		node->n_children++;
		(trie->nNodes)++;
	}

	trie_insert_recursion(trie, node->children[index], key + 1, value);
}

void trie_insert(trie_t* trie, char* key, void* value) {
	trie_insert_recursion(trie, trie->root, key, value);
}

void* trie_search_recursion(trie_t* trie, trie_node_t* node, char* key) {
	if (*key == '\0') {
		if (node->end_of_word)
			return node->value;
		else
			return NULL;
	}
	
	int index = *key - 'a';
	if (!node->children[index]) {
		return NULL;
	}

	return trie_search_recursion(trie, node->children[index], key + 1);
}

void* trie_search(trie_t* trie, char* key) {
	return trie_search_recursion(trie, trie->root, key);
}

int trie_remove_recursion(trie_t *trie, trie_node_t *node, char *key) {
	if (*key == '\0') {
		if (node->end_of_word) {
			trie->free_value_cb(node->value);
			(trie->size)--;
			node->end_of_word = 0;
			return node->n_children == 0;
		}

		return 0;
	}

	int index = *key - 'a';
	if (node->children[index] &&
		trie_remove_recursion(trie, node->children[index], key + 1)) {
		free(node->children[index]->children);
		free(node->children[index]);
		(trie->nNodes)--;
		(node->n_children)--;
		node->children[index] = NULL;

		if (!node->n_children && !node->end_of_word)
			return 1;
	}

	return 0;
}

void trie_remove(trie_t* trie, char* key) {
	if (trie_remove_recursion(trie, trie->root, key))
		return;
}

void trie_free_recursion(trie_t *trie, trie_node_t **node) {
	for (int i = 0; i < trie->alphabet_size; i++)
        if ((*node)->children[i])
            trie_free_recursion(trie, &((*node)->children[i]));

    if ((*node)->end_of_word)
        trie->free_value_cb((*node)->value);

    free((*node)->children);
    free(*node);
}

void trie_free(trie_t** pTrie) {
	trie_free_recursion(*pTrie, &(*pTrie)->root);
	free((*pTrie)->alphabet);
	free(*pTrie);
}   

/* Needed for Lambda tests, ignore :) */
void cleanup_example_string(char* str) {
	int len = strlen(str);
	
	if(str[len-2] == '\\') {
		str[len-2] = '\0';
	}
}

int main() {
	int n, value;
	char alphabet[] = ALPHABET;
	char buf[256], key[256], op;
	
	trie_t* trie = trie_create(sizeof(int), ALPHABET_SIZE, alphabet, free);
	
	fgets(buf, 256, stdin);
	sscanf(buf, "%d\n", &n);

	for(int i = 0; i < n; ++i) {
		fgets(buf, 256, stdin);
		sscanf(buf, "%c", &op);

		if(op == 'i') {
			sscanf(buf, "%c %s %d\n", &op, key, &value);
			trie_insert(trie, key, &value);
		} else if(op == 'r') {
			sscanf(buf, "%c %s\n", &op, key);
			cleanup_example_string(key);
			printf("nNodes before removing %s: %d\n", key, trie->nNodes);
			trie_remove(trie, key);
			printf("nNodes after removing %s: %d\n", key, trie->nNodes);
		} else if(op == 's') {
			sscanf(buf, "%c %s\n", &op, key);
			cleanup_example_string(key);

			if(key[0] == '_') {
				key[0] = '\0';
			}

			int* found = trie_search(trie, key);
			printf("%s: ", key[0] == '\0' ? "_" : key);

			if(found) {
				printf("%d\n", *found);
			} else {
				printf("not found\n");
			}
		}
	}

	trie_free(&trie);

	return 0;
}
