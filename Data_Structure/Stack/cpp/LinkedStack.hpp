/* LinkedStack.hpp */

template<typename T>
int LinkedStack<T>::size() const
{
	return n;
}

template<typename T>
bool LinkedStack<T>::empty() const
{
	return (n == 0);
}

template<typename T>
T& LinkedStack<T>::top() const
{
	try {
		if (empty()) throw StackEmpty();
		return DLink.front();
	}
	catch (StackEmpty e) {
		std::cout << "Stack is empty\n";
		exit(2);
	}
}

template<typename T>
void LinkedStack<T>::push(const T& data)
{
	DLink.addFront(data);
	++n;
}

template<typename T>
void LinkedStack<T>::pop()
{
	try {
		if (empty()) throw StackEmpty();

		DLink.removeFront();
		--n;
	}
	catch (StackEmpty e) {
		cout << "Stack is empty\n";
		exit(2);
	}
}

template<typename T>
void LinkedStack<T>::print()
{
	std::cout << DLink;
}