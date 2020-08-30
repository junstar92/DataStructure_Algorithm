/* DLinkedList.h */
#ifndef DLinkedList_H
#define DLinkedList_H

template<typename T>
class DNode {
public:
	DNode(const T data, DNode<T>* prev, DNode<T>* next) : data(data), prev(prev), next(next) {}

	T data;
	DNode<T> *prev, *next;
};
class InvalidIndex {};
class ListEmpty {};
template<typename T>
class DLinkedList {
public:
	DLinkedList() : count(0), head(nullptr), tail(nullptr) {}
	~DLinkedList();

	void addFront(const T data);
	void removeFront();
	void addRear(const T data);
	void removeRear();

	void insert(const T data, int k);
	void erase(int k);

	int search(const T data);
	bool empty() const;

	T front() const;

	DNode<T>* head;
	DNode<T>* tail;
	int count;
};
#include "DLinkedList.hpp"
#endif