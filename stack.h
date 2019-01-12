#ifndef _STACK_H_
#define _STACK_H_

#include <iostream>  // cout
#include <memory>    // smart pointer
#include <exception> // exceptions

template <typename T>
class stack 
{
protected:
	template <typename T>
	struct node
	{
		node(T e) { element = e; }

		T element;
		std::shared_ptr<node<T>> next = nullptr;
	};

public:
	void push(const T& e)
	{
		auto newNode{ std::make_shared<node<T>>(e) };
		newNode->element = e;
		newNode->next = head;
		head = newNode;
	}

	const T top() const
	{
		if (empty())
			throw std::out_of_range("stack empty");
		return head->element;
	}

	const T pop()
	{
		if (empty())
			throw std::out_of_range("stack empty");
		T e = top();
		auto temp{ head };
		head = head->next;
		return e;
	}

	bool empty() const { return head == nullptr; }

	std::size_t size() const
	{
		std::size_t size = 0;
		for (auto node = head; node; node = node->next, size++);
		return size;
	}

private:
	std::shared_ptr<node<T>> head = nullptr;
};

#endif
