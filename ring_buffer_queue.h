#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <algorithm>
#include <memory>

constexpr std::size_t DEFAULT_MAX_SIZE{ 2 };
constexpr std::size_t DEFAULT_INCREASE_SIZE{ 2 };

static std::size_t increment(std::size_t index, std::size_t max)
{
	if (index + 1 >= max)
		return (index + 1) % max;
	else
		return ++index;
}

template <typename T, typename Alloc = std::allocator<T>>
class queue
{
public:
	// Ctor.
	queue() = default;
	explicit queue(size_t size) : queue_(allocator_.allocate(size)), maxSize_(size) { }
		
	// Copy ctor.
	queue(queue const& q) : 
		allocator_(q.allocator_), queue_(allocator_.allocate(q.maxSize_)), front_(q.front_), back_(q.back_), maxSize_(q.maxSize_)
	{
		std::uninitialized_copy(q.queue_, q.queue_ + q.maxSize_, queue_);
	}

	// init_list ctor.
	queue(std::initializer_list<T> const& s) : 
		queue_(allocator_.allocate(s.size() + 1)), back_(s.size()), maxSize_(s.size() + 1)
	{
		std::uninitialized_copy(s.begin(), s.end(), queue_);
	}

	// Move ctor.
	queue(queue&& q) : 
		queue_(q.queue_), front_(q.front_), back_(q.back_), maxSize_(q.maxSize_)
	{
		q.queue_ = nullptr;
	}

	// Copy assignment.
	queue& operator=(queue const& q)
	{
		queue temp{ q };
		std::swap(temp, *this);
		return *this;
	}

	// Move assignment.
	queue& operator=(queue&& q)
	{
		std::swap(queue_, q.queue_);
		std::swap(front_, q.front_);
		std::swap(back_, q.back_);
		std::swap(maxSize_, q.maxSize_);
		return *this;
	}

	~queue() { allocator_.deallocate(queue_, maxSize_); }

	// Iterator.
	class iterator : public std::iterator<std::forward_iterator_tag, queue<T>>
	{
	public:
		iterator(T* queue, size_t index = 0, size_t maxSize = 0) : 
			queue_(queue), current_(&queue[index]), index_(index), maxSize_(maxSize) { }

		iterator& operator++ ()
		{
			index_ = increment(index_, maxSize_);
			current_ = &queue_[index_];
			return *this;
		}

		iterator operator++ (int)
		{
			auto pre = *this;
			index_ = increment(index_, maxSize_);
			current_ = &queue_[index_];
			return pre;
		}

		iterator& operator= (iterator const& rhs)
		{
			queue_ = rhs.queue_;
			current_ = rhs.current_;
			index_ = rhs.index_;
			maxSize_ = rhs.maxSize_;
			return *this;
		}
		iterator& operator= (T const& rhs) { *current_ = rhs; return *this; }

		T& operator* () const { return *current_; }
		T* operator-> () const { return current_; }
			
		bool operator== (iterator const& rhs)const { return current_ == rhs.current_; }
		bool operator!= (iterator const& rhs) const { return current_ != rhs.current_; }

	private:
		T* queue_;
		T* current_;
		size_t index_;
		size_t maxSize_;
	};

	iterator begin() const { return iterator(queue_, front_, maxSize_); }
	iterator end() const { return iterator(queue_ + back_); }

	iterator find(T const& t) const
	{
		for (auto it = begin(); it != end(); ++it)
			if (*it == t)
				return it;
		return end();
	}

	void resize(size_t newSize)
	{
		auto newQueue = allocator_.allocate(newSize + 1);
		auto elemSize = std::min(newSize, size());

		if (elemSize > 0)
		{
			// Linear copy front --> elem_size.
			if (front_ < back_)
				std::uninitialized_copy(queue_ + front_, queue_ + elemSize, newQueue);
			// Split up, need to copy front --> end_of_elems, then 0 --> back_.
			else
			{
				size_t frontHalf = maxSize_ - front_;
				size_t backHalf = back_ - 1;

				// Shinking, only need to copy up to elem_size (not past).
				if (elemSize < backHalf)
					backHalf = elemSize;

				std::uninitialized_copy_n(queue_ + front_, frontHalf, newQueue);
				std::uninitialized_copy_n(queue_, backHalf, newQueue + frontHalf);
			}
		}

		allocator_.deallocate(queue_, maxSize_);

		back_ = elemSize;
		front_ = 0;
		maxSize_ = newSize + 1;
		queue_ = newQueue;
	}

	void enqueue(T const& value)
	{
		if (size() + 1 >= maxSize_)
			resize(maxSize_ * DEFAULT_INCREASE_SIZE);
		allocator_.construct(&queue_[back_], value);
		back_ = increment(back_, maxSize_);
	}

	void enqueue(T const&& value)
	{
		if (size() + 1 >= maxSize_)
			resize(maxSize_ * DEFAULT_INCREASE_SIZE);
		allocator_.construct(&queue_[back_], std::move(value));
		back_ = increment(back_, maxSize_);
	}

	T dequeue()
	{
		if (size() <= 0)
			throw std::out_of_range("Queue is empty, no elements left.");

		auto elem = std::move(queue_[front_]);
		front_ = increment(front_, maxSize_);
		return elem;
	}

	bool empty() const { return size() == 0; }

	size_t size() const
	{
		if (front_ > back_)
		{
			if (maxSize_ > 0)
				return (maxSize_ - (front_ - back_)) % maxSize_;
		}
		else
			return back_ - front_;
		return 0;
	}

private:
	std::allocator<T> allocator_;
	T* queue_{ allocator_.allocate(DEFAULT_MAX_SIZE) };
	size_t front_{ 0 };
	size_t back_{ 0 };
	size_t maxSize_{ DEFAULT_MAX_SIZE };
};

#endif 
