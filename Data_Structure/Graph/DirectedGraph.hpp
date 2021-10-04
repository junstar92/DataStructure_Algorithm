#ifndef __DIRECTEDGRAPH_HPP__
#define __DIRECTEDGRAPH_HPP__

#include <iostream>
#include <functional>

template <typename T>
class DirectedGraph {
public:
	typedef struct vertex {
		struct vertex* pNextVertex;
		T data;
		uint32_t inDegree;
		uint32_t outDegree;
		struct arc* pArc;

		vertex() : pNextVertex(nullptr), inDegree(0), outDegree(0), pArc(nullptr) {}
	} VERTEX;

	typedef struct arc {
		struct vertex* destination;
		struct arc* pNextArc;
	} ARC;

private:
	uint32_t count;
	VERTEX* first;
	std::function<int(T, T)> compare;

public:
	DirectedGraph(std::function<int(const T&, const T&)> func) : count(0), first(nullptr), compare(func) {};
	~DirectedGraph();

	void insertVertex(T data);
	int deleteVertex(T delKey);
	int insertArc(T fromKey, T toKey);
	int deleteArc(T fromKey, T toKey);

	int graphCount() const {
		return this->count;
	}

	void printVertex() const;
};

template<typename T>
DirectedGraph<T>::~DirectedGraph()
{
	VERTEX* vertDelPtr;
	VERTEX* vertWalkPtr = this->first;

	while (vertWalkPtr) {
		ARC* arcDelPtr;
		ARC* arcWalkPtr = vertWalkPtr->pArc;

		while (arcWalkPtr) {
			arcDelPtr = arcWalkPtr;
			arcWalkPtr = arcWalkPtr->pNextArc;

			delete arcDelPtr;
		}

		vertDelPtr = vertWalkPtr;
		vertWalkPtr = vertWalkPtr->pNextVertex;

		delete vertDelPtr;
	}
}

template <typename T>
void DirectedGraph<T>::insertVertex(T data)
{
	VERTEX* newPtr;
	VERTEX* locPtr;
	VERTEX* predPtr;

	newPtr = new VERTEX();
	if (newPtr) {
		newPtr->pNextVertex = nullptr;
		newPtr->data = data;
		newPtr->inDegree = 0;
		newPtr->outDegree = 0;
		newPtr->pArc = nullptr;
		
		this->count++;
	}
	else {
		printf("Overflow Error 100\a\n");
		exit(100);
	}

	locPtr = this->first;
	if (!locPtr) {	// if there is no vertex, first is this vertex
		this->first = newPtr;
	}
	else {	// else, find insert point
		predPtr = nullptr;
		while (locPtr && (this->compare(newPtr->data, locPtr->data) > 0)) {
			predPtr = locPtr;
			locPtr = locPtr->pNextVertex;
		}

		if (!predPtr) {
			this->first = newPtr;
		}
		else {
			predPtr->pNextVertex = newPtr;
		}
		newPtr->pNextVertex = locPtr;
	}
}

template <typename T>
int DirectedGraph<T>::deleteVertex(T delKey)
{
	VERTEX* predPtr;
	VERTEX* walkPtr;

	if (!this->first)
		return -2;

	predPtr = nullptr;
	walkPtr = this->first;

	// find delKey
	while (walkPtr && (this->compare(delKey, walkPtr->data) > 0)) {
		predPtr = walkPtr;
		walkPtr->pNextVertex;
	}

	if (!walkPtr || (this->compare(delKey, walkPtr->data) != 0)) {
		return -2;
	}

	// if vertex has arc, failed to delete
	if ((walkPtr->inDegree > 0) || (walkPtr->outDegree > 0))
		return -1;

	if (!predPtr) {
		this->first = walkPtr->pNextVertex;
	}
	else {
		predPtr->pNextVertex = walkPtr->pNextVertex;
	}

	this->count--;
	delete walkPtr;

	return 1;
}

template <typename T>
int DirectedGraph<T>::insertArc(T fromKey, T toKey)
{
	ARC* newPtr;
	VERTEX* vertFromPtr;
	VERTEX* vertToPtr;

	newPtr = new ARC();
	if (!newPtr) { // failed to alloc
		return -1;
	}

	// find fromKey
	vertFromPtr = this->first;
	while (vertFromPtr && (this->compare(fromKey, vertFromPtr->data) > 0)) {
		vertFromPtr = vertFromPtr->pNextVertex;
	}
	if (!vertFromPtr || (this->compare(fromKey, vertFromPtr->data) != 0)) {
		return -2; // failed to find fromKey
	}

	// find toKey
	vertToPtr = this->first;
	while (vertToPtr && (this->compare(toKey, vertToPtr->data) > 0)) {
		vertToPtr = vertToPtr->pNextVertex;
	}
	if (!vertToPtr || (this->compare(toKey, vertToPtr->data) != 0)) {
		return -3; // failed to find toKey
	}

	++(vertFromPtr->outDegree);
	++(vertToPtr->inDegree);
	newPtr->destination = vertToPtr;
	if (!vertFromPtr->pArc) {
		vertFromPtr->pArc = newPtr;
		newPtr->pNextArc = nullptr;
		return 1;
	}

	// find insert point in adjacency arc list
	ARC* arcPredPtr = nullptr;
	ARC* arcWalkPtr = vertFromPtr->pArc;

	while (arcWalkPtr && (this->compare(toKey, arcWalkPtr->destination->data) >= 0)) {
		arcPredPtr = arcWalkPtr;
		arcWalkPtr = arcWalkPtr->pNextArc;
	}

	if (!arcPredPtr) {
		vertFromPtr->pArc = newPtr;
	}
	else {
		arcPredPtr->pNextArc = newPtr;
	}
	newPtr->pNextArc = arcWalkPtr;

	return 1;
}

template <typename T>
int DirectedGraph<T>::deleteArc(T fromKey, T toKey)
{
	VERTEX* vertFromPtr;
	VERTEX* vertToPtr;

	// find fromKey
	vertFromPtr = this->first;
	while (vertFromPtr && (this->compare(fromKey, vertFromPtr->data) > 0)) {
		vertFromPtr = vertFromPtr->pNextVertex;
	}

	if (!vertFromPtr || (this->compare(fromKey, vertFromPtr->data) != 0)) {
		return -2;	// failed to find fromKey
	}
	
	if (!vertFromPtr->pArc) {
		return -3;	// there is no adjacency edge
	}

	ARC* arcPredPtr = nullptr;
	ARC* arcWalkPtr = vertFromPtr->pArc;
	// find arc to tokey
	while (arcWalkPtr && (this->compare(toKey, arcWalkPtr->destination->data) > 0)) {
		arcPredPtr = arcWalkPtr;
		arcWalkPtr = arcWalkPtr->pNextArc;
	}
	
	if (!arcWalkPtr || (this->compare(toKey, arcWalkPtr->destination->data) != 0)) {
		return -3;	// failed to find arc to tokey
	}
	vertToPtr = arcWalkPtr->destination;

	// delete arc
	--(vertFromPtr->outDegree);
	--(vertToPtr->inDegree);

	if (!arcPredPtr) {
		vertFromPtr->pArc = arcWalkPtr->pNextArc;
	}
	else {
		arcPredPtr->pNexetArc = arcWalkPtr->pNextArc;
	}
	delete arcWalkPtr;

	return 1;
}

template <typename T>
void DirectedGraph<T>::printVertex() const
{
	VERTEX* walkPtr = this->first;
	ARC* arcWalkPtr;
	while (walkPtr) {
		std::cout << walkPtr->data;
		arcWalkPtr = walkPtr->pArc;
		if (arcWalkPtr) {
			std::cout << " -> ";
		}
		while (arcWalkPtr) {
			std::cout << arcWalkPtr->destination->data;
			if (arcWalkPtr->pNextArc) {
				std::cout << ", ";
			}
			arcWalkPtr = arcWalkPtr->pNextArc;
		}

		std::cout << "\n";
		walkPtr = walkPtr->pNextVertex;
	}
}

#endif