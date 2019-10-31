#ifndef _SMART_LIST_H_
#define _SMART_LIST_H_

#include <iostream>  // cout
#include <memory>    // smart pointer
#include <exception> // out_of_range

template <typename T>
class list
{
private:
	struct node
	{
		template <typename T> friend class list;
		
		node(T e) { element = e; }

		T element;
		std::shared_ptr<node> next = nullptr;
	};

	std::shared_ptr<node> head = nullptr, tail = nullptr;

public:
	void clear()
	{
		while (!empty())
			pop_front();

		head.reset(), tail.reset();
	}

	std::size_t size() const
	{
		std::size_t size = 0;
		for (auto n = head; n; n = n->next, size++);

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
		auto newNode{ std::make_shared<node>(e) };

		if (!head)
			head = newNode;

		if (tail)
			tail->next = newNode;
		
		tail = newNode;
	}

	void push_front(const T& e)
	{
		auto newNode{ std::make_shared<node>(e) };

		if (!tail)
			tail = newNode;
		
		newNode->next = head;
		
		head = newNode;
	}

	void pop_front()
	{
		if (empty())
			return;
		
		if (tail == head)
			tail.reset();

		auto temp = head;
		head = std::move(head->next);
		temp.reset();
	}

	bool find(T d)
	{
		auto current = head;

		while (current != nullptr)
		{
			if (current->element == d)
				return true;
		
			current = current->next;
		}

		return false;
	}

	bool remove(T e) {
		if (empty())
			return false;

		auto current{ head }, prev{ head };

		while (current)
		{
			if (current->element == e)
				break;
			prev = current;
			current = current->next;
		}

		if (current == nullptr)
			return false;

		if (head == current)
			head = current->next;

		if (tail == current)
			tail = prev;

		prev->next = current->next;
		current.reset();

		return true;
	}

	void reverse()
	{
		if (empty() || !head->next)
			return;

		std::shared_ptr<node> prv = nullptr;
		std::shared_ptr<node> cur = head;
		std::shared_ptr<node> nxt = nullptr;

		while (cur) 
		{
			nxt = std::move(cur->next);
			cur->next = std::move(prv);
			prv = std::move(cur);
			cur = std::move(nxt);
		}

		std::swap(head, tail);
	}

	friend std::ostream& operator<< (std::ostream& os, const list<T>& list)
	{
		for (auto node = list.head; node; node = node->next)
			os << node->element;
		return os << std::endl;
	}
};

#endif
