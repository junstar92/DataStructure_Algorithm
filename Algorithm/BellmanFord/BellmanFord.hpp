#ifndef __BELLMANFORD_HPP__
#define __BELLMANFORD_HPP__

#include <vector>

typedef struct _edge {
	int destination;
	int weight;
} EDGE;

typedef struct _graph {
	int num_node;
	std::vector<std::vector<EDGE>> edge;
} GRAPH;

void init_graph(GRAPH& graph, int num_node);
void add_edge(GRAPH& graph, int u, int v, int w, bool direction = false);
std::pair<std::vector<int>, std::vector<int>> bellmanford(GRAPH& graph, int src);
std::pair<std::vector<int>, std::vector<int>> bellmanford_queue(GRAPH& graph, int src);
// to check negative weight (negative cycle)
std::pair<std::pair<std::vector<int>, std::vector<int>>, bool> bellmanford_nc(GRAPH& graph, int src);
std::pair<std::pair<std::vector<int>, std::vector<int>>, bool> bellmanford_queue_nc(GRAPH& graph, int src);

#endif