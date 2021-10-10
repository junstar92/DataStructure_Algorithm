#include "FloydWarshall.hpp"

void init_graph(GRAPH& graph, int num_node)
{
	graph.num_node = num_node;
	graph.edge.resize(num_node);
}

void add_edge(GRAPH& graph, int u, int v, int w, bool direction)
{
	graph.edge[u].push_back({ v, w });
	if (direction == false) {
		graph.edge[v].push_back({ u, w });
	}
}

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> floydwarshall(GRAPH& graph)
{
	std::vector<std::vector<int>> dist(graph.num_node, std::vector<int>(graph.num_node, INF));
	std::vector<std::vector<int>> pred(graph.num_node, std::vector<int>(graph.num_node, -1));

	for (int i = 0; i < graph.num_node; i++)
	{
		dist[i][i] = 0;
		for (auto& edge : graph.edge[i]) {
			int& j = edge.destination;
			int& w = edge.weight;

			dist[i][j] = w;
			pred[i][j] = i;
		}
	}

	for (int k = 0; k < graph.num_node; k++) {
		for (int i = 0; i < graph.num_node; i++) {
			for (int j = 0; j < graph.num_node; j++) {
				if ((dist[i][k] != INF || dist[j][k] != INF) && (dist[i][j] > dist[i][k] + dist[k][j])) {
					dist[i][j] = dist[i][k] + dist[k][j];
					pred[i][j] = pred[k][j];
				}
			}
		}
	}

	return { dist, pred };
}