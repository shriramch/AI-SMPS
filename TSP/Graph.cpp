#include "GraphTSP.hpp"

Graph::Graph() {}

Graph::Graph(int n) {
  adj.assign(n, vector<double>(n, 0.0));
}

void Graph::addNode(double u, double v) {
  cds.push_back({u, v});
}

void Graph::addEdge(int u, int v, double w) {
  adj[u][v] = w;
}

int Graph::getN() {
  return (int) adj.size();
}

double Graph::tourCost(vector<int> &tour) {
  double cost = 0.0;
  for (int i = 1; i < (int) tour.size(); ++i) {
    cost += adj[tour[i]][tour[i - 1]];
  }
  return cost + adj[tour[(int) tour.size() - 1]][tour[0]];
}