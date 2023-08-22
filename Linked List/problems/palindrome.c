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

void
reverse(dll_t* list)
{
    if (!list || !list->head)
        return;

    // Luam 3 pointeri: prev, curr, next
    node_t* prev = list->head;
    node_t* curr = list->head->next;
    node_t* next = NULL;

    // Intorc capatul (prev) catre NULL
    prev->next = NULL;

    while (curr) {
        // Get next node
        next = curr->next;

        // Reverse pointer
        curr->next = prev;

        // Move
        prev = curr;
        curr = next;
    }

    // Change head
    list->head = prev;
}

int is_equal(dll_t* list1, dll_t* list2)
{
    if (list1->size != list2->size)
        return 0;

    node_t *iterator1 = list1->head;
    node_t *iterator2 = list2->head;

    while (iterator1 && iterator2) {
        if (iterator1->data != iterator2->data)
            return 0;
        iterator1 = iterator1->next;
        iterator2 = iterator2->next;
    }

    return 1;
}

/* 
 * Aceasta functie printeaza "YES", daca lista primita ca parametru stocheaza
 * un palindrom si "NO" in caz contrar.
 */
void is_palindrome(dll_t* list)
{
    node_t *node1 = list->head;
    node_t *node2 = list->head;
    while (node2->next)
        node2 = node2->next;

    for (unsigned int i = 0; i < list->size / 2; i++) {
        if (*(char *)(node1->data) != *(char *)(node2->data)) {
            printf("NO\n");
            return;
        }
        node1 = node1->next;
        node2 = node2->prev;
    }

    printf("YES\n");
}

int main()
{
    dll_t* list;

    char command[16], added_elem[64], *end_ptr;
    long size;

    list = dll_create(sizeof(char));
    scanf("%ld", &size);

    char curr_char;
    scanf("%c", &curr_char);
    for (int i = 0; i < size; ++i) {
        scanf("%c", &curr_char);
        dll_add_nth_node(list, size, &curr_char);
    }

    is_palindrome(list);

    dll_free(&list);

    return 0;
}