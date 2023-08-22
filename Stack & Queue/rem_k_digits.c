#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

#define MAX_STRING_SIZE 64

typedef struct ll_node_t ll_node_t;
struct ll_node_t
{
    void* data;
    ll_node_t* next;
};

typedef struct linked_list_t linked_list_t;
struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
};

linked_list_t*
ll_create(unsigned int data_size)
{
    linked_list_t* ll;

    ll = malloc(sizeof(linked_list_t));
    DIE(ll == NULL, "linked_list malloc");

    ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;

    return ll;
}


void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *prev, *curr;
    ll_node_t* new_node;

    if (list == NULL) {
        return;
    }

    if (n > list->size) {
        n = list->size;
    } else if (n < 0) {
        return;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(ll_node_t));
    DIE(new_node == NULL, "new_node malloc");
    new_node->data = malloc(list->data_size);
    DIE(new_node->data == NULL, "new_node->data malloc");
    memcpy(new_node->data, new_data, list->data_size);

    new_node->next = curr;
    if (prev == NULL) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }

    list->size++;
}

ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;

    if (list == NULL) {
        return NULL;
    }

    if (list->head == NULL) {
        return NULL;
    }

    if (n > list->size - 1) {
        n = list->size - 1;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (prev == NULL) {
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    list->size--;

    return curr;
}


unsigned int
ll_get_size(linked_list_t* list)
{
    if (list == NULL) {
        return -1;
    }

    return list->size;
}


void
ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;

    if (pp_list == NULL || *pp_list == NULL) {
        return;
    }

    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        free(currNode);
    }

    free(*pp_list);
    *pp_list = NULL;
}

void
ll_print_int(linked_list_t* list)
{
    ll_node_t* curr;

    if (list == NULL) {
        return;
    }

    curr = list->head;
    while (curr != NULL) {
        printf("%d ", *((int*)curr->data));
        curr = curr->next;
    }

    printf("\n");
}

void
ll_print_string(linked_list_t* list)
{
    ll_node_t* curr;

    if (list == NULL) {
        return;
    }

    curr = list->head;
    while (curr != NULL) {
        printf("%s ", (char*)curr->data);
        curr = curr->next;
    }

    printf("\n");
}

ll_node_t*
get_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t* node = list->head;

    for (unsigned int i = 0; i < n; i++)
        node = node->next;

    return node;
}


/* ---------------------------------- TODO: Stack implementation ----------------------------------------*/

struct Stack {
    linked_list_t *list;
};

void init_stack(struct Stack *stack, unsigned int data_size) {
    stack->list = ll_create(data_size);
}

int get_size_stack(struct Stack *stack) {
    return stack->list->size;
}

int is_empty_stack(struct Stack *stack) {
    return get_size_stack(stack) ? 0 : 1;
}

void* peek_stack(struct Stack *stack) {
    ll_node_t* node = get_nth_node(stack->list, 0);
    return node->data;
}

void pop_stack(struct Stack *stack) {
    ll_node_t *removed = ll_remove_nth_node(stack->list, 0);
	free(removed->data);
	free(removed);
}

void push_stack(struct Stack *stack, void *new_data) {
    ll_add_nth_node(stack->list, 0, new_data);
}

void clear_stack(struct Stack *stack) {
    while (!is_empty_stack(stack))
        pop_stack(stack);
}

void purge_stack(struct Stack *stack) {
    ll_free(&stack->list);
    free(stack->list);    
}

/*
 * Functie care afla cel mic numar posibil format
 * prin stergea a k cifre dintr-un numar primit ca input
 */

char*
remove_k_digits(char *num, int k) 
{
    struct Stack *st = malloc(sizeof(struct Stack));
    DIE(!st, "malloc() failed\n");
    init_stack(st, sizeof(char));

    int len = strlen(num);
    int prec_zeros = 0;
    int new_len = len - k;

    for (int i = 0; i < len; i++) {
        while (!is_empty_stack(st) && *(char *)peek_stack(st) > num[i] && k) {
            pop_stack(st);
            k--;
        }
        push_stack(st, &num[i]);

        if (get_size_stack(st) == 1 && *(char *)peek_stack(st) == '0') {
            pop_stack(st);
            prec_zeros++;
        }
    }

    while (!is_empty_stack(st) && k) {
        pop_stack(st);
        k--;
    }

    new_len -= prec_zeros;
    
    for (int i = new_len - 1; i >= 0 && !is_empty_stack(st); i--) {
        num[i] = *(char *)peek_stack(st);
        pop_stack(st);
    }
    num[new_len] = '\0';
    return num;
}

int main()
{   int k;
    char number[MAX_STRING_SIZE];
   
    scanf("%d", &k);

    scanf("%s", number);

    printf("%s\n", remove_k_digits(number, k));

    return 0;
}
