#ifndef GRAPHTSP_HPP
#define GRAPHTSP_HPP

#include <bits/stdc++.h>

using namespace std;

class Graph {
  vector<vector<double>> adj;
  vector<pair<double, double>> cds;
 public:
  set<pair<double, vector < int >>, greater<pair<double, vector < int>> > > best_tours;
  Graph();
  Graph(int);
  void addNode(double, double);
  void addEdge(int, int, double);
  int getN();
  double tourCost(vector<int> &);
  vector<int> greedyTSP();
  void random_tour();
};

Graph input();
void generate_cycles(int, int, vector<vector<int>> &, vector<int>);
void cycle_crossover(vector<int> &, vector<int> &, vector<int> &, vector<int> &);
void newGeneration(vector<vector<int>> &, vector<vector<int>> &, Graph);
void runGenetic(Graph);
void random_tour(Graph);

#endif //GRAPHTSP_HPP
