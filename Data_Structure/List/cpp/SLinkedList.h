#ifndef SLinkedList_H
#define SLinkedList_H
#include <stdlib.h>

template<typename T>
class SNode {
public:
	SNode(const T data, SNode<T>* next) : data(data), next(next) {}

	T data;
	SNode<T>* next;
};

template<typename T>
class SLinkedList {
public:
	SLinkedList() : count(0), head(nullptr), tail(nullptr) {}
	~SLinkedList();

	/* insert(const T& data, int k) : insert node at the k-th */
	void insert(const T data, int k);
	/* erase(int k) : erase node at the k-th */
	void erase(int k);
	/* search(const T data) : find index of data at first. Or return -1 if fail to find data */
	int search(const T data);
	/* isEmpty() : If SLinkedList is empty, return true. Or false */
	bool isEmpty() const;
	/* front() : Return the data of first node */
	T front() const;

	SNode<T>* head;
	SNode<T>* tail;
	int count;
};
#include "SLinkedList.hpp"
#endif