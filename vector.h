#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <memory>    // unique pointer
#include <algorithm> // min
#include <limits>    // numeric_limits

template <typename T>
class vector
{
	std::size_t count;             // Number of actually stored objects.
	std::size_t reservedSize;      // Allocated reservedSize.
	std::unique_ptr<T[]> elements; // Data elements.

public:
	using iterator = T * ;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	typename vector<T>::iterator begin() noexcept { return elements.get(); }
	typename vector<T>::iterator end() noexcept { return iterator(elements.get() + count); }
	typename vector<T>::const_iterator cbegin() const noexcept { return begin(); }
	typename vector<T>::const_iterator cend() const noexcept { return end(); }
	typename vector<T>::reverse_iterator rbegin() noexcept { return reverse_iterator(elements.get() + count); }
	typename vector<T>::reverse_iterator rend() noexcept { return reverse_iterator(elements.get()); }
	typename vector<T>::const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(elements.get() + count); }
	typename vector<T>::const_reverse_iterator crend() const noexcept { return const_reverse_iterator(elements.get()); }

	template<typename T> friend bool operator== (const vector<T>&, const vector<T>&);
	template<typename T> friend bool operator!= (const vector<T>&, const vector<T>&);
	template<typename T> friend bool operator< (const vector<T>&, const vector<T>&);
	template<typename T> friend bool operator<= (const vector<T>&, const vector<T>&);
	template<typename T> friend bool operator> (const vector<T>&, const vector<T>&);
	template<typename T> friend bool operator>= (const vector<T>&, const vector<T>&);
	template<typename T> friend void swap(vector<T>&, vector<T>&);

	vector() : count(0), reservedSize(0), elements(nullptr) { }

	// Copy ctor.
	vector(vector const &rhs) : count(rhs.count), reservedSize(rhs.reservedSize)
	{
		std::unique_ptr<T[]> elements = std::make_unique<T[]>(reservedSize);
		std::copy(rhs.elements.get(), (rhs.elements.get() + count), elements.get());
	}

	// Move ctor.
	vector(vector&& rhs) noexcept { swap(rhs); }

	// Assignment ctor.
	vector<T>& operator= (const vector& rhs)
	{
		swap(vector(rhs));
		return *this;
	}

	// Move assignment ctor.
	vector<T>& operator= (vector<T>&& rhs)
	{
		swap(rhs);
		return *this;
	}

	~vector() { clear(); }

	void clear() 
	{ 
		while (0 < count)
			elements[--count].~T();
		reservedSize = 0;
	}

	void push_back(T const &d)
	{
		if (reservedSize == count)
			resize();
		elements[count++] = d;
	}

	void pop_back()
	{
		if (count == 0)
			return;
		count--;
	}

	T const &operator[] (size_t i) const { return elements[i]; }
	T& operator[] (size_t i) { return elements[i]; }

	T const & at(size_t i) const { return elements[i]; }
	T& at(size_t i) { return elements[i]; }

	T* data() noexcept { return elements.get(); }
	const T* data() const noexcept { return elements; }

	T& front() const { return elements[0]; }
	T& back() const { return elements[count - 1]; }

	void insert(size_t i, T& d)
	{
		if (i < 0 || i > count - 1)
			return;

		if (reservedSize == count)
			resize();

		std::copy((elements.get() + i), (elements.get() + count), (elements.get() + i + 1));
		elements[i] = d;
		count++;
	}

	void insert(iterator it, const T& d)
	{
		size_t i = it - begin();

		if (i < 0 || i > count - 1)
			return;

		if (reservedSize == count)
			resize();

		std::copy((elements.get() + i), (elements.get() + count), (elements.get() + i + 1));
		elements[i] = d;
		count++;
	}

	void erase(size_t pos)
	{
		if (pos < 0 || pos >= count)
			return;

		--count;
		std::copy((elements.get() + pos + 1), (elements.get() + count), (elements.get() + pos));
	}

	void erase(iterator it)
	{
		size_t pos = it - begin();

		if (pos < 0 || pos >= count)
			return;

		--count;
		std::copy((elements.get() + pos + 1), (elements.get() + count), (elements.get() + pos));
	}

	void assign(size_t n, const T& d)
	{
		for (size_t i = 0; i < n; i++)
			push_back(d);
	}

	void swap(vector<T>& rhs)
	{
		std::swap(count, rhs.count);
		std::swap(reservedSize, rhs.reservedSize);
		std::swap(elements, rhs.elements);
	}

	size_t size() const noexcept { return count; }
	bool empty() const noexcept { return count == 0; }
	size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }
	size_t capacity() const noexcept { return reservedSize; }

	void reserve(size_t n) 
	{
		std::unique_ptr<T[]> newElements(static_cast<T*>(elements.release()));
		elements.reset(new T[reservedSize]);

		count = std::min(n, count);
		std::copy(elements.get(), (elements.get() + count), newElements.get());
		reservedSize = n;
		elements = std::move(newElements);

	}
	void resize(size_t n) { reserve(n); }
	void shrink_to_fit() { reserve(count); }

private:
	// Allocates double the old space.
	void resize()
	{
		reservedSize = reservedSize ? reservedSize * 2 : 1;

		std::unique_ptr<T[]> temp(static_cast<T*>(elements.release()));
		elements.reset(new T[reservedSize]);
		std::copy(temp.get(), (temp.get() + count), elements.get());
	}
};

template <typename T>
bool operator== (const vector<T>& lhs, const vector<T>& rhs)
{
	if (lhs.count != rhs.count)
		return false;

	for (size_t i = 0; i < lhs.count; i++)
		if (lhs[i] != rhs[i])
			return false;

	return true;
}

template<typename T>
bool operator!= (const vector<T>& lhs, const vector<T>& rhs)
{
	return !(lhs == rhs);
}

template<typename T>
bool operator< (const vector<T>& lhs, const vector<T>& rhs)
{
	typename vector<T>::size_type n = (lhs.count < rhs.count) ? lhs.count : rhs.count;
	
	for (size_t i = 0; i < n; i++)
		if (lhs[i] != rhs[i])
			return lhs[i] < rhs[i];
	
	return lhs.count < rhs.count;
}

template<typename T>
bool operator> (const vector<T>& lhs, const vector<T>& rhs)
{
	typename vector<T>::size_type n = lhs.count < rhs.count ? lhs.count : rhs.count;

	for (size_t i = 0; i < n; i++)
		if (lhs[i] != rhs[i])
			return lhs[i] > rhs[i];

	return lhs.count > rhs.count;
}

template<typename T>
bool operator<= (const vector<T>& lhs, const vector<T>& rhs) { return !(lhs > rhs); }

template<typename T>
bool operator>= (const vector<T>& lhs, const vector<T>& rhs) { return !(lhs < rhs); }

template<typename T>
void swap(vector<T>& lhs, vector<T>& rhs) { lhs.swap(rhs); }

#endif
