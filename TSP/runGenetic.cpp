#include "GraphTSP.hpp"

#define GEN_CNT 2000
#define TIMEOUT 310
#define ROUNDS 1000

void printTour(vector<int> &tour) {
  int N = (int) tour.size();
  for (int i = 0; i < N; ++i) {
    cout << tour[i] << " ";
  }
  cout << endl;
}

void generate_cycles(int cnt, int N, vector<vector<int>> &cycles) {
  vector<int> temp(N);
  iota(temp.begin(), temp.end(), 0);
  cycles.clear();
  for (int i = 0; i < cnt; ++i) {
    cycles.push_back(temp);
    random_shuffle(cycles[i].begin(), cycles[i].end());
  }
}

void combine_cycles(int cnt,
                    int N,
                    vector<vector<int>> &cycles,
                    set<pair<double, vector < int >>, greater<pair<double, vector < int>> > > &top_random) {
  generate_cycles(cnt, N, cycles);
  int i = 0;
  for (auto j = top_random.begin(); i < (int)cycles.size() && j != top_random.end(); ++i, ++j) {
    cycles[i] = (*j).second;
  }
}

void runGenetic(Graph G) {
  cout << "genetic thread" << endl;
  ofstream plot;
  plot.open("plot.txt");
  int n = G.getN();
  srand(time(0));
  vector<vector<int>> cycles;
  generate_cycles(GEN_CNT, n, cycles);
  //cycles[0] = G.greedyTSP();
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
  plot << firstCost << ",";
  vector<int> bestTour;
  float bestCost = numeric_limits<float>::infinity();
  auto start = std::chrono::steady_clock::now();
  int set = 0;
  while (true) { //make this while true
    if (set == 0) {
      generate_cycles(GEN_CNT, n, cycles);
    } else {
      auto top_random = G.best_tours;
      combine_cycles(GEN_CNT, n, cycles, top_random);
    }
    ++set;
    int r = 0;
    while (r < ROUNDS) {
      r++;
      vector<vector<int>> crossed(cycles);
      newGeneration(cycles, crossed, G);
      assert(cycles.size() == crossed.size());
      vector < pair < double, pair < int, int >>> final_c;
      int N = (int) cycles.size();
      for (int i = 0; i < N; ++i) {
        final_c.push_back({G.tourCost(cycles[i]), {1, i}});
      }
      for (int i = 0; i < N; ++i) {
        final_c.push_back({G.tourCost(crossed[i]), {2, i}});
      }
      if (final_c[0].second.first == 1) {
        float costi = G.tourCost(cycles[final_c[0].second.second]);
        if (costi < bestCost) {
          bestCost = costi;
          bestTour = cycles[final_c[0].second.second];
        }
        plot << bestCost << ",";
        printTour(bestTour);
      } else {
        float costi = G.tourCost(crossed[final_c[0].second.second]);
        if (costi < bestCost) {
          bestCost = costi;
          bestTour = crossed[final_c[0].second.second];
        }
        plot << bestCost << ",";
        printTour(bestTour);
      }
      vector<vector<int>> new_cycles;
      sort(final_c.begin(), final_c.end());
      reverse(final_c.begin(), final_c.end());
      auto fi = final_c.begin();
      while ((int) new_cycles.size() < n) {
        if ((*fi).second.first == 1) {
          new_cycles.push_back(cycles[(*fi).second.second]);
        } else {
          new_cycles.push_back(crossed[(*fi).second.second]);
        }
        fi = next(fi);
      }
      cycles = new_cycles;
      crossed.clear();
      //if(std::chrono::steady_clock::now() - start > std::chrono::seconds{1}) break;
    }

    if (std::chrono::steady_clock::now() - start > std::chrono::seconds{TIMEOUT}) break;
  }
  plot.close();
}
