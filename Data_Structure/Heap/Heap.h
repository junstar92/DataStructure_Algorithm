/* Heap.h */
#ifndef HEAP_H
#define HEAP_H

class HeapEmpty {};
class HeapFull {};

template<typename T>
class Heap
{
public:
	Heap(int maxSize = 100) : n(0), maxSize(maxSize), heapData(new T(maxSize)) {}
	~Heap() { delete heapData; }

	void push(const T& data);
	void pop();
	T& top() const;
	int size() const;
	bool empty() const;
	void print() const;

private:
	T* heapData;
	int n;
	int maxSize;
};

template<typename T>
void Heap<T>::push(const T& data)
{
	try {
		if (size() == maxSize) throw HeapFull();

		heapData[n] = data;

		int parent = (n - 1) / 2;
		int child = n;

		while (parent >= 0 && heapData[parent] < heapData[child])
		{
			T tmp = heapData[parent];
			heapData[parent] = heapData[child];
			heapData[child] = tmp;

			child = parent;
			parent = (child - 1) / 2;
		}

		++n;
	}
	catch (HeapFull e) {
		std::cout << "Heap is full\n";
		exit(2);
	}
}

template<typename T>
void Heap<T>::pop()
{
	try {
		if (empty()) throw HeapEmpty();

		heapData[0] = heapData[--n];

		int parent = 0;
		int child = parent * 2 + 1;
		bool placed = false;

		while (!placed && child < n)
		{
			if (child < n - 1 && heapData[child] < heapData[child + 1])
				child += 1;

			if (heapData[parent] >= heapData[child])
				placed = true;
			else
			{
				T tmp = heapData[parent];
				heapData[parent] = heapData[child];
				heapData[child] = tmp;
			}

			parent = child;
			child = parent * 2 + 1;
		}
	}
	catch (HeapEmpty e) {
		std::cout << "Heap is empty\n";
		exit(2);
	}
}

template<typename T>
T& Heap<T>::top() const
{
	try {
		if (empty()) throw HeapEmpty();

		return heapData[0];
	}
	catch (HeapEmpty e) {
		std::cout << "Heap is empty\n";
		exit(2);
	}
}

template<typename T>
int Heap<T>::size() const
{
	return n;
}

template<typename T>
bool Heap<T>::empty() const
{
	return (n == 0);
}

template<typename T>
void Heap<T>::print() const
{
	std::cout << "[ ";
	for (int i = 0; i < n; i++)
		std::cout << heapData[i] << " ";
	std::cout << "]\n";
}
#endif