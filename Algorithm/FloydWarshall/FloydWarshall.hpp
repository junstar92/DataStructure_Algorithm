#ifndef __FLOYDWARSHALL_HPP__
#define __FLOYDWARSHALL_HPP__

#include <vector>

#define INF 123456789

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
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> floydwarshall(GRAPH& graph);

#endif