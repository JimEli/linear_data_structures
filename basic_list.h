#ifndef _BASIC_LIST_H_
#define _BASIC_LIST_H_

#include <iostream>  // cout

template <typename T>
class list 
{
	struct node
	{
		explicit node(T e) : next(nullptr) { element = e; }

		T element;
		node* next;
		
		template <typename T> friend class list;
	};

	node *head, *tail;

public:
	list() : head(nullptr), tail(nullptr) { }
	
	~list() { clear(); }

	void clear()
	{
		while (!empty())
			pop_front();

		head = tail = nullptr;
	}

	std::size_t size() const
	{
		std::size_t size = 0;
		for (const node *node = head; node; node = node->next, size++);
		return size;
	}

	bool empty() const { return head == nullptr; }

	T& front() const 
	{
		if (!empty())
			return head->element;
		else
			throw std::out_of_range("empty list");
	}

	T& back() const 
	{ 
		if (!empty())
			return tail->element;
		else
			throw std::out_of_range("empty list");
	}

	void push_back(const T& e)
	{
		node *newNode = new node(e);
		
		if (!head)
			head = newNode;
		
		if (tail)
			tail->next = newNode;
		
		tail = newNode;
	}

	void push_front(const T& e)
	{
		node *newNode = new node(e);
		
		if (!tail)
			tail = newNode;
		
		newNode->next = head;
		head = newNode;
	}

	void pop_front()
	{
		if (empty())
			return;
		
		node *temp = head;
		
		head = head->next;
		
		if (tail == temp)
			tail = nullptr;
		
		delete temp;
	}

	bool find(T d) const
	{
		node *curr = head;
		while (curr != nullptr)
		{
			if (curr->element == d)
				return true;
			curr = curr->next;
		}
		return false;
	}

	bool remove(T d)
	{
		node *prev = head;
		node *curr = head;

		while (curr != nullptr)
		{
			if (curr->element == d)
				break;
			else
			{
				prev = curr;
				curr = curr->next;
			}
		}

		if (curr == nullptr)
			return false;
		
		else
		{
			if (head == curr)
				head = curr->next;
		
			if (tail == curr)
				tail = prev;
			
			prev->next = curr->next;
			
			delete curr;
		}

		return true;
	}

	void reverse()
	{
		node *prev = nullptr, *curr = head, *next = nullptr;

		while (curr != nullptr)
		{
			next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}

		std::swap(head, tail);
	}

		friend std::ostream& operator<< (std::ostream& os, const list<T>& list)
	{
		for (const node *node = list.head; node; node = node->next)
			os << node->element;
		
		return os << std::endl;
	}

	// Inner iterator class. Member typedefs provided through inheritance from std::iterator.
	class iterator : public std::iterator<std::forward_iterator_tag, T>
	{
	private:
		node *pnode = nullptr;

		// Ctor is private, so only friends can create instances.
		iterator(node *n) : pnode(n) { }

		friend class list;

	public:
		// Overload comparison operators.
		bool operator== (const iterator& it) const { return pnode == it.pnode; }
		bool operator!= (const iterator& it) const { return pnode != it.pnode; }

		// Overload dereference and pointer operators.
		T& operator* () { return pnode->element; }
		T* operator-> () { return &pnode->element; }

		// Overload prefix increment operator.
		iterator& operator++ ()
		{
			pnode = pnode->next;
			return *this;
		}
	}; // End iterator inner class.

	// Begin and end iterators.
	iterator begin() const { return iterator(head); }
	iterator end() const { return iterator(tail->next); } // { return iterator(tail); }
};

#endif
