#include "GraphTSP.hpp"

#define GEN_CNT 16


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
    // printTour(cycles[i]);
  }
  // cout<<"\n";
}


void runGenetic(Graph G) {
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
  // printTour(cycles[first]);
  int r = 0;
  while (true && r<1000) {
    r++;
    // cout<<"generation "<<r<<endl;
    // cout<<"the cycles of this generation are \n";
    // for (auto c : cycles){
    //   printTour(c);
    // }
    // cout<<"\noriginal costs = ";
    // for (auto c : cycles){
    //   cout<<G.tourCost(c)<<" ";
    // }
    vector<vector<int>> crossed(cycles);
    newGeneration(cycles, crossed, G);
    assert(cycles.size() == crossed.size());
    set < pair < double, pair < int, int >>, less < pair < double, pair < int, int > > > > final;
    int N = (int) cycles.size();
    // cout<<"the cycles of cross are \n";
    // for (auto c : crossed){
    //   printTour(c);
    // }
    //---------------------------------------------------
    // cout<<"\nCrossed costs = ";
    // for (auto c : crossed){
    //   cout<<G.tourCost(c)<<" ";
    // }
    // cout<<"\n";
    //--------------------------------------------------
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
    auto fi = final.begin();
    while ((int) new_cycles.size() < n) {
      if ((*fi).second.first == 1) {
        new_cycles.push_back(cycles[(*fi).second.second]);
      } else {
        new_cycles.push_back(crossed[(*fi).second.second]);
      }
      fi = next(fi);
    }
    // cout<<"the new ones are \n";
    // for (auto n : new_cycles){
    //   printTour(n);
    // }
    // cout<<"with costs \n";
    // for (auto n : new_cycles){
    //   cout<<G.tourCost(n)<<" ";
    // }
    // cout<<endl;
    cycles = new_cycles;
    crossed.clear();

  }
}
