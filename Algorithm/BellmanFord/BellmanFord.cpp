#include "BellmanFord.hpp"

#define INF 123456789

void init_graph(GRAPH& graph, int num_node)
{
	graph.num_node = num_node;
	graph.edge.resize(num_node, {});
}

void add_edge(GRAPH& graph, int u, int v, int w, bool direction)
{
	graph.edge[u].push_back({ v, w });
	if (direction == false)
		graph.edge[v].push_back({ u, w });
}

std::pair<std::vector<int>, std::vector<int>> bellmanford(GRAPH& graph, int src)
{
	std::vector<int> pred(graph.num_node, -1);
	std::vector<int> dist(graph.num_node, INF);
	dist[src] = 0;

	for (int i = 0; i < graph.num_node - 1; i++) {
		for (int u = 0; u < graph.num_node; u++) {
			for (auto& edge : graph.edge[u]) {
				int& v = edge.destination;
				int& w = edge.weight;

				if ((dist[u] != INF) && (dist[v] > dist[u] + w)) {
					dist[v] = dist[u] + w;
					pred[v] = u;
				}
			}
		}
	}

	return { pred, dist };
}