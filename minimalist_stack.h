// Simplistic stack derived from vector.
#include <iostream>
#include <vector>

template <class T>
class stack : public std::vector<T>
{
private:
	std::vector<T> data;
public:
	std::size_t size() { return data.size(); }
	std::size_t empty() { return data.empty(); }
	void push(const T& item) { data.push_back(item); }
	T& pop() { T& d = data.back(); data.pop_back(); return d; }
	friend std::ostream& operator<< (std::ostream& os, stack<T>& s) {
		std::size_t n = s.data.size();
		while (n) os << s.data.at(--n) << " ";
		return os;
	}
};
