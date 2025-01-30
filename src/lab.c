/**
 * @file lab.c
 * @brief Implementation of a circular doubly linked list.
 *
 * A circular doubly linked list is a linked list that can be iterated 
 * forward or back, and loops back around to itself.
 * 
 * This implementation adds new nodes exclusively to the tail, but may 
 * be extended in the future.
 */

#include "lab.h"
#include <stdio.h>

/**
 * @brief Creates and initializes an empty circular doubly linked list.
 * @return Pointer to the newly created list.
 */
List* create_list() {
    List* list = (List*)malloc(sizeof(List));
    if (!list) {
        fprintf(stderr, "Memory allocation failed for the list.\n");
        return NULL;
    }
    
    list->sentinel = (Node*)malloc(sizeof(Node));
    if (!list->sentinel) {
        fprintf(stderr, "Memory allocation failed for the sentinel node.\n");
        free(list);
        return NULL;
    }
    
    list->sentinel->data = NULL; // sentinel is dummy node with no data
    list->sentinel->next = list->sentinel; // list is empty so it loops to itself
    list->sentinel->prev = list->sentinel;
    
    return list;
}

/**
 * @brief Adds a new node with the given data to the list.
 * @param list Pointer to the list.
 * @param data Pointer to the integer data to store in the new node.
 */
void list_add_node(List* list, int* data) {
    // If there's no list or no data, short-circuit.
    if (!list || !data) {
        return;
    }
    // Create node.
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed for the new node.\n");
        return;
    }
    // Add the node's data.
    new_node->data = data;
    /* Insert the new node before the sentinel, which is the end of the list since the linked list is circular and doubly linked. */
    new_node->next = list->sentinel;
    new_node->prev = list->sentinel->prev; // new node's previous is sentinel's old previous
    list->sentinel->prev->next = new_node; // sentinel's old previous's next is now new node
    list->sentinel->prev = new_node; // sentinel's old previous is now new node
}

/**
 * @brief Removes a specified node from the list.
 * @param list Pointer to the list.
 * @param node Pointer to the node to remove.
 * @return Pointer to the removed node, or NULL if the node is invalid.
 */
Node* list_remove(List* list, Node* node_to_remove) {
    // Short-circuit if no list, no node, or list is empty
    if (!list || !node_to_remove || (node_to_remove == list->sentinel)) {
         return NULL;
    }
    /* Link the node to remove's next and previous to each other. */
    node_to_remove->prev->next = node_to_remove->next;
    node_to_remove->next->prev = node_to_remove->prev;
    
    return node_to_remove;
}

/**
 * @brief Destroys the list and frees all allocated memory.
 * @param list Pointer to the list to destroy.
 */
void list_destroy(List* list) {
    // Short circuit if there is no list to destroy.
    if (!list) {
        return;
    }
    // Iterate through list to clean-up each node in it.
    Node* current = list->sentinel->next;
    while (current != list->sentinel) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    // Memory clean-up.
    free(list->sentinel);
    free(list);
}