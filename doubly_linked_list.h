// Basic doubly-linked list.
// 8.19.2018 JME
#ifndef DL_LIST_H
#define DL_LIST_H

#include <iostream>

template<typename T>
class dlist
{
private:
	struct Node 
	{ 
		T data;
		Node *prev, *next;
	
		// Copy ctor.
		Node(const T& d = T{}, Node* p = nullptr, Node* n = nullptr) : data{ d }, prev{ p }, next{ n } { }
		// Move ctor.
		Node(T&& d, Node* p = nullptr, Node* n = nullptr) : data{ std::move(d) }, prev{ p }, next{ n } { }
	};

public: 
	// Nested const_iterator class.
	class const_iterator 
	{
	public:
		const_iterator();
		const T& operator*() const;
		const_iterator& operator++ ();
		const_iterator operator++ (int);
		const_iterator& operator-- ();
		const_iterator operator-- (int);
		bool operator== (const const_iterator& rhs) const;
		bool operator!= (const const_iterator& rhs) const;

	protected:
		Node* current; // Current node const_iterator points at.
		const_iterator(Node* p);

		friend class dlist<T>;
	};

	// Nested iterator class.
	class iterator : public const_iterator
	{
	public:
		iterator();
		T& operator*();
		const T& operator*() const;

		iterator& operator++ ();
		iterator operator++ (int);
		iterator& operator-- ();
		iterator operator-- (int);

	protected:
		iterator(Node* p);
		friend class dlist<T>;
	};

private:
	void init() 
	{
		size_ = 0;
		head = new Node;
		tail = new Node;
		head->next = tail;
		tail->prev = head;
	}

public:
	// Ctor.
	dlist() { init(); }
	// Copy ctor.
	dlist(const dlist<T>& rhs)
	{
		init();
		for (auto it = rhs.begin(); it != rhs.end(); ++it)
			push_back(*it);
	}
	// Move ctor.
	dlist(dlist<T>&& rhs) : size_(rhs.size_), head{ rhs.head }, tail{ rhs.tail }
	{
		rhs.size_ = 0;
		rhs.head = nullptr;
		rhs.tail = nullptr;
	}
	// Copy assignment.
	const dlist<T>& operator= (const dlist<T>& rhs)
	{
		auto copy = rhs;
		std::swap(*this, copy);
		return *this;
	}
	// Move assignment.
	dlist<T>& operator= (dlist<T>&& rhs)
	{
		init();
		*this = std::move(rhs);
		return *this;
	}

	~dlist()
	{
		clear();
		delete head;
		delete tail;
	}

	int size() const;
	bool empty() const;
	void clear();
	void reverse();

	T& front();
	const T& front() const;
	T& back();
	const T& back() const;

	void push_front(const T& val);
	void push_front(T&& val);
	void push_back(const T& val);
	void push_back(T&& val);
	void pop_front();
	void pop_back();

	void remove(const T& val);

	void print(std::ostream& os, char ofc = ' ') const;

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	iterator insert(iterator it, const T& val);
	iterator insert(iterator it, T&& val);
	iterator erase(iterator it);
	// Erase from start to end, not including end, erase [start, end).
	iterator erase(iterator start, iterator end);

private:
	int size_;  // Current number of nodes, except for head and tail.
	Node *head, *tail; // Tail node.
};

template<typename T>
bool operator== (const dlist<T>& lhs, const dlist<T>& rhs);
template<typename T>
bool operator!= (const dlist<T>& lhs, const dlist<T>& rhs);

template<typename T>
std::ostream& operator<< (std::ostream& os, const dlist<T>& lst);
	
template<typename T>
dlist<T>::const_iterator::const_iterator() : current{ nullptr } { }

template<typename T>
const T& dlist<T>::const_iterator::operator* () const { return current->data; }

template<typename T>
typename dlist<T>::const_iterator & dlist<T>::const_iterator::operator++ ()
{
	current = current->next;
	return *this;
}

template<typename T>
typename dlist<T>::const_iterator dlist<T>::const_iterator::operator++ (int)
{
	auto copy = *this;
	current = current->next;
	return copy;
}

template<typename T>
typename dlist<T>::const_iterator& dlist<T>::const_iterator::operator-- ()
{
	current = current->prev;
	return *this;
}

template<typename T>
typename dlist<T>::const_iterator dlist<T>::const_iterator::operator-- (int)
{
	auto copy = *this;
	current = current->prev;
	return *this;
}

template<typename T>
bool dlist<T>::const_iterator::operator== (const typename dlist<T>::const_iterator& rhs) const
{
	return current == rhs.current;
}
template<typename T>
bool dlist<T>::const_iterator::operator!= (const typename dlist<T>::const_iterator& rhs) const
{
	return current != rhs.current;
}

template<typename T>
dlist<T>::const_iterator::const_iterator(typename dlist<T>::Node* p) : current{ p } { }
template<typename T>
dlist<T>::iterator::iterator() { }

template<typename T>
T& dlist<T>::iterator::operator* () { return this->current->data; }
template<typename T>
const T& dlist<T>::iterator::operator* () const { return this->current->data; }

template<typename T>
typename dlist<T>::iterator& dlist<T>::iterator::operator++ ()
{
	this->current = this->current->next;
	return *this;
}
template<typename T>
typename dlist<T>::iterator dlist<T>::iterator::operator++ (int)
{
	auto copy = *this;
	this->current = this->current->next;
	return copy;
}
template<typename T>
typename dlist<T>::iterator& dlist<T>::iterator::operator-- ()
{
	this->current = this->current->prev;
	return *this;
}
template<typename T>
typename dlist<T>::iterator dlist<T>::iterator::operator-- (int)
{
	auto copy = *this;
	this->current = this->current->prev;
	return *this;
}

template<typename T>
dlist<T>::iterator::iterator(typename dlist<T>::Node* p) : dlist<T>::const_iterator{ p } { }

template<typename T>
int dlist<T>::size() const { return size_; }
template<typename T>
bool dlist<T>::empty() const { return size() == 0; }

template<typename T>
T& dlist<T>::front() { return *begin(); }
template<typename T>
const T & dlist<T>::front() const { return *begin(); }
template<typename T>
T& dlist<T>::back() { return *(--end()); }
template<typename T>
const T& dlist<T>::back() const { return *(--end()); }

template<typename T>
void dlist<T>::push_front(const T& val) { insert(begin(), val); }
template<typename T>
void dlist<T>::push_front(T&& val) { insert(begin(), std::move(val)); }
template<typename T>
void dlist<T>::push_back(const T& val) { insert(end(), val); }
template<typename T>
void dlist<T>::push_back(T&& val) { insert(end(), std::move(val)); }
template<typename T>
void dlist<T>::pop_front() { erase(begin()); }
template<typename T>
void dlist<T>::pop_back() { erase(--end()); }

template<typename T>
void dlist<T>::remove(const T& t)
{
	for (auto it = begin(); it != end(); ++it)
		if (*it == t) 
		{
			it = erase(it);
			it--; // Backup 1 node.
		}
}

template<typename T>
typename dlist<T>::iterator dlist<T>::begin()
{
	if (!empty())
	{
		typename dlist<T>::iterator it{ head->next };
		return it;
	}
	else
		return nullptr;
}
template<typename T>
typename dlist<T>::const_iterator dlist<T>::begin() const
{
	if (!empty())
	{
		typename dlist<T>::const_iterator constIt{ head->next };
		return constIt;
	}
	else
		return nullptr;
}

template<typename T>
typename dlist<T>::iterator dlist<T>::end()
{
	typename dlist<T>::iterator it{ tail };
	return it;
}
template<typename T>
typename dlist<T>::const_iterator dlist<T>::end() const
{
	typename dlist<T>::const_iterator constIt{ tail };
	return constIt;
}

template<typename T>
typename dlist<T>::iterator dlist<T>::insert(typename dlist<T>::iterator it, const T& val)
{
	auto* p = it.current;

	size_++;

	auto* nptr = new dlist<T>::Node{ val, p->prev, p };

	p->prev->next = nptr;
	p->prev = nptr;

	typename dlist<T>::iterator iter{ nptr };

	return iter;
}

template<typename T>
typename dlist<T>::iterator dlist<T>::insert(typename dlist<T>::iterator it, T&& val)
{
	auto* p = it.current;

	size_++;

	auto* nptr = new dlist<T>::Node{ std::move(val), p->prev, p };

	p->prev->next = nptr;
	p->prev = nptr;

	typename dlist<T>::iterator iter{ nptr };

	return iter;
}

template<typename T>
typename dlist<T>::iterator dlist<T>::erase(typename dlist<T>::iterator it)
{
	auto* p = it.current;
	typename dlist<T>::iterator retVal{ p->next };

	p->prev->next = p->next;
	p->next->prev = p->prev;

	delete p;
	size_--;

	return retVal;
}

template<typename T>
typename dlist<T>::iterator dlist<T>::erase(typename dlist<T>::iterator start, typename dlist<T>::iterator end)
{
	for (auto it = start; it != end;)
		it = erase(it);
	return end;
}

template<typename T>
void dlist<T>::clear()
{
	while (!empty())
		pop_back();
}

template<typename T>
void dlist<T>::reverse()
{
	if (!empty())
	{
		auto curr = head;

		while (curr != nullptr)
		{
			std::swap(curr->next, curr->prev);
			curr = curr->prev;
		}
	}

	std::swap(head, tail);
}

template<typename T>
bool operator== (const dlist<T>& lhs, const dlist<T>& rhs)
{
	bool flag = true;

	if (lhs.size() == rhs.size())
	{
		auto rhsIt = rhs.begin();

		for (auto lhsIt = lhs.begin(); lhsIt != lhs.end(); ++lhsIt)
		{
			if (*lhsIt != *rhsIt)
			{
				flag = false;
				break;
			}
			++rhsIt;
		}

		return flag;
	}
	
	return false;
}

template<typename T>
bool operator!= (const dlist<T>& lhs, const dlist<T>& rhs) { return !(lhs == rhs); }

template<typename T>
void dlist<T>::print(std::ostream& os, char ofc) const
{
	for (auto it = begin(); it != end(); ++it)
		os << *it << ofc;
}

template<typename T>
std::ostream& operator<< (std::ostream& os, const dlist<T>& list)
{
	list.print(os);
	return os;
}
#endif
