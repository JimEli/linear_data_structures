/*************************************************************************
* Title: Queue
* File: queue.h
* Author: James Eli
* Date: 10/26/2018
*
* Basic queue implemented as static circular buffer using a smart pointer.
*
* Notes:
*  (1) Circular buffer concepts researched at Chapter 7. Boost.Circular
*      Buffer located here:
*      https://www.boost.org/doc/libs/1_61_0/doc/html/circular_buffer.html
*  (2) Queue (circular buffer) size is fixed at compile time by QUEUE_SIZE
*      constant.
*  (3) Note: when queue (circular buffer) is full, further calls to enqueue
*      are ignored. Many circular buffers continue by overwritting data.
*  (4) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0 (32 & 64-bit), and with Eclipse
*      Oxygen.3a Release (4.7.3a), using CDT 9.4.3/MinGw32 gcc-g++ (6.3.0-1).
*************************************************************************
* Change Log:
*  10/21/2018: Initial release. JME
*  10/26/2018: Added size template parameter.  JME
*  10/26/2018: Added smart pointer.  JME
*************************************************************************/
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <exception> // out of range
#include <memory>    // smart pointer

// Default size of queue array if not specified during instantiation.
constexpr std::size_t DEFAULT_QUEUE_SIZE = 16;

template<class T, std::size_t QUEUE_SIZE = DEFAULT_QUEUE_SIZE>
class queue
{
private:
	std::unique_ptr<T[]> data; // Array of queue elements.
	size_t head;               // Elements popped from this array index.
	size_t tail;               // Elements pushed to this array index.
	bool full;                 // True if queue array is full.

public:
	queue() : head(0), tail(0), full(false) { data = std::make_unique<T[]>(QUEUE_SIZE); }
	~queue() = default;

	bool enqueue(T val)
	{
		if (full)                       // Check if queue full.
			return false;
		data[tail] = val;               // Insert value into queue.
		tail = (tail + 1) % QUEUE_SIZE; // Increment pointer, wrap if necessary.
		full = (tail == head);          // Queue full?
		return true;
	}

	bool dequeue()
	{
		if (empty())	                // Check if empty.
			return false;
		full = false;                   // Queue can not be full.
		head = (head + 1) % QUEUE_SIZE; // Increment pointer (wrap if necessary).
		return true;
	}
/*
	// Alternative version of dequeue which throws oor exception.
	T dequeue()
	{
		if (empty())
			throw std::out_of_range("empty queue");
		T tmp = data[head];
		full = false;                   // Queue can not be full.
		head = (head + 1) % QUEUE_SIZE; // Increment pointer (wrap if necessary).
		return tmp;
	}
*/
	bool empty() { return (!full && (tail == head)); }
	bool isFull() { return full; }

	T front()
	{
		if (empty())
			throw std::out_of_range("empty queue");
		else
			return data[head];
	}

	T back()
	{
		if (empty())
			throw std::out_of_range("empty queue");
		else
			return data[(tail ? tail - 1 : QUEUE_SIZE - 1)];
	}
};

#endif
