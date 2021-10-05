#ifndef __DIRECTEDGRAPH_HPP__
#define __DIRECTEDGRAPH_HPP__

#include <iostream>
#include <functional>
#include <list>
#include <map>
#include <queue>
#include <tuple>

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
		int32_t weight;
	} ARC;

private:
	uint32_t count;
	VERTEX* first;
	std::function<int(T, T)> compare;

	void _topologicalSort_DFS(const VERTEX* curVertex, std::map<T, bool>& visited, std::list<T>& sorted);

public:
	DirectedGraph(std::function<int(const T&, const T&)> func) : count(0), first(nullptr), compare(func) {};
	~DirectedGraph();

	void insertVertex(T data);
	int deleteVertex(T delKey);
	int insertArc(T fromKey, T toKey, int32_t weight = 0);
	int deleteArc(T fromKey, T toKey);

	int graphCount() const { return this->count; }
	void printVertex() const;

	std::list<T> topologicalSort_DFS();
	std::list<T> topologicalSort_Q();

	std::tuple<std::map<T, T>, std::map<T, int32_t>> criticalPath(T start);
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
int DirectedGraph<T>::insertArc(T fromKey, T toKey, int32_t weight)
{
	ARC* newPtr;
	VERTEX* vertFromPtr;
	VERTEX* vertToPtr;

	newPtr = new ARC();
	if (!newPtr) { // failed to alloc
		return -1;
	}

	// set weight
	newPtr->weight = weight;

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

template <typename T>
void DirectedGraph<T>::_topologicalSort_DFS(const VERTEX* curVertex, std::map<T, bool>& visited, std::list<T>& sorted)
{
	visited[curVertex->data] = true;

	const ARC* walkArcPtr = curVertex->pArc;
	while (walkArcPtr) {
		if (!visited[walkArcPtr->destination->data]) {
			this->_topologicalSort_DFS(walkArcPtr->destination, visited, sorted);
		}
		walkArcPtr = walkArcPtr->pNextArc;
	}

	sorted.push_front(curVertex->data);
}

template <typename T>
std::list<T> DirectedGraph<T>::topologicalSort_DFS()
{
	std::map<T, bool> visited;
	
	const VERTEX* walkPtr = this->first;
	while (walkPtr) {	// visited initiation
		visited[walkPtr->data] = false;
		walkPtr = walkPtr->pNextVertex;
	}

	std::list<T> sorted;
	walkPtr = this->first;
	while (walkPtr) {
		if (!visited[walkPtr->data])
			this->_topologicalSort_DFS(walkPtr, visited, sorted);
		walkPtr = walkPtr->pNextVertex;
	}

	return sorted;
}

template <typename T>
std::list<T> DirectedGraph<T>::topologicalSort_Q()
{
	std::map<T, uint32_t> inDegrees;
	std::queue<const VERTEX*> q;

	const VERTEX* walkPtr = this->first;
	while (walkPtr) {	// inDegrees initiation
		inDegrees[walkPtr->data] = walkPtr->inDegree;

		if (walkPtr->inDegree == 0)
			q.push(walkPtr);

		walkPtr = walkPtr->pNextVertex;
	}

	std::list<T> sorted;
	for (int i = 0; i < this->count; i++) {
		if (q.empty()) {
			std::cout << "Failed to sort\n";
			sorted.clear();
			return sorted;
		}

		const VERTEX* curPtr = q.front();
		q.pop();
		sorted.push_back(curPtr->data);
		std::cout << curPtr->data << "\n";

		const ARC* walkArcPtr = curPtr->pArc;
		while (walkArcPtr) {	// search adj vertex
			inDegrees[walkArcPtr->destination->data]--;
			if (inDegrees[walkArcPtr->destination->data] == 0)
				q.push(walkArcPtr->destination);

			walkArcPtr = walkArcPtr->pNextArc;
		}
	}

	return sorted;
}

template <typename T>
std::tuple<std::map<T, T>, std::map<T, int32_t>> DirectedGraph<T>::criticalPath(T start)
{
	std::map<T, int32_t> pred;
	std::map<T, int32_t> dist;
	std::map<T, VERTEX*> vertices;

	VERTEX* walkVertPtr = this->first;
	// dist, pred initiation, set vertex mapping table
	while (walkVertPtr) {
		dist[walkVertPtr->data] = (1 << 32);
		pred[walkVertPtr->data] = -1;
		vertices[walkVertPtr->data] = walkVertPtr;

		walkVertPtr = walkVertPtr->pNextVertex;
	}

	dist[start] = 0;
	auto sorted = this->topologicalSort_DFS();

	for (auto& u : sorted) {
		ARC* walkArcPtr = vertices[u]->pArc;

		while (walkArcPtr) {
			T v = walkArcPtr->destination->data;
			if (dist[v] < dist[u] + walkArcPtr->weight) {
				dist[v] = dist[u] + walkArcPtr->weight;
				pred[v] = u;
			}

			walkArcPtr = walkArcPtr->pNextArc;
		}
	}
	
	return { pred, dist };
}

#endif