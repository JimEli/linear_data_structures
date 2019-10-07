# C++ Linear Data Structures

Lean and mean header only include files with minimal commenting and no error checking.

* array
* vector
* singly-linked-list (using raw pointers)
* singly-linked-list (using smart pointers)
* stack
* queue

## Basic Feature Descriptions

### Array
* Templated class with a fixed, default size of 16.
* Forward and reverse iterator support.
* At method performs range check.
* Methods/operators include: [], begin, end, front, back, size, max_size, empty, swap, fill, =, !=, ==, >=, <=, >, <.

### Vector
* Templated class.
* Methods/operators include: ctor, copy ctor, clear, =,  push_back, pop_back, size, and [].

### Singly-Linked List
* Templated class.
* Forward iterator support.
* Methods/operators include: <<, clear, size, empty, front, back, push_back, push_front, pop_front, find, remove and reverse.

### Stack
* Templated class.
* Methods include: push, pop, top, empty and size.

### Queue
* Templated class.
* Forward iterator support.
* Methods/operators include: ctor, front, back, enqueue, dequeue, pop_front, push_front, clear, empty, <<.

