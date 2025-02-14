# Parallel Linked List with OpenMP

## Overview
This subdirectory contains an **optimized linked list implementation** that supports **parallel operations** using **OpenMP locks** to ensure thread safety.

### Features
- **Thread-Safe Insertions & Deletions**: Uses OpenMP locks for safe concurrent modifications.
- **Sorted Insertion**: Ensures the list remains sorted at all times.
- **Efficient Search & Removal**: Optimized traversal with locking mechanisms.
- **Dynamic Memory Management**: Proper allocation and deallocation to prevent memory leaks.

### Files
#### `linkedlist.c`
- `init_list()`: Initializes a new linked list with a head node.
- `sorted_insert()`: Inserts a value while maintaining order.
- `find_val()`: Searches for a value in the list.
- `get_len()`: Returns the length of the list.
- `remove_val()`: Removes a value safely.
- `free_list()`: Frees all allocated memory.

#### `linkedlist.h`
Defines the linked list structure and function prototypes.

## Usage
1. Compile using:
   ```sh
   gcc -fopenmp -o linkedlist linkedlist.c
   ```
2. Run the program:
   ```sh
   ./linkedlist
   ```

   
