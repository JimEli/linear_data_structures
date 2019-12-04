#ifndef _dataH_
#define _dataH_

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
	explicit queue(size_t size) : data(alloc.allocate(size)), maxSize(size) { }
		
	// Copy ctor.
	queue(queue const& q) : 
		alloc(q.alloc), data(alloc.allocate(q.maxSize)), head(q.head), tail(q.tail), maxSize(q.maxSize)
	{
		std::uninitialized_copy(q.data, q.data + q.maxSize, data);
	}

	// init_list ctor.
	queue(std::initializer_list<T> const& s) : 
		data(alloc.allocate(s.size() + 1)), tail(s.size()), maxSize(s.size() + 1)
	{
		std::uninitialized_copy(s.begin(), s.end(), data);
	}

	// Move ctor.
	queue(queue&& q) : data(q.data), head(q.head), tail(q.tail), maxSize(q.maxSize)	{ q.data = nullptr;	}

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
		std::swap(data, q.data);
		std::swap(head, q.head);
		std::swap(tail, q.tail);
		std::swap(maxSize, q.maxSize);
		return *this;
	}

	~queue() { alloc.deallocate(data, maxSize); }

	// Iterator.
	class iterator : public std::iterator<std::forward_iterator_tag, queue<T>>
	{
	public:
		iterator(T* queue, size_t index = 0, size_t maxSize = 0) : 
			data(queue), current_(&queue[index]), index_(index), maxSize(maxSize) { }

		iterator& operator++ ()
		{
			index_ = increment(index_, maxSize);
			current_ = &data[index_];
			return *this;
		}

		iterator operator++ (int)
		{
			auto pre = *this;
			index_ = increment(index_, maxSize);
			current_ = &data[index_];
			return pre;
		}

		iterator& operator= (iterator const& rhs)
		{
			data = rhs.data;
			current_ = rhs.current_;
			index_ = rhs.index_;
			maxSize = rhs.maxSize;
			return *this;
		}
		iterator& operator= (T const& rhs) { *current_ = rhs; return *this; }

		T& operator* () const { return *current_; }
		T* operator-> () const { return current_; }
			
		bool operator== (iterator const& rhs)const { return current_ == rhs.current_; }
		bool operator!= (iterator const& rhs) const { return current_ != rhs.current_; }

	private:
		T* data;
		T* current_;
		size_t index_;
		size_t maxSize;
	};

	iterator begin() const { return iterator(data, head, maxSize); }
	iterator end() const { return iterator(data + tail); }

	iterator find(T const& t) const
	{
		for (auto it = begin(); it != end(); ++it)
			if (*it == t)
				return it;
		return end();
	}

	void resize(size_t newSize)
	{
		auto newQueue = alloc.allocate(newSize + 1);
		auto elemSize = std::min(newSize, size());

		if (elemSize > 0)
		{
			// Linear copy front --> elem_size.
			if (head < tail)
				std::uninitialized_copy(data + head, data + elemSize, newQueue);
			// Split up, need to copy front --> end_of_elems, then 0 --> tail.
			else
			{
				size_t frontHalf = maxSize - head;
				size_t backHalf = tail - 1;

				// Shinking, only need to copy up to elem_size (not past).
				if (elemSize < backHalf)
					backHalf = elemSize;

				std::uninitialized_copy_n(data + head, frontHalf, newQueue);
				std::uninitialized_copy_n(data, backHalf, newQueue + frontHalf);
			}
		}

		alloc.deallocate(data, maxSize);

		tail = elemSize;
		head = 0;
		maxSize = newSize + 1;
		data = newQueue;
	}

	void enqueue(T const& value)
	{
		if (size() + 1 >= maxSize)
			resize(maxSize * DEFAULT_INCREASE_SIZE);
		alloc.construct(&data[tail], value);
		tail = increment(tail, maxSize);
	}

	void enqueue(T const&& value)
	{
		if (size() + 1 >= maxSize)
			resize(maxSize * DEFAULT_INCREASE_SIZE);
		alloc.construct(&data[tail], std::move(value));
		tail = increment(tail, maxSize);
	}

	T dequeue()
	{
		if (size() <= 0)
			throw std::out_of_range("Queue is empty, no elements left.");

		auto elem = std::move(data[head]);
		head = increment(head, maxSize);
		return elem;
	}

	bool empty() const { return size() == 0; }

	size_t size() const
	{
		if (head > tail)
		{
			if (maxSize > 0)
				return (maxSize - (head - tail)) % maxSize;
		}
		else
			return tail - head;
		return 0;
	}

private:
	std::allocator<T> alloc;
	T* data{ alloc.allocate(DEFAULT_MAX_SIZE) };
	size_t head{ 0 };
	size_t tail{ 0 };
	size_t maxSize{ DEFAULT_MAX_SIZE };
};

#endif 
