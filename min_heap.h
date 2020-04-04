// Min heap. Requires that type T has a function operator< defined.
#include <memory>
#include <algorithm>

template <typename T>
class min_heap 
{
	std::unique_ptr<T[]> data;
	std::size_t arrayUsed, arraySize;
	static constexpr std::size_t INITIAL_SIZE = 2;

	void resize(std::size_t newSize)
	{
		std::unique_ptr<T[]> temp = std::make_unique<T[]>(sizeof(T)*arraySize);
		std::copy(data.get(), data.get() + arraySize, temp.get());

		data.reset(new T[newSize]);

		std::copy(temp.get(), temp.get() + std::min(arraySize, newSize), data.get());

		temp.reset();
		arraySize = newSize;
	}

public:
	min_heap() : arrayUsed(0), arraySize(INITIAL_SIZE) { data.reset(new T[INITIAL_SIZE]); }
	~min_heap() { data.reset(); }
	
	void push(const T& element) 
	{
		std::size_t i;

		if (arrayUsed >= arraySize) 
			resize(arraySize * 2);

		for (i = arrayUsed; i > 0;) 
		{
			int parent = (i - 1) / 2;
			if (element < data[parent]) 
			{
				data[i] = data[parent];
				i = parent;
			}
			else 
				break;
		}

		data[i] = element;
		arrayUsed++;
	}

	void pop() 
	{
		T element = data[arrayUsed - 1];
		std::size_t i;

		if (arrayUsed < arraySize / 2 && arraySize > INITIAL_SIZE) 
			resize(arraySize / 2);

		for (i = 0; (i * 2 + 1) < arrayUsed;) 
		{
			std::size_t left = i * 2 + 1, right = i * 2 + 2;

			if (right >= arrayUsed) 
			{
				if (data[left] < element) 
				{
					data[i] = data[left];
					i = left;
				}
				break;
			}
			else 
			{
				std::size_t child = left;
				
				if (data[2 * i + 2] < data[2 * i + 1]) 
					child = right;

				if (data[child] < element) 
				{
					data[i] = data[child];
					i = child;
				}
				else 
					break;
			}
		}
		data[i] = element;

		if (arrayUsed > 0) 
			arrayUsed--;
	}

	T top() { return data[0]; }

	bool empty() { return arrayUsed == 0; }
};

