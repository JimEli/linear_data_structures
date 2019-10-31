#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <iostream>  // cout
#include <memory>    // smart pointer
#include <exception> // exceptions

template <typename T>
class queue 
{
	std::shared_ptr<node> head = nullptr, tail = nullptr;

protected:
	struct node
	{
		template <typename T> friend class queue;
		
		explicit node(T e) { element = e; }

		T element;
		std::shared_ptr<node> next = nullptr;
	};

public:
	Queue() : head(nullptr), tail(nullptr), count(0) { }

	bool empty() const { return head == nullptr; }

	// Clear the list.
	void clear()
	{
		while (!empty())
			pop_front();
		head = tail = nullptr;
	}

	T& front() const { return this->head->element; }

	T& back() const
	{
		if (empty())
			throw std::out_of_range("queue empty");
		return tail->element;
	}

	// Adds new node to tail of queue.
	void enqueue(const T& e)
	{
		auto newNode{ std::make_shared<node>(e) };
		if (head == nullptr) // Update head pointer.
			head = newNode;
		if (tail)            // Update tail pointer.
			tail->next = newNode;
		tail = newNode;
	}

	// Deletes head node.
	void dequeue()
	{
		if (empty())
			throw std::out_of_range("queue empty");
		std::shared_ptr<node> temp{ head };
		head = head->next;
		if (tail == temp)
			tail = nullptr;
	}

	// Removes node from head of queue.
	void pop_front()
	{
		if (empty())
			return;
		auto temp{ head };
		head = head->next;
		if (tail == temp)
			tail = nullptr;
	}

	// Adds new node to head of list.
	void push_front(const T& e)
	{
		auto newNode{ std::make_shared<node>(e) };
		newNode->next = head;
		head = newNode;
		if (tail == nullptr)
			tail = newNode;
	}

	// Overloaded output operator to display list contents.
	friend std::ostream& operator<< (std::ostream& os, const list<T>& list)
	{
		for (auto node = list.head; node; node = node->next)
			os << node->element;
		return os << std::endl;
	}

	// Iterator class. Member typedefs provided through inheritance of std::iterator.
	class iterator : public std::iterator<std::forward_iterator_tag, T>
	{
	private:
		std::shared_ptr<node> pNode;

		// Ctor is private, so only friends can create instances.
		iterator(std::shared_ptr<node> n) : pNode(n) { }
		iterator() : pNode(nullptr) { }

		friend class Queue;

	public:
		bool operator== (const iterator& it) const { return pNode == it.pNode; }
		bool operator!= (const iterator& it) const { return pNode != it.pNode; }
	
		T& operator* () { return pNode->element; }
		T* operator-> () { return &pNode->element; }
		
		iterator& operator++ ()
		{
			pNode = pNode->next;
			return *this;
		}

	}; // End iterator inner class.

	iterator begin() const { return iterator(head); }
	iterator end() const { return iterator(tail); }
};

#endif
