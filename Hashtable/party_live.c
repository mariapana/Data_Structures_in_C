#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 25000
#define MAX_STRING_SIZE	256
#define HMAX 30000

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

linked_list_t *
ll_create(unsigned int data_size)
{
    linked_list_t* ll;

    ll = malloc(sizeof(*ll));

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

    if (!list) {
        return;
    }

    if (n > list->size) {
        n = list->size;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(*new_node));
    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, new_data, list->data_size);

    new_node->next = curr;
    if (prev == NULL) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }

    list->size++;
}

ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;

    if (!list || !list->head) {
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
     if (!list) {
        return -1;
    }

    return list->size;
}

void
ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;

    if (!pp_list || !*pp_list) {
        return;
    }

    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }

    free(*pp_list);
    *pp_list = NULL;
}

typedef struct info info;
struct info {
	void *key;
	void *value;
};

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);
};

int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

unsigned int
hash_function_string(void *a)
{
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned long hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}


hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
    if (!hash_function || !compare_function) {
        return NULL;
    }
    
    hashtable_t* map = malloc(sizeof(hashtable_t));

    map->size = 0;
    map->hmax = hmax;
    map->hash_function = hash_function;
    map->compare_function = compare_function;
    
    map->buckets = malloc(map->hmax * sizeof(linked_list_t *));
    for (unsigned int i = 0; i < map->hmax; ++i) {
        map->buckets[i] = ll_create(sizeof(info));
    }
    return map;	
}

int
ht_has_key(hashtable_t *ht, void *key)
{
    if (!ht || !key) {
        return -1;
    }

    int hash_index = ht->hash_function(key) % ht->hmax;
    ll_node_t* node = ht->buckets[hash_index]->head;

    while (node != NULL) {
        info* data_info = (info *)node->data;
        if (!ht->compare_function(data_info->key, key)) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

void *
ht_get(hashtable_t *ht, void *key)
{
	if (!ht || !key || ht_has_key(ht, key) != 1) {
        return NULL;
    }
    
    int hash_index = ht->hash_function(key) % ht->hmax;
    ll_node_t* node = ht->buckets[hash_index]->head;

    while (node != NULL) {
        info* data_info = (info *)node->data;
        if (!ht->compare_function(data_info->key, key)) {
            return data_info->value;
        }
        node = node->next;
    }
    return NULL;
}

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
    void *value, unsigned int value_size)
{
    if (!ht || !key || !value) {
        return;
    }

    int hash_index = ht->hash_function(key) % ht->hmax;
    
    if (ht_has_key(ht, key) == 1) {
        ll_node_t* node = ht->buckets[hash_index]->head;
        while (node != NULL) {
            info* data_info = node->data;

            if (!ht->compare_function(data_info->key, key)) {
                free(data_info->value);

                data_info->value = malloc(value_size);
                memcpy(data_info->value, value, value_size);
                return;  
            }

            node = node->next;
        }
    }

    info* data_info = malloc(sizeof(info));

    data_info->key = malloc(key_size);
    data_info->value = malloc(value_size);

    memcpy(data_info->key, key, key_size);
    memcpy(data_info->value, value, value_size);

    ll_add_nth_node(ht->buckets[hash_index], 0, data_info);
    ht->size++;

    free(data_info);
}

void
ht_remove_entry(hashtable_t *ht, void *key)
{
	if (!ht || !key || ht_has_key(ht, key) != 1) {
        return;
    }

    int hash_index = ht->hash_function(key) % ht->hmax;
    ll_node_t* node = ht->buckets[hash_index]->head;

    unsigned int node_nr = 0;
    while (node != NULL) {
        info* data_info = (info *)node->data;

        if (!ht->compare_function(data_info->key, key)) {
            free(data_info->key);
            free(data_info->value);
            free(data_info);

            ll_node_t* deleted_node = ll_remove_nth_node(ht->buckets[hash_index], node_nr);
            free(deleted_node);

            ht->size--;
            return;
        }

        node = node->next;
        node_nr++;
    }
}


void
ht_free(hashtable_t *ht)
{	
    if (!ht) {
        return;
    }

    for (unsigned int i = 0; i < ht->hmax; ++i) {
        ll_node_t* node = ht->buckets[i]->head;

        while (node != NULL) {
            info* data_info = (info *)node->data;
            free(data_info->key);
            free(data_info->value);
            node = node->next;
        }

        ll_free(&ht->buckets[i]);
    }
    free(ht->buckets);
    free(ht);
}

unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int
ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}

typedef struct preferences preferences;
struct preferences {
    char sign;
    char ingredient[MAX_STRING_SIZE];
};

void check_conflicts(preferences *preference_array, int size) {
    hashtable_t *ht = ht_create(HMAX, hash_function_string, compare_function_strings);
    // TODO

    /*
        ht :

        'pizza': '+',
        'hamburger': '-'
    */

    // element curent: {ingredient: 'pizza', sign: '-'}

    for (int i = 0; i < size; ++i) {
        // preference_array[i] - elementul curent

        // verific daca am deja ingredient in dictionar
        void* data = ht_get(ht, preference_array[i].ingredient);
        if (data != NULL) {
            // exista deja o cheie in dictionar cu acel string
            char sign = *(char *) data;
            if (sign != preference_array[i].sign) {
                // avem un conflict
                printf("Unhappy..\n");
                printf("%s\n", preference_array[i].ingredient);
                return;
            }
        } else {
            ht_put(ht, preference_array[i].ingredient, strlen(preference_array[i].ingredient) + 1,
            &preference_array[i].sign, sizeof(char));
        }
    }

    printf("Happy!\n");

}

int main()
{
    int index = 0;
    char line[MAX_STRING_SIZE] = {0};
    preferences *preference_array = calloc(MAXN, sizeof(*preference_array));

	while (fgets(line, MAX_STRING_SIZE, stdin)) {
        line[strlen(line) - 1] = 0;
        preference_array[index].sign = line[0];
        memcpy(preference_array[index].ingredient, line + 2, strlen(line + 2));
        ++index;

        memset(line, 0, sizeof(line));
	}

    check_conflicts(preference_array, index);

	return 0;
}