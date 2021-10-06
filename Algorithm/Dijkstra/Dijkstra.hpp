#ifndef __DIJKSTRA_HPP__
#define __DIJKSTRA_HPP__

#include <vector>

#define INF	123456789

typedef struct _idx_dist {
	int node;
	int dist;

	bool operator()(const struct _idx_dist& a, const struct _idx_dist& b) const {
		return a.dist < b.dist;
	}
	bool operator<(const struct _idx_dist& o) const {
		return dist < o.dist;
	}
} IDX_DIST;

typedef struct _graph {
	int num_node;
	std::vector<std::vector<IDX_DIST>> adjs;
} GRAPH;

void init_graph(GRAPH& graph, int num_node);
void add_edge(GRAPH& graph, int u, int v, int d, bool direction = false);
std::pair<std::vector<int>, std::vector<int>> dikstra(GRAPH& graph, int s);

#endif