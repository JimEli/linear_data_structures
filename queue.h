#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <iostream>  // cout
#include <memory>    // smart pointer
#include <exception> // exceptions

template <typename T>
class queue 
{
protected:
	template <typename T>
	struct Node
	{
		Node(T e) { element = e; }
		T element;
		std::shared_ptr<Node<T>> next = nullptr;
	};

public:
	Queue() : head(nullptr), tail(nullptr), count(0) { }

	T& front() const { return this->head->element; }
	T& back() const
	{
		if (empty())
			throw std::out_of_range("queue empty");
		return tail->element;
	}

	void enqueue(const T& e)
	{	// Adds new node to tail of queue.
		auto newNode{ std::make_shared<Node<T>>(e) };
		if (head == nullptr) // Update head pointer.
			head = newNode;
		if (tail)            // Update tail pointer.
			tail->next = newNode;
		tail = newNode;
	}

	void dequeue()
	{	// Deletes head node.
		if (empty())
			throw std::out_of_range("queue empty");
		std::shared_ptr<Node<T>> temp{ head };
		head = head->next;
		if (tail == temp)
			tail = nullptr;
	}

	void pop_front()
	{	// Removes node from head of queue.
		if (empty())
			return;
		auto temp{ head };
		head = head->next;
		if (tail == temp)
			tail = nullptr;
	}

	void clear()
	{	// Clear the list.
		while (!empty())
			pop_front();
		head = tail = nullptr;
	}

	bool empty() const { return head == nullptr; }

	void push_front(const T& e)
	{	// Adds new node to head of list.
		auto newNode{ std::make_shared<Node<T>>(e) };
		newNode->element = e;
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
		std::shared_ptr<Node<T>> pNode;

		// Ctor is private, so only friends can create instances.
		iterator(std::shared_ptr<Node<T>> n) : pNode(n) { }
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
	//iterator end() const { return iterator(tail); }
	iterator end() const { return iterator(tail->next); }

private:
	std::shared_ptr<Node<T>> head = nullptr, tail = nullptr;
};

#endif
