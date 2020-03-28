#ifndef _SMART_LIST_H_
#define _SMART_LIST_H_

#include <iostream>  // cout
#include <memory>    // smart pointer
#include <exception> // out_of_range

template <typename T>
class list
{
private:
	struct node
	{
		template <typename T> friend class list;
		explicit node(T e) : element(e) { }
		explicit node(std::shared_ptr<node> n) : next(n) { }
		node(T e, std::shared_ptr<node> n) : element(e), next(n) { }
		~node() { ~T(); next = nullptr; }

	protected:
		T element;
		std::shared_ptr<node> next = nullptr;
	};

	std::shared_ptr<node> head = nullptr, tail = nullptr;

public:
	// Inner iterator class. Member typedefs provided through inherit from std::iterator.
	class iterator : public std::iterator<std::forward_iterator_tag, T>
	{
	private:
		friend class list;

		std::shared_ptr<node> pnode = nullptr;

		// Ctor is private, so only friends can create instances.
		iterator() { };
		iterator(std::shared_ptr<node> n) : pnode(n) { }

		// These setter/getters are used by insert_after and erase_after.
		const std::shared_ptr<node> getNext() const { return pnode->next; }
		void setNext(const std::shared_ptr<node> n) { pnode->next = n; }

	public:
		bool operator== (const iterator& it) const { return pnode == it.pnode; }
		bool operator!= (const iterator& it) const { return pnode != it.pnode; }

		T& operator* () const { return pnode->element; }
		T* operator-> () const { return &pnode->element; }

		iterator operator++ ()
		{
			pnode = pnode->next;
			return iterator(pnode);
		}
		iterator operator++ (int)
		{
			auto temp = pnode;
			++* this;
			return iterator(temp);
		}

		iterator operator+ (int i)
		{
			while (i-- && pnode != nullptr)
				pnode = pnode->next;
			return iterator(pnode);
		}
	};

	iterator begin() const { return iterator(head); }
	iterator end() const { return iterator(tail->next); }

	iterator before_begin() const
	{
		auto proNode = std::make_shared<node>(head);
		return iterator(proNode);
	}

	void clear()
	{
		while (!empty())
			pop_front();

		head = tail = nullptr;
	}

	std::size_t size() const
	{
		std::size_t size = 0;
		for (auto n = head; n; n = n->next, size++);

		return size;
	}

	bool empty() const { return head == nullptr; }

	T& front() const
	{
		if (!empty())
			return head->element;
		else
			throw std::out_of_range("empty list");
	}

	T& back() const
	{
		if (!empty())
			return tail->element;
		else
			throw std::out_of_range("empty list");
	}

	template<typename ...Args>
	void emplace_front(Args&&... args)
	{
		auto newNode = std::make_shared<node>(std::forward<Args>(args)..., head);

		if (!tail)
			tail = newNode;

		head = newNode;
	}

	template<typename ...Args>
	void emplace_back(Args&&... args)
	{
		auto newNode = std::make_shared<node>(std::forward<Args>(args)..., nullptr);

		if (!head)
			head = newNode;

		if (tail)
			tail->next = newNode;

		tail = newNode;
	}

	void push_back(const T& e) { emplace_back(e); }
	void push_front(const T& e) { emplace_front(e); }

	void pop_front()
	{
		if (empty())
			return;

		if (tail == head)
			tail = nullptr; //tail.~shared_ptr();

		auto temp = head;
		head = std::move(head->next);
		temp.reset();
	}

	bool find(T d)
	{
		auto current = head;

		while (current != nullptr)
		{
			if (current->element == d)
				return true;

			current = current->next;
		}

		return false;
	}

	iterator insert_after(iterator it, const T& e) noexcept
	{
		auto newNode{ std::make_shared<node>(e, it.getNext()) };

		if (it.getNext() == head)
			head = newNode;
		else
			it.setNext(newNode);

		if (it == tail)
			tail = newNode;

		return iterator(newNode);
	}

	iterator erase_after(iterator it)
	{
		if (it == tail)
			return it;

		auto tmp = it.getNext();

		if (it.getNext() == head)
			head = it.getNext()->next;
		else
			it.setNext(it.getNext()->next);

		if (it.getNext() == tail)
			tail = it.pnode;

		tmp.reset();

		return it;
	}

	bool remove(T e)
	{
		if (empty())
			return false;

		auto cur = std::make_shared<node>(head);
		auto prv = std::make_shared<node>(head);

		while (cur)
		{
			if (cur->element == e)
				break;
			prv = cur;
			cur = cur->next;
		}

		if (cur == nullptr)
			return false;

		if (head == cur)
			head = cur->next;

		if (tail == cur)
			tail = prv;

		prv->next = cur->next; 
		cur.reset();

		return true;
	}

	void reverse()
	{
		if (empty() || !head->next)
			return;

		auto cur = std::make_shared<node>(head);
		std::shared_ptr<node> prv;
		std::shared_ptr<node> nxt;

		while (cur)
		{
			nxt = std::move(cur->next);
			cur->next = std::move(prv);
			prv = std::move(cur);
			cur = std::move(nxt);
		}

		std::swap(head, tail);
	}

	void resize(size_t n)
	{
		if (n >= size())
			return;

		if (n >= 1)
		{
			tail = (begin() + (n - 1)).pnode;
			tail->next = nullptr;
		}
		else
			clear();
	}

	friend std::ostream& operator<< (std::ostream& os, const list<T>& list)
	{
		for (iterator n = list.begin(); n != list.end(); n++)
			os << *n;
		return os << std::endl;
	}
};

#endif
