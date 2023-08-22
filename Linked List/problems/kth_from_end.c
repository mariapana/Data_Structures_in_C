
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

struct Node {
	int data;
	struct Node *next;
};

struct LinkedList {
	struct Node *head;
};

struct Node* findKthNode(struct LinkedList *list, int k) {
	/*
	TODO: find the kth node from the tail
	*/
	struct Node *tmp1 = list->head;
	struct Node *tmp2 = list->head;

	for (int i = 0; i < k - 1 && tmp2; i++) {
		tmp2 = tmp2->next;
	}

	while (tmp2->next) {
		tmp1 = tmp1->next;
		tmp2 = tmp2->next;
	}

	return tmp1;
}

void readList(struct LinkedList *list, int *k) {
	int n;
	int i;
	int val;

	list->head = NULL;
	scanf("%d", &n);
	struct Node* curr;

	for (i = 0; i < n; ++i) {
		scanf("%d", &val);

		if (list->head == NULL) {
			struct Node* newNode = malloc(sizeof(struct Node));
			newNode->next = NULL;
			newNode->data = val;

			list->head = newNode;
			curr = list->head;
		} else {
			struct Node* newNode = malloc(sizeof(struct Node));
			newNode->next = NULL;
			newNode->data = val;

			curr->next = newNode;
			curr = curr->next;
		}

	}
	scanf("%d", k);
}

int main() {
	struct LinkedList *list;
	list = malloc(sizeof(struct LinkedList));
	int k;
	readList(list, &k);
	printf("%d", findKthNode(list, k)->data);

	return 0;
}
