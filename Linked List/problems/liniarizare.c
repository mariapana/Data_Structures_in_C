#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node
{
    int value;
    struct Node* next;
    struct Node* child;
};

/**
* Afiseaza la STDOUT o lista pe mai multe niveluri in forma liniarizata.
*
* @param head primul element din primul nivel al listei
*/
void printListLinearised(struct Node* head)
{
    /*
    * TODO implementati afisarea unei liste cu mai multe niveluri in forma
    * liniarizata
    */
    struct Node *tmp = head;
    while (tmp) {
        printf("%d ", tmp->value);
        if (tmp->child) {
            printListLinearised(tmp->child);
        }
        tmp = tmp->next;
    }
}

/* NU MODIFICATI FUNCTIILE DE CITIRE SI MAINUL! */
struct Node* readListLevel(void)
{
    int n, val, i;
    struct Node *head = NULL, *cHead;

    scanf("%d", &n);

    /* Se citeste un nivel din lista */
    for (i = 0; i != n; ++i)
    {
        scanf("%d", &val);

        if (head == NULL)
        {
            head = calloc(1, sizeof(*head));

            head->value = val;
            cHead = head;
        } else
        {
            cHead->next = calloc(1, sizeof(*head));

            cHead = cHead->next;
            cHead->value = val;
        }
    }

    return head;
}

void freeList(struct Node* head)
{
    struct Node* cHead = head->next;

    /* Se elibereaza memoria */
    while (cHead != NULL)
    {
        free(head);

        head = cHead;
        cHead = cHead->next;
    }

    free(head);
}

void linkLists(struct Node* head1, struct Node* head2, int pos)
{
    int p;

    for (p = 0; p != pos; ++p)
    {
        head1 = head1->next;
    }

    head1->child = head2;
}

int main(void)
{
    int numLists, i, numLinks, list1, pos, list2;
    struct Node** lists;

    scanf("%d", &numLists);

    lists = calloc(numLists, sizeof(*lists));

    for (i = 0; i != numLists; ++i)
    {
        lists[i] = readListLevel();
    }

    scanf("%d", &numLinks);

    for (i = 0; i != numLinks; ++i)
    {
        scanf("%d %d %d", &list1, &pos, &list2);
        linkLists(lists[list1], lists[list2], pos);
    }

    printListLinearised(lists[0]);
    printf("\n");

    for (i = 0; i != numLists; ++i)
    {
        freeList(lists[i]);
    }
    free(lists);

    return 0;
}
