/**
 * @file lab.h
 * @brief Header file for the circular doubly linked list implementation.
 *
 * This file contains the definitions of the structures and functions used
 * for managing a circular doubly linked list with a sentinel node.
 */

#ifndef LAB_H
#define LAB_H

#include <stdlib.h>

/**
 * @struct Node
 * @brief Represents a node in the circular doubly linked list.
 *
 * Each node stores a pointer to an integer value, along with pointers to 
 * the next and previous nodes in the list.
 */
typedef struct Node {
    int* data;          // information stored in node
    struct Node* next;  // pointer to the next node
    struct Node* prev;  // pointer to the previous node
} Node;

/**
 * @struct List
 * @brief Represents a circular doubly linked list.
 *
 * The list contains a sentinel node, which acts as a dummy node to 
 * simplify insertion and deletion by avoiding special cases for the head 
 * and tail.
 */
typedef struct List {
    Node* sentinel; // Sentinel node to simplify list operations
} List;

/**
 * @brief Creates and initializes an empty circular doubly linked list.
 * @return Pointer to the newly created list.
 */
List* create_list();

/**
 * @brief Creates and adds a new node with the given data to the list.
 * @param list Pointer to the list.
 * @param data Pointer to the integer data to store in the new node.
 */
void list_add_node(List* list, int* data);

/**
 * @brief Removes a specified node from the list.
 * @param list Pointer to the list.
 * @param node Pointer to the node to remove.
 * @return Pointer to the removed node, or NULL if the node is invalid.
 */
Node* list_remove(List* list, Node* node);

/**
 * @brief Destroys the list and frees all allocated memory.
 * @param list Pointer to the list to destroy.
 */
void list_destroy(List* list);

// Node* list_get_first(List* list);
// Node* list_get_last(List* list);

#endif
