#include "BellmanFord.hpp"
#include <queue>
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

std::pair<std::vector<int>, std::vector<int>> bellmanford_queue(GRAPH& graph, int src)
{
	std::vector<int> pred(graph.num_node, -1);
	std::vector<int> dist(graph.num_node, INF);
	std::vector<bool> inqueue(graph.num_node, false);
	std::queue<int> q;
	dist[src] = 0;

	q.push(src);
	inqueue[src] = true;
	
	while (q.size() != 0) {
		int u = q.front();
		q.pop();
		inqueue[u] = false;

		for (auto& edge : graph.edge[u]) {
			int& v = edge.destination;
			int& w = edge.weight;

			if ((dist[u] != INF) && (dist[v] > dist[u] + w)) {
				dist[v] = dist[u] + w;
				pred[v] = u;

				if (inqueue[v] == false) {
					q.push(v);
					inqueue[v] = true;
				}
			}
		}
	}

	return { pred, dist };
}

std::pair<std::pair<std::vector<int>, std::vector<int>>, bool> bellmanford_nc(GRAPH& graph, int src)
{
	std::vector<int> pred(graph.num_node, -1);
	std::vector<int> dist(graph.num_node, INF);
	dist[src] = 0;

	for (int i = 0; i < graph.num_node; i++) {
		for (int u = 0; u < graph.num_node; u++) {
			for (auto& edge : graph.edge[u]) {
				int& v = edge.destination;
				int& w = edge.weight;

				if ((dist[u] != INF) && (dist[v] > dist[u] + w)) {
					if (i == graph.num_node - 1) {
						return { {pred, dist}, false };
					}
					dist[v] = dist[u] + w;
					pred[v] = u;
				}
			}
		}
	}

	return { {pred, dist}, true };
}

std::pair<std::pair<std::vector<int>, std::vector<int>>, bool> bellmanford_queue_nc(GRAPH& graph, int src)
{
	std::vector<int> pred(graph.num_node, graph.num_node);
	std::vector<int> dist(graph.num_node, INF);
	std::vector<bool> inqueue(graph.num_node, false);
	std::queue<int> q;
	dist[src] = 0;

	q.push(src);
	inqueue[src] = true;
	q.push(graph.num_node);
	int i = 0;
	while ((q.size() != 1) && (i < graph.num_node)) {
		int u = q.front();
		q.pop();
		if (u == graph.num_node) {
			i += 1;
			q.push(graph.num_node);
		}
		else {
			inqueue[u] = false;

			for (auto& edge : graph.edge[u]) {
				int& v = edge.destination;
				int& w = edge.weight;

				if ((dist[u] != INF) && (dist[v] > dist[u] + w)) {
					dist[v] = dist[u] + w;
					pred[v] = u;

					if (inqueue[v] == false) {
						q.push(v);
						inqueue[v] = true;
					}
				}
			}
		}
	}

	return { {pred, dist}, i < graph.num_node };
}