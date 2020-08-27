/* SLinkedList.hpp */

/* Destructor */
template<typename T>
SLinkedList<T>::~SLinkedList()
{
	while (!isEmpty())
		erase(0);
}

template<typename T>
void SLinkedList<T>::insert(const T data, int k)
{
	if (k > count || k < 0)
		return;

	if (k == 0)
	{
		head = new SNode<T>(data, head);
		tail = head;
	}
	else
	{
		SNode<T>* preNode = head;

		for (int i = 0; i < k - 1; i++)
			preNode = preNode->next;

		preNode->next = new SNode<T>(data, preNode->next);

		if (k == count)
			tail = preNode->next;
	}

	count++;
}

template<typename T>
void SLinkedList<T>::erase(int k)
{
	if (k >= count || k < 0)
		return;
	SNode<T>* preNode = head;
	if (k == 0)
	{
		head = head->next;
		delete preNode;
	}
	else
	{
		SNode<T>* delNode;

		for (int i = 0; i < k - 1; i++)
			preNode = preNode->next;

		delNode = preNode->next;
		preNode->next = preNode->next->next;
		delete delNode;

		if (k == count - 1)
			tail = preNode;
	}

	count--;
}

template<typename T>
int SLinkedList<T>::search(const T data)
{
	SNode<T>* curNode = head;
	for (int i = 0; i < count; i++)
	{
		if (curNode->data == data)
			return i;

		curNode = curNode->next;
	}

	return -1;
}

template<typename T>
bool SLinkedList<T>::isEmpty() const
{
	return (count == 0);
}

template<typename T>
T SLinkedList<T>::front() const
{
	if (isEmpty())
		exit(1); //wrong access

	return head->data;
}

//for printing SLinkedList
template<typename T>
ostream& operator<<(ostream& o, const SLinkedList<T>& SList)
{
	SNode<T>* walkNode = SList.head;
	o << "[ ";
	for (int i = 0; i < SList.count; i++)
	{
		o << walkNode->data;
		walkNode = walkNode->next;
		if (i < SList.count - 1)
			o << " -> ";
	}
	o << " ]\n";

	return o;
}