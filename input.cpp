#include "Graph.h"

Graph input() {
  int N;
  cin >> N;
  Graph G(N);
  for (int i = 0; i < N; ++i) {
    double x, y;
    cin >> x >> y;
    G.addNode(x, y);
  }
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      double w;
      cin >> w;
      G.addEdge(i, j, w);
    }
  }
  return G;
}