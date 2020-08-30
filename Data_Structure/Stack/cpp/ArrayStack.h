/* ArrayStack.h */
#ifndef ArrayStack_H
#define ArrayStack_H

class StackEmpty {};
class StackFull {};

template<typename T>
class ArrayStack {
	enum { CAPACITY = 100 };
public :
	ArrayStack(int cap = CAPACITY) : Stack(new T[cap]), capacity(cap), t(-1) {}
	~ArrayStack();
	int size() const;
	bool empty() const;
	T& top() const;
	void push(const T& data);
	void pop();

	void print() const;

private:
	T* Stack;
	int capacity;
	int t;
};
#include "ArrayStack.hpp"
#endif