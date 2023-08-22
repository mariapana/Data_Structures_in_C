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
 * Procedura primeste doua liste simplu inlantuite, A si B, ale căror noduri 
 * stochează în ordine inversă cifrele câte unui număr natural reprezentat 
 * în baza 10 (primul nod al unei liste stochează cea mai puțin semnificativă
 * cifră). De exemplu: lista A: 0 -> 0 -> 3 contine numarul 300.
 * Creați o nouă listă simplu înlănțuită, C, care stochează suma celor două
 * numere si returnati lista.
 * Atentie: Lista C trebuie construită în timp ce se parcurg listele A și B!
*/
linked_list_t*
sum_lists(linked_list_t* list1, linked_list_t* list2)
{
    linked_list_t* list3 = ll_create(sizeof(int));
    ll_node_t* iterator1 = list1->head;
    ll_node_t* iterator2 = list2->head;

    int carry = 0;

    while (iterator1 && iterator2) {
        int v1 = *((int *)iterator1->data);
        int v2 = *((int *)iterator2->data);
        int sum = v1 + v2 + carry;
        carry = sum / 10;
        sum %= 10;
        ll_add_nth_node(list3, list3->size, &sum);
        iterator1 = iterator1->next;
        iterator2 = iterator2->next;
    }

    while (iterator1) {
        int v1 = *((int *)iterator1->data);
        int sum = v1 + carry;
        carry = sum / 10;
        sum %= 10;
        ll_add_nth_node(list3, list3->size, &sum);
        iterator1 = iterator1->next;
    }

    while (iterator2) {
        int v2 = *((int *)iterator2->data);
        int sum = v2 + carry;
        carry = sum / 10;
        sum %= 10;
        ll_add_nth_node(list3, list3->size, &sum);
        iterator2 = iterator2->next;
    }

    if (carry) {
        ll_add_nth_node(list3, list3->size, &carry);
    }

    return list3;
}

int main()
{
    linked_list_t* linked_list1;
    linked_list_t* linked_list2;
    linked_list_t* linked_list_sum;

    while (1) {
        char command[16];
        long size;

        scanf("%s", command);

        if (strcmp(command, "list1") == 0) {
            linked_list1 = ll_create(sizeof(int));
            scanf("%ld", &size);

            long int curr_nr;
            for (int i = 0; i < size; ++i) {
                scanf("%ld", &curr_nr);
                ll_add_nth_node(linked_list1, size, &curr_nr);
            }
        }

        if (strcmp(command, "list2") == 0) {
            linked_list2 = ll_create(sizeof(int));
            scanf("%ld", &size);

            long int curr_nr;
            for (int i = 0; i < size; ++i) {
                scanf("%ld", &curr_nr);
                ll_add_nth_node(linked_list2, size, &curr_nr);
            }

            break;
        }
    }

    linked_list_sum = sum_lists(linked_list1, linked_list2);
    ll_print_int(linked_list_sum);

    ll_free(&linked_list_sum);
    ll_free(&linked_list1);
    ll_free(&linked_list2);

    return 0;
}