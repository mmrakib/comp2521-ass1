// COMP2521 23T2 Assignment 1

#ifndef MSET_STRUCTS_H
#define MSET_STRUCTS_H

// DO NOT CHANGE THE NAME OF THIS STRUCT
struct node {
	int item;           // DO NOT CHANGE/REMOVE THIS FIELD
	int count;          // DO NOT CHANGE/REMOVE THIS FIELD
	struct node *left;  // DO NOT CHANGE/REMOVE THIS FIELD
	struct node *right; // DO NOT CHANGE/REMOVE THIS FIELD

	// You may add more fields here if needed
	int height;
};

// DO NOT CHANGE THE NAME OF THIS STRUCT
struct mset {
	struct node *tree;  // DO NOT CHANGE/REMOVE THIS FIELD

	// You may add more fields here if needed
	int size;
	int total_count;
};

// You may define more structs here if needed
struct stack {
    struct node **items;
    int size;
    int capacity;
};

////////////////////////////////////////////////////////////////////////
// Cursors

struct cursor {
	struct node *current;

	struct node *next;
	struct node *prev;

	int at_start;
	int at_end;
};

////////////////////////////////////////////////////////////////////////

#endif

