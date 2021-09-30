#include "GraphTSP.hpp"

#define GEN_CNT 100


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


void runGenetic(Graph G) {
  ofstream plot;
  plot.open ("plot.txt");
  int n = G.getN();
  srand(time(0));
  vector<vector<int>> cycles;
  generate_cycles(GEN_CNT, n, cycles); // make this 2 * n so ensure that population is even
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
  plot<<firstCost<<",";
  vector <int> bestTour;
  float bestCost = numeric_limits<float>::infinity();
  int p = 0;
  while(p < 10){ //make this while true
    p++;
    int r = 0;
    generate_cycles(GEN_CNT, n, cycles); 
      while (true && r<1000) {
      r++;
      vector<vector<int>> crossed(cycles);
      newGeneration(cycles, crossed, G);
      assert(cycles.size() == crossed.size());
      set < pair < double, pair < int, int >>, less < pair < double, pair < int, int > > > > final;
      int N = (int) cycles.size();
      for (int i = 0; i < N; ++i) {
        final.insert({G.tourCost(cycles[i]), {1, i}});
      }
      for (int i = 0; i < N; ++i) {
        final.insert({G.tourCost(crossed[i]), {2, i}});
      }
      if ((*final.begin()).second.first == 1) {
        float costi = G.tourCost(cycles[(*final.begin()).second.second]);
        if(costi < bestCost){
          bestCost = costi;
          bestTour = cycles[(*final.begin()).second.second];
        }
        plot<<bestCost<<",";
        printTour(bestTour);
      } else {
        float costi = G.tourCost(crossed[(*final.begin()).second.second]);
        if(costi < bestCost){
          bestCost = costi;
          bestTour = crossed[(*final.begin()).second.second];
        }
        plot<<bestCost<<",";
        printTour(bestTour);
      }
      vector<vector<int>> new_cycles;
      auto fi = final.begin();
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

    }
 
  }
  plot.close();
}
