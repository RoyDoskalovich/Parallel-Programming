// Roy Doskalovich 319088548
// Tal Achizvi 313555088

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <omp.h>
#include "linkedList.h"


// Node structure for the linked list.
typedef struct node_s { 
    int value;  
    struct node_s *next;
    omp_lock_t *lock;   // Lock for the node.
} node_t;


// Initializes the list.
node_t *init_list() {   
    node_t *head = malloc(sizeof(node_t));      
    head->next = NULL;  
    head->value = INT_MIN; // Set the head value to the smallest possible integer.
    head->lock = malloc(sizeof(omp_lock_t));  // Initialize the lock for the head node.
    omp_init_lock(head->lock);      // Initialize the lock.
    return head;
}


void sorted_insert(node_t *head, int val) {
    /* Find the right place to insert */
    node_t *ptr = head->next;
    node_t *prev = head;

    while (ptr != NULL && ptr->value < val) {
        prev = ptr;
        ptr = ptr->next;
    }

    /* Lock the element that may be modified */
    omp_set_lock(prev->lock);

    /* Confirm that the element is still in the right place */
    if (prev->next != ptr) {
        ptr = prev->next;
        while (ptr != NULL && ptr->value < val) {
            omp_set_lock(ptr->lock);
            omp_unset_lock(prev->lock);
            prev = ptr;
            ptr = ptr->next;
        }
    }

    /* Insert the new node */
    node_t *new_node = malloc(sizeof(node_t));
    new_node->value = val;
    new_node->next = ptr;
    new_node->lock = malloc(sizeof(omp_lock_t));
    omp_init_lock(new_node->lock);
    prev->next = new_node;
    omp_unset_lock(prev->lock);
}


// Returns 1 if the value is found, 0 otherwise.
int find_val(node_t *head, int val) {
    node_t *ptr = head->next;   // Skip the head node. 

    while (ptr != NULL) {   
        if (ptr->value == val) return 1;    // If the value is found, return 1.
        ptr = ptr->next; 
    }

    return 0; 
}

// Returns the length of the list.
int get_len(node_t *head){
    int length = 0;
    node_t *ptr = head->next;   // Skip the head node.

    while (ptr != NULL){
        length++;
        ptr = ptr->next;
    }
    
    return length;
}


// Removes the first instance of the value from the list.
void remove_val(node_t *head, int val) {
    omp_set_lock(head->lock);   // Lock the head node.
    node_t *prev = head;
    node_t *current = head->next;
    while (current != NULL && current->value != val) {  // Find the value to remove.
        omp_set_lock(current->lock);    // Lock the current node.
        omp_unset_lock(prev->lock);     // Unlock the previous node.
        prev = current;
        current = current->next;
    }
    if (current != NULL) {  // If the value is found, remove it.
        prev->next = current->next; // Remove the node from the list.
        omp_unset_lock(current->lock);  // Unlock the current node.
        free(current);  // Free the memory.
    }
    omp_unset_lock(prev->lock); // Unlock the previous node.
}


// Frees the memory of the list.
void free_list(node_t *head) {
    node_t *ptr = head;

    while (ptr != NULL){
        node_t *temp = ptr;
        ptr = ptr->next;
        free(temp->lock);   // Free the lock.
        free(temp); // Free the node.
    }
}


// void print_list(node_t *head) {
//     node_t *current = head;
//     while (current) {
//         printf("%d -> ", current->value);
//         current = current->next;
//     } 
// }
