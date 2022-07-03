#include <stdlib.h>

#include "bst.h"

/**
 * Returns a pointer to a new note, whose pointers are set to NULL and the contained value is set to val.
 * @param val
 * @return pointer to a new note
 */
struct node *new_node(int val){
    struct node *new_node =  (struct node *)malloc(sizeof(struct node)); 
    new_node->val = val; 
    new_node->left = new_node->right = NULL; 
    return new_node; 
}

/**
 * Free node.
 * @param node
 */

void free_node(struct node *node)
{
    if ( node )
    {
        free(node);
    }
}

struct node *findMin(struct node *root){

while (root -> left != NULL)
root = root -> left;

    return root;
}
/**
 * Inserts node into tree defined by root.
 * @param root
 * @param node
 * @return Pointer to root node.
 */
struct node *insert_node(struct node *root, struct node *node){

  /* If the tree is empty, return a new node */
    if (root == NULL) return node; 
  
    /* Otherwise, recur down the tree */
    else if (node -> val < root -> val) 
        root->left  = insert_node(root -> left, node); 
    else if (node -> val > root -> val) 
        root->right = insert_node(root -> right, node);    
  
    /* return the (unchanged) node pointer */
       return root;

}

/**
 * Remove node with value val from tree defined by root.
 * @param root
 * @param val
 * @return Pointer to root node.
 */
struct node *remove_node(struct node *root, int val){

    if (root == NULL) return root;
    else if (val < root -> val)
    root -> left = remove_node(root -> left, val);
    else if (val > root -> val)
    root -> right = remove_node(root -> right, val);
    else
    {
        /* Case 1: No child */
        if (root -> left == NULL && root->right == NULL) {
            free_node(root);
            root = NULL;
        }

        /* Case 2: One child */
        else if (root -> left == NULL) {
            struct node *temporary = root;
            root = root -> right;
            free_node(temporary);
            
        }
        else if (root -> right == NULL) {
            struct node *temporary = root;
            root = root -> left;
            free_node(temporary);
            
        }

        /* case 3: 2 children */ 
        else {
            struct node *temporary = findMin(root -> right);
            root -> val = temporary -> val;
            root -> right = remove_node(root -> right, temporary -> val);
        }
    }
    return root;
}


/**
 * Iterate in in-order and apply function f.
 * @param root
 * @param f function
 */
void iterate_in_order(struct node *root, void (*f)(struct node *n)){
     
    if (root == NULL) return; 
  
    /* first recur on left child */
    iterate_in_order(root->left,f); 
  
    f(root);
    
    /* now recur on right child */
    iterate_in_order(root->right, f); 

  return;
}


/**
 * Iterate in pre-order and apply function f.
 * @param node
 * @param f function
 */
void iterate_pre_order(struct node *node, void (*f)(struct node *n)){

    if (node == NULL) return; 
  
  f(node);

    /* first recur on left child */
    iterate_pre_order(node->left, f); 

    /* now recur on right child */
    iterate_pre_order(node->right, f); 

     return;
}

/**
 * Iterate in post-order and apply function f.
 * @param node
 * @param f function
 */
void iterate_post_order(struct node *node, void (*f)(struct node *n)){
    
    if (node == NULL) return; 
  
    // first recur on left subtree 
    iterate_post_order(node->left,f); 
  
    // then recur on right subtree 
    iterate_post_order(node->right, f); 

     f(node);
    
     return;

}

