#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
* Implementation
*/

#define TRUE 1
#define FALSE 0
#define UNDEFINED INT_MIN

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

struct node {
    int item;
    int count;
    struct node *left;
    struct node *right;
    int height;
};

typedef struct node *Node;

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

/*
* Testing
*/

#define CC_RED 31
#define CC_GREEN 32

void PrintColor(const char *s, int color) {
    if (color != 0) {
        printf("\033[0;%dm", color);
        printf("%s", s);
        printf("\033[0m");
    } else {
        printf("%s", s);
    }
}

#define TEST(name, cond) printf("%s: ", name); (cond) ? PrintColor("passed\n", CC_GREEN) : PrintColor("failed\n", CC_RED);

void TestAVLNew() {
    printf("AVLNew:\n");

    Node tree1 = AVLNew(3, 2);
    TEST("Function works correctly", (tree1->item == 3 && tree1->count == 2)); 

    Node tree2 = AVLNew(UNDEFINED, 2);
    TEST("Check incorrect argument 1", (tree2 == NULL));

    Node tree3 = AVLNew(12, 0);
    TEST("Check incorrect argument 2", (tree3 == NULL));

    AVLFree(tree1);
    AVLFree(tree2);
    AVLFree(tree3);

    printf("\n");
}

void TestAVLInsert() {
    printf("AVLInsert:\n");

    Node tree = NULL;
    tree = AVLInsert(tree, 5, 2);
    TEST("Adding to empty tree", (tree->item == 5 && tree->count == 2));

    tree = AVLInsert(tree, 3, 4);
    TEST("Adding to left of tree", (tree->left->item == 3 && tree->left->count == 4));

    tree = AVLInsert(tree, 8, 5);
    TEST("Adding to right of tree", (tree->right->item == 8 && tree->right->count == 5));

    AVLFree(tree);

    printf("\n");
}

void TestAVLMinMax() {
    Node tree = NULL;
    tree = AVLInsert(tree, 6, 1);
    tree = AVLInsert(tree, 3, 1);
    tree = AVLInsert(tree, 8, 1);
    tree = AVLInsert(tree, 2, 1);
    tree = AVLInsert(tree, 9, 1);

    printf("AVLGetMinNode:\n");
    Node min_node = AVLGetMinNode(tree);
    TEST("Correct functionality", (min_node->item == 2));

    printf("AVLGetMaxNode:\n");
    Node max_node = AVLGetMaxNode(tree);
    TEST("Correct functionality", (max_node->item == 9));

    AVLFree(tree);

    printf("\n");
}

void TestAVLSearch() {
    printf("AVLSearch:\n");

    Node tree = NULL;
    tree = AVLInsert(tree, 7, 1);
    tree = AVLInsert(tree, 2, 1);
    tree = AVLInsert(tree, 12, 5);
    tree = AVLInsert(tree, 8, 1);
    tree = AVLInsert(tree, 3, 1);

    Node found = AVLSearch(tree, 12);
    TEST("Correct functionality", (found->item == 12 && found->count == 5));

    found = AVLSearch(tree, 15);
    TEST("Non-existent node", (found == NULL));

    AVLFree(tree);

    printf("\n");
}

void TestAVLDelete() {
    printf("AVLDelete:\n");

    Node tree = NULL;
    tree = AVLInsert(tree, 6, 1);
    tree = AVLInsert(tree, 5, 1);
    tree = AVLInsert(tree, 7, 1);
    tree = AVLInsert(tree, 10, 1);
    tree = AVLInsert(tree, 8, 1);

    tree = AVLDelete(tree, 10);
    Node found = AVLSearch(tree, 10);
    TEST("Correct functionality", (found == NULL));

    AVLFree(tree);

    printf("\n");
}

int main(int argc, char **argv) {
    printf("Running tests...\n\n");

    TestAVLNew();
    TestAVLInsert();
    TestAVLMinMax();
    TestAVLSearch();
    TestAVLDelete();

    printf("Finished tests.\n");

    return 0;
}
