#include "Graph.hpp"

void cycle_crossover(vector<int> &p1, vector<int> &p2, vector<int> &c1, vector<int> &c2) {
  int n = (int) p1.size();
  assert((int) p2.size() == n);
  vector<int> cycle;
  vector<int> vis(n, 0);
  vector<int> pos1(n);
  for (int i = 0; i < n; ++i) {
    pos1[p1[i]] = i;
  }
  int count = 1;
  for (int i = 0; i < n; ++i) {
    if (!vis[i]) {
      cycle.push_back(i);
    }
    int t = i;
    while (true) {
      if (cycle.front() == pos1[p2[t]]) {
        for (int v: cycle) {
          vis[v] = count;
        }
        cycle.clear();
        ++count;
        break;
      } else {
        cycle.push_back(pos1[p2[t]]);
        t = pos1[p2[t]];
      }
    }
  }
  c1.resize(n);
  c2.resize(n);
  for (int i = 0; i < n; ++i) {
    if (vis[i] & 1) {
      c1[i] = p1[i];
      c2[i] = p2[i];
    } else {
      c1[i] = p2[i];
      c2[i] = p1[i];
    }
  }
}

// int main() {
//  vector<int> p1 = {0, 5, 2, 3, 1, 4, 6};
//  vector<int> p2 = {5, 1, 4, 6, 2, 0, 3};
//  vector<int> c1, c2;
//  cycle_crossover(p1, p2, c1, c2);
//  return 0;
//}