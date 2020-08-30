/* DLinkedList.hpp */
template<typename T>
DLinkedList<T>::~DLinkedList()
{
	while (!empty())
		removeFront();
}

template<typename T>
void DLinkedList<T>::addFront(const T data)
{
	if (empty())
	{
		head = new DNode<T>(data, nullptr, nullptr);
		tail = head;
	}
	else
	{
		head = new DNode<T>(data, nullptr, head);
		head->next->prev = head;
	}

	++count;
}

template<typename T>
void DLinkedList<T>::removeFront()
{
	try {
		if (empty()) throw ListEmpty();

		DNode<T>* delNode = head;

		head = head->next;
		head->prev = nullptr;

		delete delNode;

		--count;
	}
	catch (ListEmpty e) {
		std::cout << "List is empty" << std::endl;
		exit(2);
	}
}

template<typename T>
void DLinkedList<T>::addRear(const T data)
{
	if (empty())
	{
		tail = new DNode<T>(data, nullptr, nullptr);
		head = tail;
	}
	else
	{
		tail = new DNode<T>(data, tail, nullptr);
		tail->prev->next = tail;
	}
	++count;
}

template<typename T>
void DLinkedList<T>::removeRear()
{
	try {
		if (empty()) throw ListEmpty();

		DNode<T>* delNode = tail;

		tail = tail->prev;
		tail->next = nullptr;

		delete delNode;

		--count;
	}
	catch (ListEmpty e) {
		std::cout << "List is empty" << std::endl;
		exit(2);
	}
}

template<typename T>
void DLinkedList<T>::insert(const T data, int k)
{
	try {
		if (k > count || k < 0) throw InvalidIndex();

		if (k == 0)
		{
			head = new DNode<T>(data, nullptr, nullptr);
			tail = head;
		}
		else
		{
			DNode<T> *preNode = head;
			DNode<T> *newNode = new DNode<T>(data, nullptr, nullptr);
			for (int i = 0; i < k - 1; i++)
				preNode = preNode->next;

			newNode->prev = preNode;
			newNode->next = preNode->next;

			preNode->next = newNode;
			newNode->next->prev = newNode;
		}
	}
	catch (InvalidIndex e) {
		std::cout << "Invliad Index" << std::endl;
		exit(2);
	}

	++count;
}

template<typename T>
void DLinkedList<T>::erase(int k)
{
	try {
		if (k >= count || k < 0) throw InvalidIndex();

		DNode<T>* preNode = head;
		if (k == 0)
		{
			head = head->next;
			head->prev = nullptr;

			delete preNode;
		}
		else
		{
			DNode<T>* delNode;
			for (int i = 0; i < k - 1; i++)
				preNode = preNode->next;

			delNode = preNode->next;
			preNode->next = delNode->next;
			if (k == count - 1)
				tail = preNode;
			else
				preNode->next->prev = preNode;

			delete delNode;
		}
	}
	catch (InvalidIndex e) {
		std::cout << "Invliad Index" << std::endl;
		exit(2);
	}

	--count;
}

template<typename T>
int DLinkedList<T>::search(const T data)
{
	DNode<T>* curNode = head;
	for (int i = 0; i < count; i++)
	{
		if (curNode->data == data)
			return i;

		curNode = curNode->next;
	}

	return -1;
}

template<typename T>
bool DLinkedList<T>::empty() const
{
	return (count == 0);
}

template<typename T>
T DLinkedList<T>::front() const
{
	try {
		if (empty()) throw ListEmpty();
	}
	catch (ListEmpty e) {
		std::cout << "Invliad Index" << std::endl;
		exit(2);
	}

	return head->data;
}

template<typename T>
ostream& operator<<(ostream& o, DLinkedList<T>& DList)
{
	DNode<T>* walkNode = DList.head;
	o << "[ ";
	for (int i = 0; i < DList.count; i++)
	{
		o << walkNode->data;
		walkNode = walkNode->next;
		if (i < DList.count - 1)
			o << " -> ";
	}
	o << " ]\n";

	return o;
}