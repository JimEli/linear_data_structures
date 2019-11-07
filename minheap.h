// Min heap. Requires that type T has a function operator< defined.
#ifndef _MIN_HEAP_H_
#define _MIN_HEAP_H_

template <typename T>
class MinHeap 
{
  T* data;
  std::size_t length;
  std::size_t arraySize;
  static constexpr std::size_t INITIAL_SIZE = 1;

  void resize(std::size_t newSize) {
    T* newData = new T[newSize];

    for (std::size_t i = 0; i < length_; ++i)
      newData[i] = data_[i];

    delete[] data;

    data = newData;
    arraySize = newSize;
  }

public:
  MinHeap() : data(nullptr), length(0), arraySize(INITIAL_SIZE) { data_ = new T[INITIAL_SIZE]; }
  
  ~MinHeap() {
    delete[] data;
    data = nullptr;
  }
  
  void push(const T& element) {
    std::size_t i;

    if (length >= arraySize) 
      resize(arraySize * 2);

    for (i = length; i > 0;) 
    {
      std::size_t parent = (i - 1) / 2;
      if (element < data[parent]) 
      {
        data[i] = data[parent];
        i = parent;
      }
      else 
        break;
    }

    data_[i] = element;
    ++length;
  }

  void pop() {
    T element = data[length - 1];
    std::size_t i;

    if (length < arraySize / 2 && arraySize > INITIAL_SIZE) 
      resize(arraySize / 2);

    for (i = 0; (i * 2 + 1) < length;) 
    {
      std::size_t left = i * 2 + 1, right = i * 2 + 2;

      if (right >= length) 
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

    if (length > 0) 
      --length;
  }

  T& top() { return data[0]; }

  bool empty() { return length == 0; }
  std::size_t size() { return length; }
};

#endif
