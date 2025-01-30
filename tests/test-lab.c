
#include "harness/unity.h"
#include "../src/lab.h"

List* list;

Node* add_data(List* list, int data) {
    list_add_node(list, &data);
    return list->sentinel->prev; // Return new node (which is always the one before sentinel)
}

void setUp(void) {
    list = create_list();
}

void tearDown(void) {
    list_destroy(list);
}

void test_create_list(void) {
    TEST_ASSERT_NOT_NULL(list);  // list is created and is not NULL
    TEST_ASSERT_NOT_NULL(list->sentinel);  // sentinel node is created and is not NULL
    // List is empty, so...
    TEST_ASSERT_EQUAL_PTR(list->sentinel->next, list->sentinel);  // sentinel's next is self
    TEST_ASSERT_EQUAL_PTR(list->sentinel->prev, list->sentinel);  // sentinel's prev is self
}

void test_add1(void) {
    int data = 42; // data to add, meaning of life
    Node* new_node = add_data(list, &data); // get the new node from the sentinel

    TEST_ASSERT_NOT_EQUAL(new_node, list->sentinel); // new node is not sentinel
    TEST_ASSERT_EQUAL_PTR(new_node->data, &data); // new node has the data pointer we want.
}

void test_add2(void) {
    int data1 = 1;
    int data2 = 2;
    Node* first_node = add_data(list, &data1);
    Node* second_node = add_data(list, &data2);

    TEST_ASSERT_NOT_EQUAL(first_node, second_node); // different nodes
    TEST_ASSERT_EQUAL_PTR(first_node->data, &data1); // first node has first data
    TEST_ASSERT_EQUAL_PTR(second_node->data, &data2); // second node has second data
}

void test_remove_empty(void) {
    // Empty list
    Node* removed_node = list_remove(list, list->sentinel->next);
    TEST_ASSERT_NULL(removed_node);  // No node should be removed (since the list is empty)
}

void test_remove1(void) {
    // Add the node, so we can remove it after.
    int data = 9;
    Node* new_node = add_data(list, &data);
    // Do the removal.
    list_remove(list, new_node);

    TEST_ASSERT_EQUAL_PTR(list->sentinel->next, list->sentinel);  // list is empty again
}

void test_remove2(void) {
    int data1 = 10;
    int data2 = 20;
    Node* first_node = add_data(list, &data1);
    Node* second_node = add_data(list, &data2);

    // Remove the first node and test.
    Node* removed_node = list_remove(list, first_node);
    TEST_ASSERT_EQUAL_PTR(removed_node, first_node);  // Correct node is removed
    TEST_ASSERT_EQUAL_PTR(list->sentinel->next, second_node);  // Second node is now the first node
    TEST_ASSERT_EQUAL_PTR(second_node->next, list->sentinel);  // Second node points to sentinel

    // Add a node back and remove the tail one (third, since we add to tail) this time.
    int data3 = 30;
    Node* third_node = add_data(list, &data3);
    Node* removed_node = list_remove(list, third_node);
    TEST_ASSERT_EQUAL_PTR(removed_node, third_node);
    TEST_ASSERT_EQUAL_PTR(list->sentinel->prev, second_node);  // Second node is now the tail node
    TEST_ASSERT_EQUAL_PTR(second_node->next, list->sentinel);  // Second node points to sentinel

    // Remove last node.
    Node* removed_node = list_remove(list, second_node);
    TEST_ASSERT_EQUAL_PTR(list->sentinel->next, list->sentinel);  // list is empty again
}

void test_remove3(void) {
    int data1 = 7;
    int data2 = 8;
    int data3 = 9;
    Node* first_node = add_data(list, &data1);
    Node* second_node = add_data(list, &data2);
    Node* third_node = add_data(list, &data3);

    // Remove the first node and test.
    Node* removed_node = list_remove(list, first_node);
    TEST_ASSERT_EQUAL_PTR(removed_node, first_node);  // Correct node is removed
    TEST_ASSERT_EQUAL_PTR(list->sentinel->next, second_node);  // Second node is now the first
    TEST_ASSERT_EQUAL_PTR(second_node->next, third_node);  // Second node points to third node
    TEST_ASSERT_EQUAL_PTR(third_node->next, list->sentinel);  // Third node points to sentinel

    // Add a node back and remove middle one this time.
    int data4 = 14;
    Node* fourth_node = add_data(list, &data4);
    TEST_ASSERT_EQUAL_PTR(removed_node, third_node);
    TEST_ASSERT_EQUAL_PTR(second_node->next, fourth_node);  // Second node points to fourth node
    TEST_ASSERT_EQUAL_PTR(fourth_node->prev, second_node);  // Fourth node points to second

    // Add a node back and remove tail one this time.
    int data5 = 16;
    Node* fifth_node = add_data(list, &data5);
    TEST_ASSERT_EQUAL_PTR(removed_node, fifth_node);
    TEST_ASSERT_EQUAL_PTR(list->sentinel->prev, fourth_node);  // Tail is now fourth node
    TEST_ASSERT_EQUAL_PTR(list->sentinel, fourth_node->next); // Fourth points to sentinel

    // Remove last nodes.
    Node* removed_node = list_remove(list, second_node);
    Node* removed_node = list_remove(list, fourth_node);
    TEST_ASSERT_EQUAL_PTR(list->sentinel->next, list->sentinel);  // list is empty again

}

void test_list_destroy(void) {
    list_destroy(list);
    // TODO - There should be no memory leaks with valgrind or makefile. Check for that.
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_list);
    RUN_TEST(test_add1);
    RUN_TEST(test_add2);
    RUN_TEST(test_remove1);
    RUN_TEST(test_remove2);
    RUN_TEST(test_remove3);
    RUN_TEST(test_list_destroy);
    return UNITY_END();
}
