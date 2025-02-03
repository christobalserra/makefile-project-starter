/**
 * @file lab.c
 * @brief Implementation of a circular doubly linked list.
 *
 * A circular doubly linked list is a linked list that can be iterated 
 * forward or back, and loops back around to itself.
 * 
 */

#include "lab.h"
#include <stdio.h>
#include <stdlib.h>

/**
* @brief Create a new list with callbacks that know how to deal with the data that
* list is storing. The caller must pass the list to list_destroy when finished to
* free any memory that was allocated.
* @param destroy_data Function that will free the memory for user supplied data
* @param compare_to Function that will compare two user data elements
* @return struct list* pointer to the newly allocated list.
*/
list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    // Allocate memory for the list.
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (!list) {
        fprintf(stderr, "Memory allocation failed for the list.\n");
        return NULL;
    }
    // Set function pointers to help with the management of the data it is storing. 
    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    // Set size of list. 0 when first initialized since nothing added yet.
    list->size = 0;
    // Allocate memory for the sentinel node. Head will remain the sentinel.
    list->head = (node_t *)malloc(sizeof(node_t));
    if (!list->head) {
        fprintf(stderr, "Memory allocation failed for the sentinel node.\n");
        free(list);
        return NULL;
    }
    list->head->data = NULL; // sentinel is dummy node with no data
    list->head->next = list->head; // sentinel next is itself
    list->head->prev = list->head; // sentinel prev is itself
    return list;
}

/**
 * @brief Destroy the list and and all associated data. This functions will call
 * destroy_data on each nodes data element.
 * @param list a pointer to the list that needs to be destroyed
 */
void list_destroy(list_t **list) {
    // Short circuit if there is no list to destroy.
    if (!list || !(*list)) {
        return;
    }
    // Iterate through list to clean-up each node in it.
    node_t *current = (*list)->head->next;
    while (current != (*list)->head) {
        node_t *nxt = current->next; 
        // Free stored data if provided, using builtin function for init.
        if ((*list)->destroy_data && current->data) {
            (*list)->destroy_data(current->data);
        }
        // Advance
        free(current);
        current = nxt; 
    }
    // List memory clean-up.
    free((*list)->head);
    free(*list);
    *list = NULL;
}

/**
 * Adds data to the front (head) of the list.
 *
 * @param list a pointer to an existing list.
 * @param data the data to add
 * @return A pointer to the list
 */
list_t *list_add(list_t *list, void *data) {
     // If there's no list or no data, short-circuit.
    if (!list || !data) {
        return NULL;
    }
    // Create node.
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed for the new node.\n");
        return NULL;
    }
    // Add the node's data.
    new_node->data = data;
    /* Insert the new node before the sentinel, which is the end of the list since the linked list is circular and doubly linked. */
    new_node->next = list->head->next; // new node's next is the first node
    new_node->prev = list->head; // new node's previous is the sentinel
    list->head->next->prev = new_node; // first node's previous is now new node
    list->head->next = new_node; // sentinel's next is now new node
    // Increment size and return the list.
    list->size++;
    return list;
}

/**
 * @brief Removes the data at the specified index. If index is invalid
 * then this function does nothing and returns NULL
 *
 * @param list The list to remove the element from
 * @param index The index
 * @return void* The data that was removed or NULL if nothing was removed
 */
void *list_remove_index(list_t *list, size_t index) {
     // Short-circuit if no list or list is empty.
    if (!list || index >= list->size) {
        return NULL;
    }
    // Iterate through the list to find the node in the list. 
    node_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    // Get the data from the node we are removing, so we can return it later.
    void *data = current->data;
    /* Link the node to remove's next and previous to each other. */
    current->prev->next = current->next;
    current->next->prev = current->prev;
    // Memory clean-up, size decrement, and return data.
    free(current);
    list->size--;
    return data;
}

/**
 * @brief Search for any occurrence of data from the list.
 * Internally this function will call compare_to on each item in the list
 * until a match is found or the end of the list is reached. If there are
 * multiple copies of the same data in the list the first one will be returned.
 *
 * @param list the list to search for data
 * @param data the data to look for
 * @return The index of the item if found or -1 if not
 */
int list_indexof(list_t *list, void *data) {
    // If there is no list or the data is invalid, short-circuit with -1.
    if (!list || !data) {
        return -1;
    }
    // Iterate from first node through the list until we find the queried data or sentinel.
    node_t *current = list->head->next;
    int index = 0;
    while (current != list->head) {
        if (list->compare_to(current->data, data) == 0) { // data found
            return index;
        }
        // Advance
        current = current->next;
        index++;
    }
    return -1; // data not found
}