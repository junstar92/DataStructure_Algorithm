/* ArrayStack.hpp */

template<typename T>
ArrayStack<T>::~ArrayStack()
{
	delete Stack;
}

template<typename T>
int ArrayStack<T>::size() const
{
	return (t + 1);
}

template<typename T>
bool ArrayStack<T>::empty() const
{
	return (t < 0);
}

template<typename T>
T& ArrayStack<T>::top() const
{
	try {
		if (empty()) throw StackEmpty();

		return Stack[t];
	}
	catch (StackEmpty e) {
		std::cout << "Stack is empty\n";
		exit(2);
	}
}

template<typename T>
void ArrayStack<T>::push(const T& data)
{
	try {
		if (size() == capacity) throw StackFull();

		Stack[++t] = data;
	}
	catch (StackFull e) {
		std::cout << "Stack is full\n";
		exit(2);
	}
}

template<typename T>
void ArrayStack<T>::pop()
{
	try {
		if (empty()) throw StackEmpty();

		--t;
	}
	catch (StackEmpty e){
		std::cout << "Stack is empty\n";
		exit(2);
	}
}

template<typename T>
void ArrayStack<T>::print() const
{
	int s = size();
	std::cout << " bottom [ ";
	for (int i = 0; i < s; i++)
	{
		std::cout << Stack[i] << " ";
	}
	std::cout << "] top\n";
}