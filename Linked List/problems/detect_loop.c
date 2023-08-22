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
ll_create(unsigned int data_size)
{
    linked_list_t* list = calloc(1, sizeof(linked_list_t));
    list->data_size = data_size;
    return list;
}

ll_node_t*
ll_get_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t* node = list->head;

    if (n >= list->size)
		n = list->size - 1;

    for (unsigned int i = 0; i < n; i++)
        node = node->next;

    return node;
}

ll_node_t*
node_create(linked_list_t* list, const void* new_data)
{
    ll_node_t* new_node = (ll_node_t *)malloc(sizeof(ll_node_t));
    DIE(!new_node, "malloc() failed\n");
    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, new_data, list->data_size);
    return new_node;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, ignorati comanda (iesiti din functie).
 */
void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    // corner cases
    DIE(!list, "List needs to be created first\n");

    // n < 0 is covered by unsigned

    if (n >= list->size)
        n = list->size;

    ll_node_t* new_node = node_create(list, new_data);

    if (n == 0) {
        new_node->next = list->head;
        list->head = new_node;
    } else {
        ll_node_t* prev = ll_get_nth_node(list, n - 1);
        new_node->next = prev->next;
        prev->next = new_node;
    }

    list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, ignorati comanda (iesiti din functie).
 * Functia intoarce un pointer spre acest nod proaspat eliminat din lista.
 * Este responsabilitatea apelantului sa elibereze memoria acestui nod.
 */
ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    // corner cases
    DIE(!list, "List needs to be created first\n");

    // n < 0 is covered by unsigned

    if (n >= list->size - 1)
        n = list->size - 1;

    ll_node_t* rem_node = NULL;
    if (n == 0) {
        rem_node = list->head;
        list->head = rem_node->next;
    } else {
        ll_node_t* prev = ll_get_nth_node(list, n - 1);
        rem_node = prev->next;
        prev->next = rem_node->next;
    }

    list->size--;

    return rem_node;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
ll_get_size(linked_list_t* list)
{
    if (!list)
        return -1;
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void
ll_free(linked_list_t** pp_list)
{
    DIE(!(*pp_list), "List needs to be created first\n");

    while ((*pp_list)->size > 0) {
        ll_node_t* node = ll_remove_nth_node(*pp_list, 0);
        free(node->data);
        free(node);
    }

    free((*pp_list));
    pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void
ll_print_int(linked_list_t* list)
{
    ll_node_t* node = list->head;

    while (node) {
        printf("%d ", *(int *)node->data);
        node = node->next;
    }

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void
ll_print_string(linked_list_t* list)
{
    ll_node_t* node = list->head;


    while (node) {
        printf("%s ", (char *)node->data);
        node = node->next;
    }

    printf("\n");
}

/*
 * Procedura verifica daca o lista simplu inlantuita primita ca parametru
 * contine un ciclu sau nu. In caz afirmativ, returneaza 1, altfel 0.
 */
int has_cycle(linked_list_t* list)
{
    ll_node_t *node1 = list->head;
    ll_node_t *node2 = list->head;

    while (node2 && node2->next) {
        node1 = node1->next;
        node2 = node2->next->next;
        if (node1 == node2)
            break;
    }

    if (!node2 || !node2->next)
        return 0;
    
    return 1;
}

int main()
{
    linked_list_t* linkedList;

    while (1) {
        char command[16], added_elem[64], *end_ptr;
        long size, pos;

        scanf("%s", command);

        if (strcmp(command, "head") == 0) {
            linkedList = ll_create(sizeof(int));
            scanf("%ld", &size);

            long int curr_nr;
            for (int i = 0; i < size; ++i) {
                scanf("%ld", &curr_nr);
                ll_add_nth_node(linkedList, size, &curr_nr);
            }
        }

        if (strcmp(command, "pos") == 0) {
            scanf("%ld", &pos);
            
            if (pos >= 0) {
                ll_node_t* curr_node = linkedList->head;
                ll_node_t* cycle_pos = linkedList->head;

                for (int i = 0; i < size - 1; ++i) {
                    if (i == pos) {
                        cycle_pos = curr_node;
                    }
                    curr_node = curr_node->next;
                }

                curr_node->next = cycle_pos;
            }

            if (has_cycle(linkedList) == 1) {
                printf("true\n");
            } else {
                printf("false\n");
            }
            
            break;
        }
    }

    ll_free(&linkedList);

    return 0;
}
