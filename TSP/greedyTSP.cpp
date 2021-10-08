#include <bits/stdc++.h>

vector<int> GraphTSP::greedyTSP() {
  double cost = 0.0;
  double minE = DBL_MAX;
  set<int> vis;
  int n = 4;
  vector<int> cycle(n);
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (adj[i][j] < minE) {
        minE = adj[i][j];
        cycle[0] = i;
        cycle[1] = j;
      }
    }
  }
  vis.insert(cycle[0]);
  vis.insert(cycle[1]);
  for (int i = 2; i < n; ++i) {
    int cv = cycle[i - 1];
    int minD = cv;
    double val = DBL_MAX;
    for (int j = 0; j < n; ++j) {
      if (vis.find(j) == vis.end() && adj[cv][j] < val) {
        minD = j;
        val = adj[cv][j];
      }
    }
    vis.insert(minD);
    cycle[i] = minD;
  }
  return cycle;
}

// int main() {
// adj = {{-1, 30, 25, 10},
// {15, -1, 20, 40},
// {10, 20, -1, 25},
// {30, 10, 20, -1}};
// vector<int> cyc = greedyTSP();
// for (int i = 0; i < cyc.size(); ++i) {
// cout << cyc[i] << " ";
// }
// cout << endl;
// }