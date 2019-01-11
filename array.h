// Class array declared as an STL (wrapper) container for arrays of constant size.
// 7/16/2018 JME.
#pragma once
#include <algorithm>

//template<class T, std::size_t N>
template<class T, std::size_t N = 16>
struct array
{
	// Fixed size array of elements of type T.
	T elements[N];

	// Iterator support.
	T* begin() { return elements; }
	const T* begin() const { return elements; }
	T* end() { return elements + N; }
	const T* end() const { return elements + N; }

	// Reverse iterator support.
	std::reverse_iterator<T*> rbegin() { return std::reverse_iterator<T*>(end()); }
	std::reverse_iterator<const T*> rbegin() const { return std::reverse_iterator<const T*>(end()); }
	std::reverse_iterator<const T*> crbegin() const { return std::reverse_iterator<const T*>(end()); }
	std::reverse_iterator<T*> rend() { return std::reverse_iterator<T*>(begin()); }
	std::reverse_iterator<const T*> rend() const { return std::reverse_iterator<const T*>(begin()); }
	std::reverse_iterator<const T*> crend() const { return std::reverse_iterator<const T*>(begin()); }

	T& operator[] (std::size_t i) { return elements[i]; }
	const T& operator[] (std::size_t i) const { return elements[i]; }

	// At performs a range check.
	T& at(std::size_t i) 
	{ 
		if (i >= size())
			std::out_of_range("array<>: index out of range");
		return elements[i];
	}
	const T& at(std::size_t i) const 
	{ 
		if (i >= size())
			std::out_of_range("array<>: index out of range");
		return elements[i];
	}

	T& front() { return elements[0]; }
	const T& front() const { return elements[0]; }
	T& back() { return elements[N - 1]; }
	const T& back() const { return elements[N - 1]; }

	// Size is constant.
	static std::size_t size() { return N; }
	static std::size_t max_size() { return N; }
	static bool empty() { return false; }

	void swap(array<T, N>& a)
	{
		for (std::size_t i = 0; i < N; ++i)
			std::swap(elements[i], a.elements[i]);
	}

	// Assignment with type conversion.
	template <typename T2>
	array<T, N>& operator= (const array<T2, N>& rhs) 
	{
		std::copy(rhs.begin(), rhs.end(), begin());
		return *this;
	}

	// Assign one value to all elements.
	void fill(const T& value) { std::fill_n(begin(), size(), value); }
};

// Comparisons.
template<class T, std::size_t N>
bool operator== (const array<T, N>& x, const array<T, N>& y) 
{
	return std::equal(x.begin(), x.end(), y.begin());
}
template<class T, std::size_t N>
bool operator!= (const array<T, N>& x, const array<T, N>& y) { return !(x == y); }

template<class T, std::size_t N>
bool operator< (const array<T, N>& x, const array<T, N>& y) 
{
	return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}
template<class T, std::size_t N>
bool operator> (const array<T, N>& x, const array<T, N>& y) { return y<x; }
template<class T, std::size_t N>
bool operator<= (const array<T, N>& x, const array<T, N>& y) { return !(y<x); }
template<class T, std::size_t N>
bool operator>= (const array<T, N>& x, const array<T, N>& y) { return !(x<y); }

//
//
//
#if 0

#include<string>
#include<iostream>

using namespace std;

#ifndef ARRAY_H
#define ARRAY_H

template <class T>
class Array
{
public:
	Array(int);
	Array(const Array&);
	~Array();

	const Array& operator= (const Array&);

	T GetElement(int) const;
	void SetElement(T, int);

	void Resize(int);

	void Print() const;
	void Destroy();

private:
	int size;
	T* elements;
};

template <class T>
Array<T>::Array(int s)
{
	if (s < 0)
		return;
	size = s;
	elements = new T[size];
}

template <class T>
Array<T>::Array(const Array &obj)
{
	size = obj.size;
	elements = new T[size];

	for (int i = 0; i < size; ++i)
		elements[i] = obj.elements[i];
}

template <class T>
Array<T>::~Array()
{
	delete[] elements;
	elements = NULL;
	size = 0;
}

template <class T>
void Array<T>::Destroy()
{
	delete[] elements;
	elements = NULL;
	size = 0;
}

template <class T>
const Array<T> &Array<T>::operator= (const Array &obj)
{
	if (this != &obj)
	{
		size = obj.size;
		if (elements != NULL)
			delete[] elements;
	
		elements = new T[size];

		for (int i = 0; i < size; i++)
			elements[i] = obj.elements[i];
	}

	return *this;
}

template <class T>
Type Array<T>::GetElement(int index) const
{
	if (index < 0 || index >= size)
		cout << "Sorry, this operation can not be preformed\n";
	else
		return elements[index];
}

template <class T>
void Array<T>::SetElement(T e, int index)
{
	if (index < 0 || index >= size) 
	{
		cout << "Sorry, this operation can not be preformed\n";
		return;
	}
	else
		elements[index] = e;
}

template <class T>
void Array<T>::Print() const
{
	for (int i = 0; i<size; ++i)
		cout << elements[i] << endl;
}

template <class T>
void Array<T>::Resize(int newSize)
{
	if (newSize == size || newSize <= 0)
		return;

	T *temp = new T[newSize];
	int min = (newSize>size) ? size : newSize;

	for (int i = 0; i<min; i++)
		temp[i] = elements[i];
	
	delete[]elements;
	elements = temp;
	size = newSize;
	temp = nullptr;
}
#endif

#endif // #if 0

