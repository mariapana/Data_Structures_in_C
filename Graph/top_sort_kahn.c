#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HMAX 100
#define MAX_STRING_SIZE 64
#define MIN(x, y) ((x) < (y) ? (x) : (y))


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

typedef struct queue_t queue_t;
struct queue_t
{
	unsigned int max_size;
	unsigned int size;
	unsigned int data_size;
	unsigned int read_idx;
	unsigned int write_idx;
	void **buff;
};

typedef struct
{
	linked_list_t** neighbors;
	int nodes;
} list_graph_t;


/* Helper data structures functions */
linked_list_t*
ll_create(unsigned int data_size)
{
	linked_list_t* ll = calloc(1, sizeof(*ll));

	ll->data_size = data_size;

	return ll;
}

static ll_node_t*
get_nth_node(linked_list_t* list, unsigned int n)
{
	unsigned int len = list->size - 1;
	unsigned int i;
	ll_node_t* node = list->head;

	n = MIN(n, len);

	for (i = 0; i < n; ++i)
		node = node->next;

	return node;
}

static ll_node_t*
create_node(const void* new_data, unsigned int data_size)
{
	ll_node_t* node = calloc(1, sizeof(*node));

	node->data = malloc(data_size);

	memcpy(node->data, new_data, data_size);

	return node;
}

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
	ll_node_t *new_node, *prev_node;

	if (!list)
		return;

	new_node = create_node(new_data, list->data_size);

	if (!n || !list->size) {
		new_node->next = list->head;
		list->head = new_node;
	} else {
		prev_node = get_nth_node(list, n - 1);
		new_node->next = prev_node->next;
		prev_node->next = new_node;
	}

	++list->size;
}

ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
	ll_node_t *prev_node, *removed_node;

	if (!list || !list->size)
		return NULL;

	if (!n) {
		removed_node = list->head;
		list->head = removed_node->next;
		removed_node->next = NULL;
	} else {
		prev_node = get_nth_node(list, n - 1);
		removed_node = prev_node->next;
		prev_node->next = removed_node->next;
		removed_node->next = NULL;
	}

	--list->size;

	return removed_node;
}

unsigned int
ll_get_size(linked_list_t* list)
{
	return !list ? 0 : list->size;
}

void
ll_free(linked_list_t** pp_list)
{
	ll_node_t* node;

	if (!pp_list || !*pp_list)
		return;

	while ((*pp_list)->size) {
		node = ll_remove_nth_node(*pp_list, 0);
		free(node->data);
		free(node);
	}

	free(*pp_list);
	*pp_list = NULL;
}

void
ll_print_int(linked_list_t* list)
{
	ll_node_t* node = list->head;

	for (; node; node = node->next)
		printf("%d ", *(int*)node->data);
	printf("\n");
}


queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t *q = calloc(1, sizeof(*q));
	q->data_size = data_size;
	q->max_size = max_size;
	q->buff = malloc(max_size * sizeof(*q->buff));
	return q;
}

unsigned int
q_get_size(queue_t *q)
{
	return !q ? 0 : q->size;
}

unsigned int
q_is_empty(queue_t *q)
{
	return !q ? 1 : !q->size;
}

void *
q_front(queue_t *q)
{
	if (!q || !q->size)
		return NULL;

	return q->buff[q->read_idx];
}

int
q_dequeue(queue_t *q)
{
	if (!q || !q->size)
		return 0;

	free(q->buff[q->read_idx]);

	q->read_idx = (q->read_idx + 1) % q->max_size;
	--q->size;
	return 1;
}

int
q_enqueue(queue_t *q, void *new_data)
{
	void *data;
	if (!q || q->size == q->max_size)
		return 0;

	data = malloc(q->data_size);
	memcpy(data, new_data, q->data_size);

	q->buff[q->write_idx] = data;
	q->write_idx = (q->write_idx + 1) % q->max_size;
	++q->size;

	return 1;
}

void
q_clear(queue_t *q)
{
	unsigned int i;
	if (!q || !q->size)
		return;

	for (i = q->read_idx; i != q->write_idx; i = (i + 1) % q->max_size)
		free(q->buff[i]);

	q->read_idx = 0;
	q->write_idx = 0;
	q->size = 0;
}

void
q_free(queue_t *q)
{
	if (!q)
		return;

	q_clear(q);
	free(q->buff);
	free(q);
}

static int is_node_in_graph(int n, int nodes)
{
	return n >= 0 && n < nodes;
}


list_graph_t*
lg_create(int nodes)
{
	int i;

	list_graph_t *g = malloc(sizeof(*g));

	g->neighbors = malloc(nodes * sizeof(*g->neighbors));

	for (i = 0; i != nodes; ++i)
		g->neighbors[i] = ll_create(sizeof(int));

	g->nodes = nodes;

	return g;
}

void
lg_add_edge(list_graph_t* graph, int src, int dest)
{
	if (
		!graph || !graph->neighbors
		|| !is_node_in_graph(src, graph->nodes)
		|| !is_node_in_graph(dest, graph->nodes)
	)
		return;

	ll_add_nth_node(graph->neighbors[src], graph->neighbors[src]->size, &dest);
}

static ll_node_t *find_node(linked_list_t *ll, int node, unsigned int *pos)
{
	ll_node_t *crt = ll->head;
	unsigned int i;

	for (i = 0; i != ll->size; ++i) {
		if (node == *(int *)crt->data) {
			*pos = i;
			return crt;
		}

		crt = crt->next;
	}

	return NULL;
}

int
lg_has_edge(list_graph_t* graph, int src, int dest)
{
	unsigned int pos;

	if (
		!graph || !graph->neighbors
		|| !is_node_in_graph(src, graph->nodes)
		|| !is_node_in_graph(dest, graph->nodes)
	)
		return 0;

	return find_node(graph->neighbors[src], dest, &pos) != NULL;
}

linked_list_t*
lg_get_neighbours(list_graph_t* graph, int node)
{
	if (
		!graph || !graph->neighbors
		|| !is_node_in_graph(node, graph->nodes)
	)
		return NULL;

	return graph->neighbors[node];
}

/* Printeaza lista de adiacenta a grafului
 */
void
lg_print_graph(list_graph_t* graph)
{
	for (int i = 0; i < graph->nodes; i++) {
		printf("%d: ", i);
		ll_print_int(graph->neighbors[i]);
	}
}

void
lg_free(list_graph_t* graph)
{
	int i;

	for (i = 0; i != graph->nodes; ++i)
		ll_free(graph->neighbors + i);
	
	free(graph->neighbors);
	free(graph);
}

linked_list_t* top_sort(list_graph_t* graph) {
    int n = graph->nodes;
    int *in_degree = calloc(n, sizeof(int));

    int i;
    for (i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
        in_degree[i] += lg_has_edge(graph, j, i);
    }

    // Create an queue and enqueue
	// all vertices with indegree 0
    queue_t *queue = q_create(sizeof(int), 5);
    for (int i = 0; i < n; i++) {
        if (!in_degree[i])
            q_enqueue(queue, &i);
    }

    linked_list_t *sorted_list = ll_create(sizeof(int));
    int ct = 0;
    while (!q_is_empty(queue)) {
        // Enqueue the queue and add the element to the list
        int x = *(int*)q_front(queue);
        q_dequeue(queue);
        ll_add_nth_node(sorted_list, sorted_list->size, &x);

        for (i = 0; i < n; i++) {
            if (lg_has_edge(graph, x, i));
                in_degree[i]--;
            if (!in_degree[i])
                q_enqueue(queue, &i);
        }
        ct++;
    }
    q_free(queue);
    free(in_degree);

    if (ct != n) {
        printf("There is a cycle in the graph\n");
        return NULL;
    }

    return sorted_list;
}


int main()
{
	list_graph_t *lg = NULL;
    lg = lg_create(6);
    lg_add_edge(lg, 5, 2);
    lg_add_edge(lg, 5, 0);
    lg_add_edge(lg, 4, 0);
    lg_add_edge(lg, 4, 1);
    lg_add_edge(lg, 2, 3);
    lg_add_edge(lg, 3, 1);

    printf("graful initial:\n");
    lg_print_graph(lg);

    printf("graful sortat topologic: ");
    linked_list_t* sorted = top_sort(lg);
    if (sorted)
        ll_print_int(sorted);

    ll_free(&sorted);
	lg_free(lg);


	return 0;
}
