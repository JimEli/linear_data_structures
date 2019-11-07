// Priority Queue Implementation.
#ifndef _PQ_H_
#define _PQ_H_

#include <algorithm>
#include <type_traits>

// Compare = std::less gives a max-oriented heap, std::greater gives min-oriented one.
template <class T, class Container = std::vector<T>, class Compare = std::less<T>>
class PriorityQueue 
{
protected:
	Container c;
	Compare comp;

public:
	explicit PriorityQueue(const Container& c_ = Container(), const Compare& comp_ = Compare()) : c(c_), comp(comp_)
	{
		std::make_heap(c.begin(), c.end(), comp);
	}

	explicit PriorityQueue(Container&& c_) : c(std::move(c_)) 
	{
		std::make_heap(c.begin(), c.end(), comp);
	}

	explicit PriorityQueue(const Compare& comp_, Container&& c_ = Container()) : c(std::move(c_)), comp(comp_)
	{
		std::make_heap(c.begin(), c.end(), comp);
	}

	bool empty() const { return c.empty(); }
	
	std::size_t size() const { return c.size(); }

	const T& top() const { return c.front(); }

	void push(const T& x)
	{
		c.push_back(x);
		std::push_heap(c.begin(), c.end(), comp);
	}

	void pop()
	{
		std::pop_heap(c.begin(), c.end(), comp);
		c.pop_back();
	}

#if __cplusplus >= 201703L // >201402L = C++17 feature
		void swap(PriorityQueue& pq) noexcept(__is_nothrow_swappable<T>::value)
#else
	void swap(PriorityQueue& pq)
#endif
	{
		using std::swap;
		swap(c, pq.c);
	}
};
