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
	typedef T value_type;
	typedef A allocator_type;
	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type difference_type;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::const_pointer const_pointer;
	class iterator;

	explicit inline queue(size_type capacity, const allocator_type& allocator = allocator_type())
		: capacity_(capacity), allocator_(allocator), buffer_(allocator_.allocate(capacity)), head_(0), tail_(buffer_)
	{
		assert(capacity > 0);
	}


	inline ~queue()
	{
		clear(); // deallocates all objects
		allocator_.deallocate(buffer_, capacity_);
	}

	inline typename allocator_type getAllocator() const { return allocator_; }

	inline typename size_type capacity() const { return capacity_; }
	inline bool empty() const { return !head_; }
	inline typename size_type size() const { return !head_ ? 0 : (tail_ > head_ ? tail_ : tail_ + capacity_) - head_; }
	inline typename size_type max_size() const { return allocator_.max_size(); }

	inline bool push_back(const value_type& value)
	{
		if (head_ && head_ == tail_)
			allocator_.destroy(tail_);

		allocator_.construct(tail_, value);

		value_type* const next = wrap(tail_ + 1);
		if (!head_)
		{
			// first entry in the buffer
			head_ = tail_;
			tail_ = next;
			return true;
		}
		else if (head_ == tail_)
		{
			// buffer is full already, throw something away
			head_ = tail_ = next;
			return false;
		}
		else
		{
			tail_ = next;
			return true;
		}
	}

	inline typename reference front()
	{
		assert(head_);
		return *head_;
	}
	inline typename const_reference front() const
	{
		assert(head_);
		return *head_;
	}

	inline typename reference back()
	{
		assert(head_);
		return *wrap(tail_ - 1);
	}
	inline typename const_reference back() const
	{
		assert(head_);
		return *wrap(tail_ - 1);
	}

	inline void pop_front()
	{
		assert(head_);

		allocator_.destroy(head_);
		value_type* const next = wrap(head_ + 1);
		if (next == tail_)
			head_ = 0;
		else
			head_ = next;
	}

	inline void clear()
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
	inline typename reference operator[] (size_type n) { return *wrap(head_ + n); }
	inline typename const_reference operator[] (size_type n) const { return *wrap(head_ + n); }

	inline typename reference at(size_type n)
	{
		if (n >= size())
			throw std::out_of_range("Parameter out of range");
		return (*this)[n];
	}

	inline typename const_reference at(size_type n) const
	{
		if (n >= size())
			throw std::out_of_range("Parameter out of range");
		return (*this)[n];
	}

	class iterator : public std::iterator<std::random_access_iterator_tag, value_type, size_type, pointer, reference>
	{
	public:
		typedef queue<T> parent_type;
		typedef typename parent_type::iterator self_type;

		iterator(parent_type& parent, size_type index) : parent(parent), index(index) {}

		self_type& operator++()
		{
			++index;
			return *this;
		}
		self_type operator++(int) // postincrement
		{
			self_type old(*this);
			operator++();
			return old;
		}
		self_type& operator--()
		{
			--index;
			return *this;
		}
		self_type operator--(int) // postdecrement
		{
			self_type old(*this);
			operator--();
			return old;
		}

		reference operator*() { return parent[index]; }
		pointer operator->() { return &(parent[index]); }

		bool operator==(const self_type& other) const { return &parent == &other.parent && index == other.index; }
		bool operator!=(const self_type& other) const { return !(other == *this); }

	private:
		parent_type& parent;
		size_type index;
	};

	typename iterator begin() { return iterator(*this, 0); }
	typename iterator end() { return iterator(*this, size()); }
#endif

private:
	size_type capacity_;
	allocator_type allocator_;
	pointer buffer_;
	pointer head_;
	pointer tail_;

	typedef queue<T> class_type;

	queue(const class_type&);
	//class_type& operator= (const class_type&);

	value_type* wrap(value_type* ptr) const
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
