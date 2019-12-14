#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename T>
struct MaxHeap 
{
private:
	std::vector<int> array;

	std::size_t parent(std::size_t i) { return (i - 1) / 2; }
	std::size_t leftChild(std::size_t i) { return (2 * i + 1); }
	std::size_t rightChild(std::size_t i) { return (2 * i + 2); }

	void heapifyDown(std::size_t i) 
	{
		std::size_t left = leftChild(i);
		std::size_t right = rightChild(i);
		std::size_t largest = i;

		// compare array[i] with its left and right child & find largest value
		if (left < size() && array[left] > array[i])
			largest = left;
		if (right < size() && array[right] > array[largest])
			largest = right;
		// swap with child having greater value & call heapify-down on the child
		if (largest != i) {
			std::swap(array[i], array[largest]);
			heapifyDown(largest);
		}
	}

	void heapifyUp(std::size_t i) 
	{
		// check if node at index i and its parent violates the heap property
		if (i && array[parent(i)] < array[i]) 
		{
			// swap the two if heap property is violated
			std::swap(array[i], array[parent(i)]);
			// call Heapify-up on the parent
			heapifyUp(parent(i));
		}
	}

public:
	std::size_t size() { return array.size(); }

	bool empty() { return size() == 0; }

	void push(const T& key) 
	{
		array.push_back(key);
		std::size_t index = size() - 1;
		heapifyUp(index);
	}

	void pop() 
	{
		try {
			// If heap has no elements, throw an exception.
			if (size() == 0)
				throw std::out_of_range("index out of range (Heap underflow)");
		}
		catch (const std::out_of_range& oor) 
		{
			std::cout << "\n" << oor.what();
		}
		array[0] = array.back();
		array.pop_back();
		heapifyDown(0);
	}

	T peek() 
	{
		try {
			// if heap has no elements, throw an exception
			if (size() == 0)
				throw std::out_of_range("index out of range (Heap underflow)");
		}
		catch (const std::out_of_range& oor) 
		{
			std::cout << "\n" << oor.what();
		}
		return array.at(0);	// or return array[0];
	}
};
