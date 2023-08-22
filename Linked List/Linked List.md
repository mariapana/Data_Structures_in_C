# Linked List

# What is a linked list?

A linked list is a **linear data structure**, in which elements are **not stored in contiguous memory** locations. 

It consists of a **chain of connected nodes**, which contain the data itself and a pointer to the next node in the list.

![https://media.geeksforgeeks.org/wp-content/uploads/20220816144425/LLdrawio.png](https://media.geeksforgeeks.org/wp-content/uploads/20220816144425/LLdrawio.png)

# Types

- Singly Linked Lists: head; data + next
- Doubly Linked lists: head; data + prev + next
- Circular Linked lists: head, tail; data + next
- Combos

# Why use a Linked List?

- Easy insertion / deletion of elements
- Dynamic allocation (instead of a statically allocated array)
- Good for storing data you don’t need to access frequently (since searching is expensive)

# API

| ll_add_first() | insert element at the beginning of the list | O(1) |
| --- | --- | --- |
| ll_add_last() | insert element at the end of the list | O(n) / O(1) if pointer to tail |
| ll_get_nth_node() | access nth node | O(n) |
| ll_size() | number of elements in list | O(1) |

# LinkedList vs. ArrayList

| Memory Map | contiguous 
✓ faster caching | random
✗ more cache misses
✓ better use of limited memory |
| --- | --- | --- |
| Used memory | less | more (includes pointers) |
| Element access | O(1)
v[ i ] | O(n)
needs ll traversal |
| Insert element start | O(n)
shift (+realloc) | O(1)
directly relink |
| Insert element end | O(1)
(realloc) | O(n) / O(1) if pointer to tail
(traversal +) relink |
| Insert element random | O(n)
shift (+ realloc) | O(n)
traversal + relink |
| Remove element start | O(n)
shift (+ realloc) | O(1) |
| Remove element end | O(1) | O(n) / O(1) if pointer to tail |
| Remove element random | O(n)
shift (+ realloc) | O(n)
traversal + relink |
| Searching | O(nlogn)
binary search | O(n)
only sequential search |

# Use cases

- implementation of other data structures (stack, hashtable, graphs etc.)
- blockchain
- web navigation buttons/arrows (doubly linked lists)

etc.