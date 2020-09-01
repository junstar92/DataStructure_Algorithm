/* ArrayQueue.h */
#ifndef ARRAYQUEUE_H
#define ARRAYQUEUE_H
class QueueEmpty{};
class QueueFull{};

template<typename T>
class ArrayQueue
{
public:
	ArrayQueue(int Capacity = 100) : N(Capacity), n(0), f(0), r(0), Q(new T[Capacity]) {}
	~ArrayQueue();

	void push(const T& data);
	void pop();
	T& front() const;
	bool empty() const;
	int size() const;
	void print() const;

private:
	int N;
	int n;
	int f, r;
	T* Q;
};

template<typename T>
ArrayQueue<T>::~ArrayQueue()
{
	delete Q;
}

template<typename T>
void ArrayQueue<T>::push(const T& data)
{
	try {
		if (size() == N) throw QueueFull();

		Q[r] = data;
		r = (r + 1) % N;
		++n;
	}
	catch (QueueFull e) {
		cout << "Queue is Full\n";
		exit(2);
	}
}

template<typename T>
void ArrayQueue<T>::pop()
{
	try {
		if (empty()) throw QueueEmpty();

		f = (f + 1) % N;
		--n;
	}
	catch (QueueEmpty e) {
		cout << "Queue is Empty\n";
		exit(2);
	}
}

template<typename T>
T& ArrayQueue<T>::front() const
{
	try {
		if (empty()) throw QueueEmpty();

		return Q[f];
	}
	catch (QueueEmpty e) {
		cout << "Queue is Empty\n";
		exit(2);
	}
}

template<typename T>
bool ArrayQueue<T>::empty() const
{
	return (n == 0);
}

template<typename T>
int ArrayQueue<T>::size() const
{
	return n;
}

template<typename T>
void ArrayQueue<T>::print() const
{
	cout << "front [ ";
	for (int i = f; i != r; i = (i + 1) % N)
	{
		cout << Q[i] << " ";
	}
	cout << "] rear\n";
}
#endif