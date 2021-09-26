#ifndef GRAPH_H
#define GRAPH_H

#include <bits/stdc++.h>

using namespace std;

class Graph {
  vector<vector<double>> adj;
  vector<pair<double, double>> cds;
 public:
  Graph();
  Graph(int);
  void addNode(double, double)
  void addEdge(double, double, double);
  int getN();
  double tourCost(vector<int> &);
};

void generate_cycles(int, int, vector<vector<int>> &);
void cycle_crossover(vector<int> &, vector<int> &, vector<int> &, vector<int> &);
void newGeneration(vector<vector<int>> &, vector<vector<int>> &);
void mix(vector<vector<int>> &, vector<vector<int>> &);

#endif //GRAPH_H
