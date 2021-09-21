#include "Graph.h"

Graph::Graph() {}

Graph::Graph(int n) {
  adj.assign(n, vector<double>(n, 0.0));
}

void Graph::addNode(double u, double v) {
  cds.push_back({u, v});
}

void Graph::addEdge(int u, int v, double w) {
  adj[u][v] = adj[v][u] = w;
}
