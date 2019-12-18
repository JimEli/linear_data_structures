// Adapted from "C++ Concurrency in Action" listing 3.5
#ifndef _STACK_H_
#define _STACK_H_

#include <memory>    // smart pointer
#include <optional>  // optional
#include <mutex>     // mutex, lock_guard

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
	stack() { }
	stack(const stack&) = delete;
	stack& operator= (const stack&) = delete;

	void push(const T& e)
	{
		auto newNode{ std::make_shared<node<T>>(e) };
		newNode->element = e;
		const std::lock_guard<std::mutex> lock(stackMutex);
		newNode->next = head;
		head = newNode;
	}

	std::optional<T> pop()
	{
		const std::lock_guard<std::mutex> lock(stackMutex);
		if (empty())
			return std::nullopt;
		auto e = head->element;
		head = head->next;
		return e;
	}
/*
	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(stackMutex);
		if (empty())
			return nullptr;
		std::shared_ptr<T> const value(std::make_shared<T>(head->element));
		head = head->next;
		return value;
	}

	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m);
		if (empty())
			throw empty_stack();
		value = head->element;
		head = head->next;
	}
*/
	bool empty() const { return head == nullptr; }

	std::size_t size() const
	{
		std::size_t size = 0;
		const std::lock_guard<std::mutex> lock(stackMutex);
		for (auto node = head; node; node = node->next, size++);
		return size;
	}

	friend std::ostream& operator<< (std::ostream& os, const stack<T>& s)
	{
		for (auto n = s.head; n; n = n->next)
			os << n->element;
		return os << std::endl;
	}

private:
	std::shared_ptr<node<T>> head = nullptr;
	mutable std::mutex stackMutex;
};

#endif
