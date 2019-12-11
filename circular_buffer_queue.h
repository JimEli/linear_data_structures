#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <algorithm>
#include <cassert>
#include <stdexcept>

//#define INCLUDE_ITERATOR

template <typename T, typename A = std::allocator<T>>
class queue
{
public:
	explicit queue(size_t capacity, const A& allocator = A())
		: capacity_(capacity), allocator_(allocator), buffer_(allocator_.allocate(capacity)), head_(0), tail_(buffer_)
	{
		assert(capacity > 0);
	}

	~queue()
	{
		clear();
		allocator_.deallocate(buffer_, capacity_);
	}

	A getAllocator() const { return allocator_; }

	size_t capacity() const { return capacity_; }
	bool empty() const { return !head_; }
	size_t size() const { return !head_ ? 0 : (tail_ > head_ ? tail_ : tail_ + capacity_) - head_; }
	size_t max_size() const { return allocator_.max_size(); }

	bool push_back(const T& value)
	{
		if (head_ && head_ == tail_)
			allocator_.destroy(tail_);

		allocator_.construct(tail_, value);

		T* const next = wrap(tail_ + 1);
		if (!head_)
		{
			// First entry in the buffer.
			head_ = tail_;
			tail_ = next;
			return true;
		}
		else if (head_ == tail_)
		{
			// Buffer is full already, throw something away.
			head_ = tail_ = next;
			return false;
		}
		else
		{
			tail_ = next;
			return true;
		}
	}

	T& front()
	{
		assert(head_);
		return *head_;
	}
	const T& front() const
	{
		assert(head_);
		return *head_;
	}

	T& back()
	{
		assert(head_);
		return *wrap(tail_ - 1);
	}
	const T& back() const
	{
		assert(head_);
		return *wrap(tail_ - 1);
	}

	void pop_front()
	{
		assert(head_);

		allocator_.destroy(head_);
		T* const next = wrap(head_ + 1);
		if (next == tail_)
			head_ = 0;
		else
			head_ = next;
	}

	void clear()
	{
		if (head_)
		{
			do {
				allocator_.destroy(head_);
				head_ = wrap(head_ + 1);
			} while (head_ != tail_);
		}
		head_ = 0;
	}

#ifdef INCLUDE_ITERATOR
	T& operator[] (size_t n) { return *wrap(head_ + n); }
	const T& operator[] (size_t n) const { return *wrap(head_ + n); }

	T& at(size_t n)
	{
		if (n >= size())
			throw std::out_of_range("Parameter out of range");
		return (*this)[n];
	}

	const T& at(size_t n) const
	{
		if (n >= size())
			throw std::out_of_range("Parameter out of range");
		return (*this)[n];
	}

	class iterator : public std::iterator<std::random_access_iterator_tag, T, size_t, T*, T&>
	{
	public:
		iterator(queue<T>& p, size_t i) : parent(p), index(i) { }

		iterator& operator++ ()
		{
			++index;
			return *this;
		}
		iterator operator++ (int) // postincrement
		{
			iterator old(*this);
			operator++();
			return old;
		}
		iterator& operator-- ()
		{
			--index;
			return *this;
		}
		iterator operator-- (int) // postdecrement
		{
			iterator old(*this);
			operator--();
			return old;
		}

		T& operator* () { return parent[index]; }
		T* operator-> () { return &(parent[index]); }

		bool operator== (const iterator& other) const { return &parent == &other.parent && index == other.index; }
		bool operator!= (const iterator& other) const { return !(other == *this); }

	private:
		queue<T>& parent;
		size_t index;
	};

	iterator begin() { return iterator(*this, 0); }
	iterator end() { return iterator(*this, size()); }
#endif

private:
	size_t capacity_;
	A allocator_;
	T* buffer_;
	T* head_;
	T* tail_;

	queue(const queue<T>&);
	//queue<T>& operator= (const queue<T>&);

	T* wrap(T* ptr) const
	{
		assert(ptr < buffer_ + capacity_ * 2);
		assert(ptr > buffer_ - capacity_);
		
		if (ptr >= buffer_ + capacity_)
			return ptr - capacity_;
		else if (ptr < buffer_)
			return ptr + capacity_;
		else
			return ptr;
	}
};

#endif
