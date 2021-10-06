#include "Dijkstra.hpp"
#include <queue>

class Heap {
public:
	void push(const IDX_DIST& data);
	void pop();
	IDX_DIST top() const;
	int size() const;
	void update(int node, int dist);

	Heap(int num_node) : max_size(num_node * 10), num_data(0), num_node(num_node)
	{
		this->heapData.resize(this->max_size);
		this->pos.resize(this->num_node, -1);
	}

private:
	std::vector<IDX_DIST> heapData;
	int max_size;
	int num_data;
	int num_node;
	std::vector<int> pos;

	void _sinkUp(int idx);
	void _sinkDown(int idx);
};

void init_graph(GRAPH& graph, int num_node)
{
	graph.num_node = num_node;
	graph.adjs.resize(num_node);
}

void add_edge(GRAPH& graph, int u, int v, int d, bool direction)
{
	graph.adjs[u].push_back({ v, d });
	if (!direction)
		graph.adjs[v].push_back({ u, d });
}

std::pair<std::vector<int>, std::vector<int>> dikstra(GRAPH& graph, int s)
{
	if (graph.num_node < s) {
		exit(-1);
	}

	std::vector<int> pred(graph.num_node, -1);
	std::vector<int> dist(graph.num_node, INF);
	std::vector<int> cnt(graph.num_node, 0);
	Heap pq(graph.num_node);

	dist[s] = 0;

	for (int i = 0; i < graph.num_node; i++) {
		pq.push({ i, dist[i] });
	}

	while (pq.size() != 0) {
		auto idx_dist = pq.top();
		pq.pop();

		int& u = idx_dist.node;
		cnt[u]++;
		for (auto& adj : graph.adjs[u]) {
			int& v = adj.node;

			if ((dist[u] != INF) && (dist[v] > dist[u] + adj.dist)) {
				dist[v] = dist[u] + adj.dist;
				pred[v] = u;
				
				pq.update(v, dist[v]);
			}
		}
	}

	return { pred, dist };
}

void Heap::push(const IDX_DIST& data)
{
	if (this->num_data == this->max_size)
		return;

	this->heapData[this->num_data] = data;
	this->pos[data.node] = this->num_data;

	this->_sinkUp(this->num_data);
	++this->num_data;

	return;
}

void Heap::pop()
{
	if (this->num_data == 0)
		return;

	this->pos[this->heapData[0].node] = -1;
	this->heapData[0] = this->heapData[--this->num_data];
	this->pos[this->heapData[0].node] = 0;
	
	this->_sinkDown(0);

	return;
}

IDX_DIST Heap::top() const
{
	if (this->num_data == 0)
		return {};

	return this->heapData[0];
}

int Heap::size() const
{
	return this->num_data;
}

void Heap::update(int node, int dist)
{
	if (this->pos[node] == -1) {
		exit(-1);
	}
	int position = this->pos[node];
	int prev_dist = this->heapData[position].dist;
	this->heapData[position].dist = dist;
	if (prev_dist < dist)
		_sinkDown(position);
	else
		_sinkUp(position);

	return;
}

void Heap::_sinkUp(int idx)
{
	int child = idx;
	int parent = (idx - 1) / 2;

	while (parent >= 0 && this->heapData[child] < this->heapData[parent]) {
		IDX_DIST tmp = this->heapData[parent];
		this->heapData[parent] = this->heapData[child];
		this->heapData[child] = tmp;

		// update position
		this->pos[this->heapData[parent].node] = parent;
		this->pos[this->heapData[child].node] = child;

		child = parent;
		parent = (child - 1) / 2;
	}

	return;
}

void Heap::_sinkDown(int idx)
{
	int parent = idx;
	int child = 2 * parent + 1;
	bool placed = false;

	while (!placed && child < this->num_data)
	{
		if (child < this->num_data - 1 && this->heapData[child + 1] < this->heapData[child]) {
			child += 1;
		}

		if (this->heapData[parent] < this->heapData[child]) {
			placed = true;
		}
		else {
			IDX_DIST tmp = this->heapData[parent];
			this->heapData[parent] = this->heapData[child];
			this->heapData[child] = tmp;

			// update position
			this->pos[this->heapData[parent].node] = parent;
			this->pos[this->heapData[child].node] = child;
		}

		parent = child;
		child = 2 * parent + 1;
	}

	return;
}