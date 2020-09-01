/* LinkedDeque.h */
#ifndef LINKEDDEQUE_H
#define LINKEDDEQUE_H

class DequeEmpty{};

template<typename T>
class Node
{
public:
	T data;
	Node<T> *next, *prev;

	Node() : next(nullptr) {}
	Node(T data, Node<T>* next, Node<T>* prev) : data(data), next(next), prev(prev) {}
};

template<typename T>
class LinkedDeque
{
public:
	LinkedDeque() : n(0), head(nullptr), tail(nullptr) {}
	~LinkedDeque();

	void push_front(const T& data);
	void pop_front();
	T& front() const;
	void push_back(const T& data);
	void pop_back();
	T& back() const;
	int size() const;
	bool empty() const;
	void print() const;

private:
	int n;
	Node<T>* head;
	Node<T>* tail;
};

template<typename T>
LinkedDeque<T>::~LinkedDeque()
{
	Node<T>* delNode = head;
	while (head != nullptr)
	{
		head = head->next;
		delete delNode;
	}
}

template<typename T>
void LinkedDeque<T>::push_front(const T& data)
{
	if (empty())
	{
		head = new Node<T>(data, nullptr, nullptr);
		tail = head;
	}
	else
	{
		head = new Node<T>(data, head, nullptr);
		head->next->prev = head;
	}

	++n;
}

template<typename T>
void LinkedDeque<T>::pop_front()
{
	try {
		if (empty()) throw DequeEmpty();

		Node<T>* delNode = head;
		head = head->next;
		head->prev = nullptr;

		delete delNode;
		--n;

		if (n == 0)
			tail = nullptr;
	}
	catch (DequeEmpty e) {
		cout << "Deque is Empty\n";
		exit(2);
	}
}

template<typename T>
T& LinkedDeque<T>::front() const
{
	try {
		if (empty()) throw DequeEmpty();

		return head->data;
	}
	catch (DequeEmpty e) {
		cout << "Deque is Empty\n";
		exit(2);
	}
}

template<typename T>
void LinkedDeque<T>::push_back(const T& data)
{
	if (empty())
	{
		head = new Node<T>(data, nullptr, nullptr);
		tail = head;
	}
	else
	{
		tail = new Node<T>(data, nullptr, tail);
		tail->prev->next = tail;
	}

	++n;
}

template<typename T>
void LinkedDeque<T>::pop_back()
{
	try {
		if (empty()) throw DequeEmpty();

		Node<T>* delNode = tail;
		tail = tail->prev;
		tail->next = nullptr;

		delete delNode;
		--n;

		if (n == 0)
			head = nullptr;
	}
	catch (DequeEmpty e) {
		cout << "Deque is Empty\n";
		exit(2);
	}
}

template<typename T>
T& LinkedDeque<T>::back() const
{
	try {
		if (empty()) throw DequeEmpty();

		return tail->data;
	}
	catch (DequeEmpty e) {
		cout << "Deque is Empty\n";
		exit(2);
	}
}

template<typename T>
int LinkedDeque<T>::size() const
{
	return n;
}

template<typename T>
bool LinkedDeque<T>::empty() const
{
	return (n == 0);
}

template<typename T>
void LinkedDeque<T>::print() const
{
	Node<T>* walk = head;
	cout << "front [ ";
	while (walk != nullptr)
	{
		cout << walk->data << " ";
		walk = walk->next;
	}
	cout << "] rear\n";
}

#endif