/*************************************************************************
* Title: Stack
* File: stack.h
* Author: James Eli
* Date: 10/26/2018
*
* Basic static array-based stack template class using a smart pointer.
*
* Notes:
*  (1) Alternative pop (which returns bool) commented in source.
*  (2) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0 (32 & 64-bit), and with Eclipse
*      Oxygen.3a Release (4.7.3a), using CDT 9.4.3/MinGw32 gcc-g++ (6.3.0-1).
*************************************************************************
* Change Log:
*  10/26/2018: Initial release. JME
*  10/29/2018: Added rezize stack to push. JME
*************************************************************************/
#ifndef _ARRAY_STACK_H_
#define _ARRAY_STACK_H_

#include <exception> // out of range
#include <memory>    // unique pointer

// Default size of stack array if not specified during instantiation.
constexpr std::size_t DEFAULT_STACK_SIZE = 16;

template<class T, std::size_t N = DEFAULT_STACK_SIZE>
class Stack
{
private:
	std::unique_ptr<T[]> data;
	std::size_t index;

public:
	Stack() : index(0) { data = std::make_unique<T[]>(N); }
	~Stack() = default;

	T top()
	{
		if (index)
			return data[index - 1];
		else
			throw std::out_of_range("stack empty");
	}

	T pop()
	{
		if (index)
		{
			T tmp = top();
			--index;
			return tmp;
		}
		else
			throw std::out_of_range("stack empty");
	}

	// Alternative pop returning bool.
	//bool pop() { if (index) { --index; return true; } else return false; }

	void push(T value)
	{
		if (index < N)
			data[index++] = value;
		else
		{
			//throw std::out_of_range("stack full");
			// Double stack capacity.
			std::size_t newSize = (index + 1) * 2;
			std::unique_ptr<T[]> temp(static_cast<T*>(data.release()));
			data.reset();
			data = std::make_unique<T[]>(newSize);
			for (std::size_t i = 0; i < index; i++)
				data[i] = temp[i];
		}
	}

	bool empty() { return (index == 0); }
};

#endif
