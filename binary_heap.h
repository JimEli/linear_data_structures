// Binary Heap (priority queue) of std::pair where,
// first==(std::size_t)key, and second==(T)item.
#ifndef MH_H
#define MH_H

#include <vector>
#include <utility>
#include <exception>

template <typename T>
struct maxheap
{
private:
	std::vector<std::pair<std::size_t, T>> heap;

	std::size_t parent(std::size_t i) { return (i - 1) / 2; }
	std::size_t left(std::size_t i) { return (2 * i + 1); }
	std::size_t right(std::size_t i) { return (2 * i + 2); }

	void heapifyDown(std::size_t i)
	{
		std::size_t l = left(i);
		std::size_t r = right(i);
		std::size_t largest = i;

		if (l < size() && heap[l].first > heap[i].first)
			largest = l;

		if (r < size() && heap[r].first > heap[largest].first)
			largest = r;

		if (largest != i)
		{
			std::swap(heap[i], heap[largest]);
			heapifyDown(largest);
		}
	}

	void heapifyUp(std::size_t i)
	{
		if (i && heap[parent(i)].first < heap[i].first)
		{
			std::swap(heap[i], heap[parent(i)]);
			heapifyUp(parent(i));
		}
	}

	void checkEmpty()
	{
		if (size() == 0)
			throw std::out_of_range("index out of range (Heap underflow)");
	}

public:
	std::size_t size() { return heap.size(); }
	
	bool empty() { return heap.size() == 0; }

	void push(std::pair<std::size_t, T> p)
	{
		heap.push_back(p);
		std::size_t index = size() - 1;
		heapifyUp(index);
	}
	
	void push(std::size_t key, T data) { push(std::make_pair<std::size_t, T>((std::size_t)key, (T)data)); }

	void pop()
	{
		checkEmpty();
		heap[0] = heap.back();
		heap.pop_back();
		heapifyDown(0);
	}

	
	std::pair<std::size_t, T>& top()
	{
		checkEmpty();
		return heap.at(0);
	}
	
	std::size_t& topKey()
	{
		checkEmpty();
		return heap.at(0).first;
	}
	
	T& topItem()
	{
		checkEmpty();
		return heap.at(0).second;
	}

	friend std::ostream& operator<< (std::ostream& os, const maxheap& mh)
	{
		for (auto i = mh.heap.begin(); i != mh.heap.end(); ++i)
			os << (*i).second << " ";
		return os;
	}
};

#endif
