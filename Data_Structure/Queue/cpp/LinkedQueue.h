#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

class QueueEmpty{};

template<typename T>
class Node 
{
public:
	T data;
	Node<T> *next;

	Node() : next(nullptr) {}
	Node(T data, Node<T>* next) : data(data), next(next) {}
};

template<typename T>
class LinkedQueue
{
public :
	LinkedQueue() : n(0), head(nullptr), tail(nullptr) {}
	~LinkedQueue();

	void push(const T& data);
	T& front() const;
	void pop();
	int size() const;
	bool empty() const;
	void print() const;

private:
	int n;
	Node<T>* head;
	Node<T>* tail;
};

template<typename T>
LinkedQueue<T>::~LinkedQueue()
{
	Node<T>* delNode = head;
	while (head != nullptr)
	{
		head = head->next;
		delete delNode;
	}
}

template<typename T>
void LinkedQueue<T>::push(const T& data)
{
	if (empty())
	{
		head = new Node<T>(data, nullptr);
		tail = head;
	}
	else
	{
		tail->next = new Node<T>(data, nullptr);
		tail = tail->next;
	}

	++n;
}

template<typename T>
T& LinkedQueue<T>::front() const
{
	try {
		if (empty()) throw QueueEmpty();

		return head->data;
	}
	catch (QueueEmpty e) {
		cout << "Queue is empty\n";
		exit(2);
	}
}

template<typename T>
void LinkedQueue<T>::pop()
{
	try {
		if (empty()) throw QueueEmpty();

		Node<T>* delNode = head;
		head = head->next;

		delete delNode;
		--n;
		
		if (n == 0)
			tail = nullptr;
	}
	catch (QueueEmpty e) {
		cout << "Queue is empty\n";
		exit(2);
	}
}

template<typename T>
int LinkedQueue<T>::size() const
{
	return n;
}

template<typename T>
bool LinkedQueue<T>::empty() const
{
	return (n == 0);
}

template<typename T>
void LinkedQueue<T>::print() const
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