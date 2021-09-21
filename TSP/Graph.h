#ifndef AI_SMPS_GRAPH_H
#define AI_SMPS_GRAPH_H

#include <bits/stdc++.h>

class Graph {
  vector<vector<double>> adj;
  vector<pair<double, double>> cds;
 public:
  Graph();
  Graph(int);
  void addNode(double, double)
  void addEdge(double, double, double);
};

#endif //AI_SMPS_GRAPH_H
