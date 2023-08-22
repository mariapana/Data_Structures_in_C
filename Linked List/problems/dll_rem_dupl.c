#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STRING_SIZE 64

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

// Structs
typedef struct node_t node_t;
struct node_t {
	void *data;
	node_t *prev;
	node_t *next;
};

typedef struct {
	node_t *head;
	unsigned int data_size;
	unsigned int size;
} dll_t;

dll_t *dll_create(unsigned int data_size)
{
	dll_t *list = malloc(sizeof(dll_t));
	DIE(!list, "dll_create(): malloc() failed\n");
	list->data_size = data_size;
	list->head = NULL;
	list->size = 0;
	return list;
}

node_t *node_create(unsigned int data_size, const void *data)
{
	node_t *new_node = malloc(sizeof(node_t));
	DIE(!new_node, "node_create(): malloc() failed\n");

	new_node->next = NULL;
	new_node->prev = NULL;

	new_node->data = malloc(data_size);
	DIE(!new_node, "malloc() failed\n");
	memcpy(new_node->data, data, data_size);

	return new_node;
}

node_t *dll_get_nth_node(dll_t *list, unsigned int n)
{
	DIE(!list, "List needs to be created first\n");

	DIE(list->size == 0, "List should contain at least one node\n");

	if (n >= list->size)
		n = n % list->size;

	node_t *node = list->head;

	if (!n)
		return node;

	for (unsigned int i = 0; i < n; i++)
		node = node->next;

	return node;
}

void dll_add_nth_node(dll_t *list, unsigned int n, const void *data)
{
	DIE(!list, "List needs to be created first\n");

	if (n >= list->size)
		n = list->size;

	node_t *new_node = node_create(list->data_size, data);

	if (list->size == 0) {
		list->head = new_node;
		new_node->next = NULL;
		new_node->prev = list->head;
	} else {
		if (n == 0) {
			new_node->next = list->head;
			new_node->prev = NULL;
			list->head = new_node;
		} else {
			node_t *prev_node = dll_get_nth_node(list, n - 1);
			new_node->prev = prev_node;
			new_node->next = prev_node->next;
			prev_node->next = new_node;
			if (n != list->size)
				new_node->next->prev = new_node;
		}
	}

	list->size++;
}

node_t *dll_remove_nth_node(dll_t *list, unsigned int n)
{
	DIE(!list, "List needs to be created first\n");
	DIE(!list->size, "List should contain at least one node\n");

	if (n >= list->size - 1)
		n = list->size - 1;

	node_t *rem_node = dll_get_nth_node(list, n);
	node_t *right = rem_node->next;
	node_t *left = rem_node->prev;

	if (!list->size) {
		list->head = NULL;
		list->size--;
		return rem_node;
	}

	if (!n) {
		list->head = right;
		list->size--;
		return rem_node;
	}

	if (left)
		left->next = right;
	if (right)
		right->prev = left;

	list->size--;
	return rem_node;
}

void dll_free(dll_t **list)
{
	DIE(!(*list), "List needs to be created first\n");

	while ((*list)->size) {
		node_t *node = dll_remove_nth_node(*list, 0);
		free(node->data);
		free(node);
	}

	free(*list);
	list = NULL;
}

/* 
 * Aceasta functie elimina duplicatele dintr-o lista circulara dublu inlantuita,
 * primita ca parametru, pastrand un singur nod din fiecare grup de duplicate.
 */
void remove_duplicates(dll_t* list)
{
    if (!list || list->size < 2)
        return;

    node_t *node1, *node2;
    node1 = list->head;
    node2 = list->head->next;
    int pos2 = 1;

    while (node2) {
        int val1 = *(int *)node1->data;
        int val2 = *(int *)node2->data;

        if (val1 == val2) {
			node2 = node2->next;
            node_t* rem_node = dll_remove_nth_node(list, pos2);
			free(rem_node->data);
			free(rem_node);
        } else {
            pos2++;
			node1 = node1->next;
			node2 = node2->next;
        }
    }
}

void
dll_print_ints_right_circular(node_t* start)
{
	node_t *aux = start;
	do {
		printf("%d ", *((int*)aux->data));
		aux = aux->next;
	} while (aux != start && aux);
	printf("\n");
}

void
dll_print_int_list(dll_t* list)
{
	dll_print_ints_right_circular(list->head);
}

int main()
{
    dll_t* list;

    char command[16], added_elem[64], *end_ptr;
    long size;

    list = dll_create(sizeof(int));
    scanf("%ld", &size);

    long int curr_nr;
    for (int i = 0; i < size; ++i) {
        scanf("%ld", &curr_nr);
        dll_add_nth_node(list, size, &curr_nr);
    }

    remove_duplicates(list);

    dll_print_int_list(list);

    dll_free(&list);

    return 0;
}