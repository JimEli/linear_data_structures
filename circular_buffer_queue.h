/*************************************************************************
* Title: FIFO circular buffer queue
* File: queue.h
* Author: James Eli
* Date: 12/13/2019
*
* Basic circular buffer queue using fixed array. Inputs wrap around to 
* buffer start. Output lags behind input by QUEUE_SIZE when input wraps.
* Define "INCLUDE_ITERATOR to include iterator support.
*
* Notes:
*  (1) Under C++17 front/back return std::optional<T>.
*  (2) Compiled with MS Visual Studio 2019 Community (v142).
*************************************************************************
* Change Log:
*   12/13/2019: Initial release. JME
*************************************************************************/
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <exception> // out of range.
#include <memory>    // smart pointer.
#include <optional>  // optional return values from front/back.
#include <algorithm> // copy.

#define INCLUDE_ITERATOR

// Default size of queue array if not specified during instantiation.
constexpr std::size_t DEFAULT_QUEUE_SIZE = 16;

template<class T, std::size_t QUEUE_SIZE = DEFAULT_QUEUE_SIZE>
class queue
{
private:
	std::unique_ptr<T[]> data; // Array of elements.
	size_t output;             // Elements are popped from this array index.
	size_t input;              // Elements are pushed to this array index.
	bool empty_;               // True if queue is empty.

	// Increment and wrap array index.
	static void increment(size_t& index) { index = (index + 1) % (QUEUE_SIZE + 1); }

public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;

	queue() : output(0), input(0), empty_(true) { data = std::make_unique<T[]>(QUEUE_SIZE); }
	// Copy ctor.
	queue(const queue<T, QUEUE_SIZE>& q) noexcept : input(q.input), output(q.output), empty_(q.empty_)
	{
		data = std::make_unique<T[]>(q.max_size());
		std::copy(q.begin(), q.end(), data.get());
	}
	// Move ctor.
	queue(queue<T, QUEUE_SIZE>&& q)	noexcept : input(q.input), output(q.output), empty_(q.empty_)
	{
		data = std::make_unique<T[]>(q.max_size());
		std::copy(q.begin(), q.end(), data.get());
		q.data = nullptr;
		q.empty_ = true;
	}
	// Copy assignment.
	queue<T, QUEUE_SIZE>& operator= (queue<T, QUEUE_SIZE> const& q) noexcept
	{
		output = q.output;
		input = q.input;
		empty_ = q.empty_;
		data = std::make_unique<T[]>(q.max_size());
		std::copy(q.begin(), q.end(), data.get());

		return *this;
	}
	// Move assignment.
	queue<T, QUEUE_SIZE>& operator= (queue<T, QUEUE_SIZE>&& q) noexcept
	{
		if (this != &q)
		{
			data = std::make_unique<T[]>(q.max_size());

			std::swap(data, q.data);
			std::swap(input, q.input);
			std::swap(output, q.output);
			std::swap(empty_, q.empty_);

			q.data = nullptr;
			q.empty_ = true;
		}
		return *this;
	}

	~queue() = default;

	void enqueue(T value)
	{
		data[input] = value;
		increment(input);
		if (!empty_ && output == input)
			increment(output);
		empty_ = false;
	}

	void dequeue()
	{
		if (empty_)
			return;
		increment(output);
		if (output == input)
			empty_ = true;
	}

	bool empty() const { return empty_; }
	size_t capacity() const { return max_size() - size(); }
	size_t max_size() const { return QUEUE_SIZE; }
	size_t size() const
	{
		if (input < output)
			return input + QUEUE_SIZE - output;
		else
			return input - output;
	}

// /Zc:__cplusplus
#if (__cplusplus >= 201703L) 
	std::optional<T> front() const
	{
		if (!empty_)
			return data[output];
		else
			return std::nullopt;
	}

	std::optional<T> back() const
	{
		if (!empty_)
			return data[input];
		else
			return std::nullopt;
	}

#else
	value_type front() const
	{
		if (empty_)
			throw std::out_of_range("empty queue");
		else
			return data[output];
	}

	value_type back() const
	{
		if (empty_)
			throw std::out_of_range("empty queue");
		else
			return data[input];
	}
#endif

#ifdef INCLUDE_ITERATOR
	class iterator : public std::iterator<std::forward_iterator_tag, queue<T, QUEUE_SIZE>>
	{
	public:
		iterator(T* queue, size_t index = 0, size_t max_size = 0) 
			: queue_(queue), current_(&queue[index]), index_(index), max_size_(max_size) { }

		iterator& operator++ ()
		{
			index_ = increment(index_, max_size_);
			current_ = &queue_[index_];
			return *this;
		}

		iterator operator++ (int)
		{
			auto pre = *this;

			index_ = increment(index_, max_size_);
			current_ = &queue_[index_];
			return pre;
		}

		iterator& operator= (iterator const& rhs)
		{
			queue_ = rhs.queue_;
			current_ = rhs.current_;
			index_ = rhs.index_;
			max_size_ = rhs.max_size_;
			return *this;
		}

		iterator& operator= (T const& rhs) 
		{ 
			*current_ = rhs; 
			return *this; 
		}

		reference operator* () const { return *current_; }
		pointer operator-> () const { return current_; }

		bool operator== (iterator const& rhs)const { return current_ == rhs.current_; }
		bool operator!= (iterator const& rhs) const { return current_ != rhs.current_; }

	private:
		pointer queue_;
		pointer current_;
		size_t index_;
		size_t max_size_;
	};
#endif

	pointer begin() const { return data.get(); }
	pointer end() const { return (data.get() + size()); }
};
#endif
