#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DIE(assertion, call_description)                \
    do {                                \
        if (assertion) {                    \
            fprintf(stderr, "(%s, %d): ",           \
                    __FILE__, __LINE__);        \
            perror(call_description);           \
            exit(errno);                        \
        }                           \
    } while (0)

#define MAX_STRING_SIZE 64

typedef struct ll_node_t
{
    void* data;
    struct ll_node_t* next;
} ll_node_t;

typedef struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
} linked_list_t;

linked_list_t*
ll_create(unsigned int data_size);

ll_node_t*
ll_get_nth_node(linked_list_t* list, unsigned int n);

ll_node_t*
node_create(linked_list_t* list, const void* new_data);

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data);

ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n);

unsigned int
ll_get_size(linked_list_t* list);

void
ll_free(linked_list_t** pp_list);

void
ll_print_int(linked_list_t* list);

void
ll_print_string(linked_list_t* list);
