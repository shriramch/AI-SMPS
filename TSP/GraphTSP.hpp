#ifndef GRAPHTSP_HPP
#define GRAPHTSP_HPP

#include <bits/stdc++.h>

using namespace std;

class Graph {
  vector<vector<double>> adj;
  vector<pair<double, double>> cds;
 public:
  Graph();
  Graph(int);
  void addNode(double, double);
  void addEdge(int, int, double);
  int getN();
  double tourCost(vector<int> &);
  vector<int> greedyTSP();
};

Graph input();
void generate_cycles(int, int, vector<vector<int>> &);
void cycle_crossover(vector<int> &, vector<int> &, vector<int> &, vector<int> &);
void newGeneration(vector<vector<int>> &, vector<vector<int>> &, Graph );
void runGenetic(Graph);

#endif //GRAPHTSP_HPP
