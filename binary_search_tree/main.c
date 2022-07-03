////////////////////////////////////////////////////////////////////////////////
// All changes in this file are not recognized during testing!
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#include "bst.h"


/**
 * Print value of node.
 * @param node
 */
void print_val(struct node *node)
{
    printf("node->val = %d\n", node->val);
}

#define NUM_NODES 10

int main()
{
    int val;
    int rem[NUM_NODES];
    struct node *root = NULL;
    struct node *node = NULL;

    val = rand() % 100;

    rem[0] = val;
    printf("Insert: %d",val);
    node = new_node(val);
    root = insert_node(NULL, node);

    for (int i=0; i<(NUM_NODES-1); i++)
    {
        val = rand() % 100;
        printf(", %d",val);
        rem[i+1] = val;
        node = new_node(val);
        insert_node(root, node);
    }

    printf("\n\n");

    printf("Printing in order:\n");
    iterate_in_order(root, print_val);
    printf("\n");

    printf("Printing in pre-order:\n");
    iterate_pre_order(root, print_val);
    printf("\n");

    printf("Printing in post-order:\n");
    iterate_post_order(root, print_val);
    printf("\n");

    // Remove 3 random nodes
    val = rand() % (NUM_NODES-3);
    for (int i=val; i < val+3; i++)
    {
        printf("Remove node with value: %d\n", rem[i]);
        remove_node(root, rem[i]);
    }

    printf("\n");

    printf("Printing in order:\n");
    iterate_in_order(root, print_val);
    printf("\n");

    printf("Printing in pre-order:\n");
    iterate_pre_order(root, print_val);
    printf("\n");

    printf("Printing in post-order:\n");
    iterate_post_order(root, print_val);
    printf("\n");

    /* Free each node*/
    iterate_post_order(root, free_node);

    return EXIT_SUCCESS;
}
