#include "GraphTSP.hpp"

#define SET_CNT 400
#define TIMEOUT 310

void Graph::random() {
  int n = G.getN();
  vector<int> temp(n);
  iota(temp.begin(), temp.end(), 0);
  auto start = std::chrono::steady_clock::now();
  while (true) {
    random_shuffle(temp.begin(), temp.end());
    best_tours.insert({G.tourCost(temp), temp});
    if (best_tours.size() > SET_CNT) {
      auto fi = best_tours.end();
      --fi;
      best_tours.erase(fi);
    }
    if (std::chrono::steady_clock::now() - start > std::chrono::seconds{TIMEOUT}) break;
  }
}