# C++ Linear Data Structures

A collection of lean and mean header only include files with minimal commenting and little to no error checking.

* array
* vector
* singly-linked-list (using raw pointers)
* singly-linked-list (using smart pointers)
* stack
* queue
* min heap
* binary heap (priority queue)
* priority queue

## Basic Feature Descriptions

### Array
* Templated class with a fixed, default size of 16.
* Forward and reverse iterator support.
* At method performs range check.
* Methods/operators include: [], begin, end, front, back, size, max_size, empty, swap, fill, =, !=, ==, >=, <=, >, <.

### Vector
* Templated class.
* Forward and reverse iterator support.
* Methods/operators include: ctor, copy and move ctor, clear, =, push_back, pop_back, front, back, size, max_size, capacity, reserve, resize, shrink_to_fit, [], at, data, insert, erase, assign, swap.

### Singly-Linked List
* Templated class.
* Forward iterator support.
* Methods/operators include: <<, clear, size, empty, front, back, push_back, push_front, pop_front, find, remove and reverse (smart pointer version includes resize, insert_after, emplace_front, emplace_back and erase_after).

### Stack
* Templated class.
* Methods include: push, pop, top, empty and size.

### Queue
* Templated class.
* Forward iterator support.
* Methods/operators include: ctor, front, back, enqueue, dequeue, pop_front, push_front, clear, empty, <<.

### Min Heap
* Templated class.
* Methods/operators include: ctor, push, pop, top, size, empty.

### Binary Heap (Priority Queue)
* Templated class.
* Methods/operators include: ctor, push, pop, top, topKey, topItem, size, empty, <<.

### Priority Queue
* Templated class.
* Built on top of STL vector and heap functions.
* Methods include: ctor, push, pop, top size, empty, swap.
