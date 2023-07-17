#include <stdio.h>
#include <stdlib.h>

struct node {
	int item;
	int count;
	struct node *left;
	struct node *right;
	int height;
};



struct stack {
    struct node **items;
    int size;
    int capacity;
};

typedef struct node *Node;
typedef struct stack *Stack;





Stack StackNew(int capacity) {
    Stack new = (Stack)malloc(sizeof(struct stack));

    new->items = (Node *)malloc(sizeof(Node) * capacity);
    new->size = 0;
    new->capacity = capacity;

    return new;
}

void StackFree(Stack s) {
    free(s->items);
    free(s);
}

void StackPush(Stack s, Node item) {
    if (s->size == s->capacity) return;

    s->items[s->size] = item;
    s->size++;
}

void StackPop(Stack s) {
    if (s->size == 0) return;

    s->size--;
}

Node StackPeek(Stack s) {
    return s->items[s->size - 1];
}




int main(int argc, char **argv) {
    Stack s = StackNew(3);

    struct node n1, n2, n3, n4;
    n1.item = 1;
    n2.item = 2;
    n3.item = 3;
    n4.item = 4;

    StackPush(s, &n1);
    StackPush(s, &n2);
    StackPush(s, &n3);

    // Node np1 = StackPeek(s);
    // printf("%d\n", np1->item);

    // StackPop(s);
    // Node np2 = StackPeek(s);
    // printf("%d\n", np2->item);

    // StackPop(s);
    // Node np3 = StackPeek(s);
    // printf("%d\n", np3->item);

    // StackPush(s, &n4);
    // Node np4 = StackPeek(s);
    // printf("%d\n", np4->item);

    // StackPop(s);
    // Node np5 = StackPeek(s);
    // printf("%d\n", np5->item);

    StackPush(s, &n4);
    
    printf("Size: %d\n", s->size);

    // while (s->size > 0) {
    //     Node np = StackPeek(s);
    //     printf("%d\n", np->item);
    //     StackPop(s);
    // }

    StackFree(s);

    return 0;
}
