#include "GraphTSP.hpp"

#define GEN_CNT 4

void generate_cycles(int cnt, int N, vector<vector<int>> &cycles) {
  vector<int> temp(N);
  iota(temp.begin(), temp.end(), 0);
  cycles.clear();
  for (int i = 0; i < cnt; ++i) {
    cycles.push_back(temp);
    random_shuffle(cycles[i].begin(), cycles[i].end());
  }
}

void printTour(vector<int> &tour) {
  int N = (int) tour.size();
  for (int i = 0; i < N; ++i) {
    cout << tour[i] << " ";
  }
  cout << endl;
}

void runGenetic(Graph G) {
  int n = G.getN();
  vector<vector<int>> cycles;
  generate_cycles(GEN_CNT, n, cycles);
  assert(!((int) cycles.size() & 1));
  int first = -1;
  double firstCost = DBL_MAX;
  for (int i = 0; i < (int) cycles.size(); ++i) {
    double curCost = G.tourCost(cycles[i]);
    if (curCost < firstCost) {
      first = i;
      firstCost = curCost;
    }
  }
  printTour(cycles[first]);
  
  while (true) {
    vector<vector<int>> crossed(cycles);
    newGeneration(cycles, crossed, G);
    assert(cycles.size() == crossed.size());
    set < pair < double, pair < int, int >>, greater < pair < double, pair < int, int > > > > final;
    int N = (int) cycles.size();
    for (int i = 0; i < N; ++i) {
      final.insert({G.tourCost(cycles[i]), {1, i}});
    }
    for (int i = 0; i < N; ++i) {
      final.insert({G.tourCost(crossed[i]), {2, i}});
    }
    if ((*final.begin()).second.first == 1) {
      printTour(cycles[(*final.begin()).second.second]);
    } else {
      printTour(crossed[(*final.begin()).second.second]);
    }
    vector<vector<int>> new_cycles;
    while ((int) new_cycles.size() < n) {
      auto i = *final.begin();
      if (i.second.first == 1) {
        new_cycles.push_back(cycles[i.second.second]);
      } else {
        new_cycles.push_back(crossed[i.second.second]);
      }
    }
    cycles = new_cycles;
    crossed.clear();
  }
}
