/* LinkedStack.h */
#ifndef LinkedStack_H
#define LinkedStack_H

#include "DLinkedList.h"

class StackEmpty {};

template<typename T>
class LinkedStack
{
public:
	LinkedStack() : n(0) {}
	int size() const;
	bool empty() const;
	T& top() const;
	void push(const T& data);
	void pop();
	void print();

private:
	DLinkedList<T> DLink;
	int n;
};

#include "LinkedStack.hpp"
#endif