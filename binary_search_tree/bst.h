#ifndef BST_H
#define BST_H

/**
 * Node struct
 */
struct node {
    struct node *left;
    struct node *right;
    int val;
};

struct node *findMin(struct node *root);

/**
 * Returns a pointer to a new note, whose pointers are set to NULL and the contained value is set to val.
 * @param val
 * @return pointer to a new note
 */
struct node *new_node(int val);

/**
 * Inserts node into tree defined by root.
 * @param root
 * @param node
 * @return Pointer to root node.
 */
struct node *insert_node(struct node *root, struct node *node);

/**
 * Remove node with value val from tree defined by root.
 * @param root
 * @param val
 * @return Pointer to root node.
 */
struct node *remove_node(struct node *root, int val);

/**
 * Iterate in in-order and apply function f.
 * @param root
 * @param f function
 */
void iterate_in_order(struct node *root,
                      void (*f)(struct node *n));

/**
 * Iterate in pre-order and apply function f.
 * @param node
 * @param f function
 */
void iterate_pre_order(struct node *node,
                       void (*f)(struct node *n));

/**
 * Iterate in post-order and apply function f.
 * @param node
 * @param f function
 */
void iterate_post_order(struct node *node,
                        void (*f)(struct node *n));

/**
 * Free node.
 * @param node
 */
void free_node(struct node *node);

#endif //BST_H
