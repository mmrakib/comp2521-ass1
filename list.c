#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
* Constants/Typedefs
*/

#define TRUE 1
#define FALSE 0
#define UNDEFINED INT_MIN

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

struct list_node {
    void *item;
    struct list_node *next;
    struct list_node *prev;
};

struct list {
	struct list_node *head;
	struct list_node *tail;
	int size;
};

typedef struct list *List;
typedef struct list_node *ListNode;

/*
* Implementation
*/

List ListNew() {
    List new = (List)malloc(sizeof(struct list));

    new->head = NULL;
    new->tail = NULL;
    new->size = 0;

    return new;
}

void ListFree(List l) {
    if (l == NULL) return;

    ListNode current = l->head;

    while (current != NULL) {
        ListNode next = current->next;
        free(current);
        current = next;
    }

    free(l);
}

int ListIsEmpty(List l) {
    if (l->size == 0 && l->head == NULL & l->tail == NULL)
        return TRUE;
    else
        return FALSE;
}

List ListPush(List l, void *item) {
    if (l == NULL) return NULL;

    ListNode new = (ListNode)malloc(sizeof(struct list_node));

    new->item = item;
    new->next = NULL;

    if (ListIsEmpty(l)) {
        l->head = new;
        l->tail = new;
    } else {
        ListNode temp = l->tail;

        temp->next = new;
        new->prev = temp;   
        l->tail = new;
    }

    l->size++;

    return l;
}

void *ListPop(List l) {
    if (l == NULL || ListIsEmpty(l)) return NULL;

    ListNode temp = l->tail;

    temp->prev->next = NULL;
    l->tail = temp->prev;

    void *item = temp->item;
    free(temp);

    l->size--;

    return item;
}

int main(int argc, char **argv) {
    List l = ListNew();

    int x = 1, y = 2, z = 3, w = 4;

    l = ListPush(l, (void *)&x);
    l = ListPush(l, (void *)&y);
    l = ListPush(l, (void *)&z);
    l = ListPush(l, (void *)&w);

    while (!ListIsEmpty(l)) {
        int *item = (int *)ListPop(l);
        if (item != NULL) printf("%d\n", *item);
    }

    ListFree(l);

    return 0;
}
