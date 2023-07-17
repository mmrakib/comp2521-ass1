// Implementation of the Multiset ADT using a balanced BST
// COMP2521 23T2 Assignment 1

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Mset.h"
#include "MsetStructs.h"

////////////////////////////////////////////////////////////////////////
// Constants/Typedefs

#define TRUE 1
#define FALSE 0

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

typedef struct node *Node;
typedef struct stack *Stack;

////////////////////////////////////////////////////////////////////////
// Stack ADT

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

int StackIsEmpty(Stack s) {
	if (s->size == 0)
		return TRUE;
	else
		return FALSE;
}

void StackPush(Stack s, Node item) {
    if (s->size == s->capacity) return;

    s->items[s->size] = item;
    s->size++;
}

void StackPop(Stack s) {
    if (StackIsEmpty(s)) return;

    s->size--;
}

Node StackPeek(Stack s) {
	if (StackIsEmpty(s)) return NULL;

    return s->items[s->size - 1];
}

void StackReset(Stack s) {
	s->size = 0;
}

////////////////////////////////////////////////////////////////////////
// AVL Tree ADT

Node AVLNew(int item, int count) {
    if (item == UNDEFINED || count == 0) return NULL;

    Node new = (Node)malloc(sizeof(struct node));

    new->item = item;
    new->count = count;
    new->left = NULL;
    new->right = NULL;
    new->height = 0;

    return new;
}

void AVLFree(Node tree) {
    if (tree == NULL) return;

    if (tree->left != NULL) AVLFree(tree->left);
    if (tree->right != NULL) AVLFree(tree->right);

    free(tree);
}

int AVLGetItem(Node tree) {
	if (tree == NULL) return 0;
	
	return tree->item;
}

int AVLGetCount(Node tree) {
	if (tree == NULL) return 0;

	return tree->count;
}

int AVLGetHeight(Node tree) {
    if (tree == NULL) return 0;

    return tree->height;
}

int AVLGetBalance(Node tree) {
    if (tree == NULL) return 0;

    return AVLGetHeight(tree->left) - AVLGetHeight(tree->right);
}

Node AVLGetMinNode(Node tree) {
    if (tree == NULL) return tree; 

    Node current = tree;
    while (current->left != NULL) current = current->left;

    return current;
}

Node AVLGetMaxNode(Node tree) {
    if (tree == NULL) return tree;

    Node current = tree;
    while (current->right != NULL) current = current->right;

    return current;
}

Node AVLSearch(Node tree, int item) {
    if (tree == NULL) return NULL;

    if (item < tree->item) {
        return AVLSearch(tree->left, item);
    } else if (item > tree->item) {
        return AVLSearch(tree->right, item);
    } else {
        return tree;
    }
}

Node AVLRotateLeft(Node tree) {
    if (tree == NULL) return tree;

    Node r = tree->right;
    Node rl = tree->right->left;

    r->left = tree;
    tree->right = rl;

    tree->height = MAX(AVLGetHeight(tree->left), AVLGetHeight(tree->right));
    r->height = MAX(AVLGetHeight(r->left), AVLGetHeight(r->right));

    return r;
}

Node AVLRotateRight(Node tree) {
    if (tree == NULL) return tree;

    Node l = tree->left;
    Node lr = tree->left->right;

    l->right = tree;
    tree->left = lr;

    tree->height = MAX(AVLGetHeight(tree->left), AVLGetHeight(tree->right));
    l->height = MAX(AVLGetHeight(l->left), AVLGetHeight(l->right));

    return l;
}

Node AVLInsert(Node tree, int item, int count) {
	if (tree == NULL) return AVLNew(item, count);

	if (item < tree->item) {
		tree->left = AVLInsert(tree->left, item, count);
	} else if (item > tree->item) {
		tree->right = AVLInsert(tree->right, item, count);
	} else {
		return tree;
	}

	tree->height = 1 + MAX(AVLGetHeight(tree->left), AVLGetHeight(tree->right));
	int balance = AVLGetBalance(tree);

	if (balance > 1 && item < tree->left->item) {
		return AVLRotateRight(tree);
	} else if (balance < -1 && item > tree->right->item) {
		return AVLRotateLeft(tree);
	} else if (balance > 1 && item > tree->left->item) {
		tree->left = AVLRotateLeft(tree->left);
		return AVLRotateRight(tree);
	} else if (balance < -1 && item < tree->right->item) {
		tree->right = AVLRotateRight(tree->right);
		return AVLRotateLeft(tree);
	} else
		return tree;
}

Node AVLDelete(Node tree, int item) {
	if (tree == NULL) return tree;

	if (item < tree->item) {
		tree->left = AVLDelete(tree->left, item);
	} else if (item > tree->item) {
		tree->right = AVLDelete(tree->right, item);
	} else {
		if ( (tree->left == NULL) || (tree->right == NULL) ) {
			Node temp = tree->left ? tree->left : tree->right;

			if (temp == NULL) {
				temp = tree;
				tree = NULL;
			} else {
				*tree = *temp;
			}

			free(temp);
		} else {
			Node temp = AVLGetMinNode(tree->right);

			tree->item = temp->item;

			tree->right = AVLDelete(tree->right, temp->item);
		}
	}

	if (tree == NULL) return tree;

	tree->height = 1 + MAX(AVLGetHeight(tree->left), AVLGetHeight(tree->right));
	int balance = AVLGetBalance(tree);

	if (balance > 1 && AVLGetBalance(tree->left) >= 0) {
		return AVLRotateRight(tree);
	} else if (balance > 1 && AVLGetBalance(tree->left) < 0) {
		tree->left = AVLRotateLeft(tree->left);
		return AVLRotateRight(tree);
	} else if (balance < -1 && AVLGetBalance(tree->right) <= 0) {
		return AVLRotateLeft(tree);
	} else if (balance < -1 && AVLGetBalance(tree->right) > 0) {
		tree->right = AVLRotateRight(tree->right);
		return AVLRotateLeft(tree);
	} else
		return tree;
}

void AVLPrintPreorder(Node tree) {
    if (tree == NULL) return;

    printf("%d\n", tree->item);

    AVLPrintPreorder(tree->left);
    AVLPrintPreorder(tree->right);
}

////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty mset
 */
Mset MsetNew(void) {
	Mset new = (Mset)malloc(sizeof(struct mset));

	new->tree = NULL;
	new->size = 0;
	new->total_count = 0;

	return new;
}

/**
 * Frees all memory allocated to the given mset
 */
void MsetFree(Mset s) {
	if (s == NULL) return;

	AVLFree(s->tree);
	free(s);
}

/**
 * Inserts one of an item into the mset. Does nothing if the item is
 * equal to UNDEFINED.
 */
void MsetInsert(Mset s, int item) {
	if (s == NULL || item == UNDEFINED) return;

	Node found = AVLSearch(s->tree, item);

	if (found == NULL) {
		s->tree = AVLInsert(s->tree, item, 1);
		s->size++;
	} else {
		found->count++;
	}

	s->total_count++;
}

/**
 * Inserts the given amount of an item into the mset. Does nothing if
 * the item is equal to UNDEFINED or the given amount is 0 or less.
 */
void MsetInsertMany(Mset s, int item, int amount) {
	if (s == NULL || item == UNDEFINED || amount <= 0) return;

	Node found = AVLSearch(s->tree, item);

	if (found == NULL) {
		s->tree = AVLInsert(s->tree, item, amount);
		s->size++;
	} else {
		found->count += amount;
	}

	s->total_count += amount;
}

/**
 * Returns the number of distinct elements in the mset
 */
int MsetSize(Mset s) {
	if (s == NULL) return 0;

	return s->size;
}

/**
 * Returns the sum of counts of all elements in the mset
 */
int MsetTotalCount(Mset s) {
	if (s == NULL) return 0;

	return s->total_count;
}

/**
 * Returns the count of an item in the mset, or 0 if it doesn't occur in
 * the mset
 */
int MsetGetCount(Mset s, int item) {
	if (s == NULL || item == UNDEFINED) return 0;

	Node found = AVLSearch(s->tree, item);

	return AVLGetCount(found);
}

/**
 * Prints the given mset in the format
 * {(elem1, count1), (elem2, count2), (elem3, count3)}
 * where the elements are in increasing order
 */
void _MsetShow(Node tree, int max_item) {
	if (tree == NULL) return;

	_MsetShow(tree->left, max_item);

	if (AVLGetItem(tree) == max_item) {
		printf("(%d, %d)", AVLGetItem(tree), AVLGetCount(tree));
	} else {
		printf("(%d, %d), ", AVLGetItem(tree), AVLGetCount(tree));
	}

	_MsetShow(tree->right, max_item);
}

void MsetShow(Mset s) {
	printf("{");

	if (s == NULL) {
		printf("}\n");
		return;
	}

	Node max_node = AVLGetMaxNode(s->tree);
	_MsetShow(s->tree, max_node->item);

	printf("}\n");
}

////////////////////////////////////////////////////////////////////////
// Advanced Operations

/**
 * Returns a new mset representing the union of the two msets
 */
Mset MsetUnion(Mset s1, Mset s2) {
	Mset new = MsetNew();

	Stack stack = StackNew(MAX(MsetSize(s1), MsetSize(s2)) + 1);
	Node current = NULL;
	int done = FALSE;

	// First iteration (copy)
	current = s1->tree;
	while (!done) {
		if (current != NULL) {
			StackPush(stack, current);
			current = current->left;
		} else {
			if (!StackIsEmpty(stack)) {
				current = StackPeek(stack);
				StackPop(stack);

				// Copy into new mset
				MsetInsertMany(new, current->item, current->count);

				current = current->right;
			} else {
				done = TRUE;
			}
		}
	}

	// Reset
	StackReset(stack);
	done = FALSE;

	// Second iteration (search and insert)
	current = s2->tree;
	while (!done) {
		if (current != NULL) {
			StackPush(stack, current);
			current = current->left;
		} else {
			if (!StackIsEmpty(stack)) {
				current = StackPeek(stack);
				StackPop(stack);

				// Do something here
				Node found = AVLSearch(s1->tree, current->item);

				if (found != NULL) {
					int count = MAX(current->count, found->count);
					
					Node to_edit = AVLSearch(new->tree, current->item);

					if (to_edit != NULL) {
						new->total_count += count - to_edit->count;
						to_edit->count = count;
					} else {
						MsetInsertMany(new, current->item, count);
					}
					
				} else {
					MsetInsertMany(new, current->item, current->count);
				}

				current = current->right;
			} else {
				done = TRUE;
			}
		}
	}

	StackFree(stack);

	return new;
}

/**
 * Returns a new mset representing the intersection of two msets
 */
Mset MsetIntersection(Mset s1, Mset s2) {
	Mset new = MsetNew();

	Stack stack = StackNew(MAX(MsetSize(s1), MsetSize(s2)) + 1);
	Node current = NULL;
	int done = FALSE;

	current = s1->tree;
	while (!done) {
		if (current != NULL) {
			StackPush(stack, current);
			current = current->left;
		} else {
			if (!StackIsEmpty(stack)) {
				current = StackPeek(stack);
				StackPop(stack);

				// Copy into new mset
				Node found = AVLSearch(s2->tree, current->item);

				if (found != NULL) {
					int count = MIN(current->count, found->count);
					MsetInsertMany(new, current->item, count);
				}

				current = current->right;
			} else {
				done = TRUE;
			}
		}
	}

	StackFree(stack);

	return new;
}

/**
 * Returns a new mset representing the addition of two msets
 */
Mset MsetSum(Mset s1, Mset s2) {
	Mset new = MsetNew();

	Stack stack = StackNew(MAX(MsetSize(s1), MsetSize(s2)) + 1);
	Node current = NULL;
	int done = FALSE;

	// First iteration (copy)
	current = s1->tree;
	while (!done) {
		if (current != NULL) {
			StackPush(stack, current);
			current = current->left;
		} else {
			if (!StackIsEmpty(stack)) {
				current = StackPeek(stack);
				StackPop(stack);

				// Copy into new mset
				MsetInsertMany(new, current->item, current->count);

				current = current->right;
			} else {
				done = TRUE;
			}
		}
	}

	// Reset
	StackReset(stack);
	done = FALSE;

	// Second iteration (search and insert)
	current = s2->tree;
	while (!done) {
		if (current != NULL) {
			StackPush(stack, current);
			current = current->left;
		} else {
			if (!StackIsEmpty(stack)) {
				current = StackPeek(stack);
				StackPop(stack);

				// Do something here
				Node found = AVLSearch(s1->tree, current->item);

				if (found != NULL) {
					int count = current->count + found->count;
					
					Node to_edit = AVLSearch(new->tree, current->item);

					if (to_edit != NULL) {
						new->total_count += count - to_edit->count;
						to_edit->count = count;
					} else {
						MsetInsertMany(new, current->item, count);
					}
					
				} else {
					MsetInsertMany(new, current->item, current->count);
				}

				current = current->right;
			} else {
				done = TRUE;
			}
		}
	}

	StackFree(stack);

	return new;
}

/**
 * Returns a new mset representing the difference s1 - s2
 */
Mset MsetDifference(Mset s1, Mset s2) {
	Mset new = MsetNew();

	Stack stack = StackNew(MAX(MsetSize(s1), MsetSize(s2)) + 1);
	Node current = NULL;
	int done = FALSE;

	current = s1->tree;
	while (!done) {
		if (current != NULL) {
			StackPush(stack, current);
			current = current->left;
		} else {
			if (!StackIsEmpty(stack)) {
				current = StackPeek(stack);
				StackPop(stack);

				// Copy into new mset
				Node found = AVLSearch(s2->tree, current->item);

				if (found != NULL) {
					int count = MAX(current->count - found->count, 0);
					MsetInsertMany(new, current->item, count);
				} else {
					MsetInsertMany(new, current->item, current->count);
				}

				current = current->right;
			} else {
				done = TRUE;
			}
		}
	}

	StackFree(stack);
	
	return new;
}

/**
 * Returns true if the mset s1 is included in the mset s2, and false
 * otherwise
 */
bool MsetIncluded(Mset s1, Mset s2) {
	Stack stack = StackNew(MAX(MsetSize(s1), MsetSize(s2)) + 1);
	Node current = NULL;
	int done = FALSE;

	current = s1->tree;
	while (!done) {
		if (current != NULL) {
			StackPush(stack, current);
			current = current->left;
		} else {
			if (!StackIsEmpty(stack)) {
				current = StackPeek(stack);
				StackPop(stack);

				// Copy into new mset
				Node found = AVLSearch(s2->tree, current->item);

				if (found == NULL || current->count > found->count) {
					return false;
				}

				current = current->right;
			} else {
				done = TRUE;
			}
		}
	}

	return true;
}

/**
 * Returns true if the two given msets are equal, and false otherwise
 */
bool MsetEquals(Mset s1, Mset s2) {
	Stack stack = StackNew(MAX(MsetSize(s1), MsetSize(s2)) + 1);
	Node current = NULL;
	int done = FALSE;

	if (s1->size != s2->size) return false;

	current = s1->tree;
	while (!done) {
		if (current != NULL) {
			StackPush(stack, current);
			current = current->left;
		} else {
			if (!StackIsEmpty(stack)) {
				current = StackPeek(stack);
				StackPop(stack);

				Node found = AVLSearch(s2->tree, current->item);

				if (found == NULL || current->count != found->count) {
					return false;
				}

				current = current->right;
			} else {
				done = TRUE;
			}
		}
	}

	return true;
}

/**
 * Stores the k most common elements in the mset into the given items
 * array in decreasing order of count and returns the number of elements
 * stored. Elements with the same count should be stored in increasing
 * order.
 */
int MsetMostCommonCompare(const void *a, const void *b) {
	struct item *item_a = (struct item *)a;
	struct item *item_b = (struct item *)b;

	if (item_b->count == item_a->count)
		return (item_a->item - item_b->item);
	else
		return (item_b->count - item_a->count);
}

int MsetMostCommon(Mset s, int k, struct item items[]) {
	int size = MsetSize(s);
	Stack stack = StackNew(size + 1);
	Node current = NULL;
	int done = FALSE;

	struct item temp[size];

	current = s->tree;
	int i = 0;
	while (!done) {
		if (current != NULL) {
			StackPush(stack, current);
			current = current->left;
		} else {
			if (!StackIsEmpty(stack)) {
				current = StackPeek(stack);
				StackPop(stack);

				// Add items to items array
				struct item item;
				item.item = current->item;
				item.count = current->count;

				temp[i] = item;
				i++;

				current = current->right;
			} else {
				done = TRUE;
			}
		}
	}

	qsort(temp, size, sizeof(struct item), MsetMostCommonCompare);

	// for (int i = 0; i < size; i++) {
	// 	printf("Item: %d, Count: %d\n", temp[i].item, temp[i].count);
	// }
	// printf("\n");

	int r = MIN(k, size);

	for (int i = 0; i < r; i++) {
		items[i] = temp[i];
	}

	return r;
}

////////////////////////////////////////////////////////////////////////
// Cursor Operations

typedef struct cursor *MsetCursor;

/**
 * Creates a new cursor positioned at the start of the mset
 */
MsetCursor MsetCursorNew(Mset s) {
	// TODO
	return NULL;
}

/**
 * Frees all memory allocated to the given cursor
 */
void MsetCursorFree(MsetCursor cur) {
	// TODO
}

/**
 * Returns the element at the cursor's position and its count, or
 * {UNDEFINED, 0} if the cursor is positioned at the start or end of
 * the mset.
 */
struct item MsetCursorGet(MsetCursor cur) {
	// TODO
	return (struct item){UNDEFINED, 0};
}

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * mset if there is no next greatest element. Returns false if the
 * cursor is now at the end of the mset, and true otherwise.
 */
bool MsetCursorNext(MsetCursor cur) {
	// TODO
	return false;
}

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * mset if there is no next smallest element. Returns false if the
 * cursor is now at the start of the mset, and true otherwise.
 */
bool MsetCursorPrev(MsetCursor cur) {
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////

