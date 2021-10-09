#include "GraphTSP.hpp"
#include <thread>
#define GEN_CNT 100
#define TIMEOUT 310

std::random_device rd1;     // only used once to initialise (seed) engine
std::mt19937 rng1(rd1());    // random-number engine used (Mersenne-Twister in this case)
std::uniform_real_distribution<float> uniReal2(0.0,1.0);


float bestCost = numeric_limits<float>::infinity();
vector <int> bestTour;


bool grandify = true;
bool printcost = false;

void printTour(vector<int> &tour) {
  int N = (int) tour.size();
  for (int i = 0; i < N; ++i) {
    cout << tour[i] << " ";
  }
  cout << endl;
}

void generate_cycles(int cnt, int N, vector<vector<int>> &cycles, Graph G, bool randify = false) {
  if(cycles.size()==0 || randify){
    vector<int> temp(N);
    iota(temp.begin(), temp.end(), 0);
    cycles.clear();
    for (int i = 0; i < cnt; ++i) {
      cycles.push_back(temp);
      random_shuffle(cycles[i].begin(), cycles[i].end());
    }
    if(grandify){
      std::uniform_int_distribution<int> uni2(0,cycles[0].size()-2); // guaranteed unbiased
      double t = 15/(double)cnt;
      t*=1000;
      int k = t;
      for(int i = 0; i<cycles.size(); i++){
        auto start = std::chrono::steady_clock::now();
        while(std::chrono::steady_clock::now() - start < std::chrono::milliseconds{k}){
          int i1 = uni2(rng1);
          int i2 = uni2(rng1);
          //double mut = uniReal2(rng1);
          if(i1 > i2) swap(i1, i2);
          auto oldCycle = cycles[i];
          reverse(cycles[i].begin() + i1, cycles[i].begin() + i2);
          if(G.tourCost(cycles[i])>=G.tourCost(oldCycle)){
              cycles[i] = oldCycle;
          }
          else{
            double cos = G.tourCost(cycles[i]);
            if(bestCost>cos){
              bestCost = cos;
              bestTour = cycles[i];
              printTour(bestTour);
              if(printcost) cout<<"cost = "<<bestCost;
            }
          }
        }
      }
    }
  }
  else{
    std::uniform_int_distribution<int> uni1(0,cycles[0].size()-2); // guaranteed unbiased
    for(int i = 0; i<cycles.size(); i++){
      int i1 = uni1(rng1);
      int i2 = uni1(rng1);
      if(i1 > i2) swap(i1, i2);
      auto oldCycle = cycles[i];
      reverse(cycles[i].begin() + i1, cycles[i].begin() + i2);
    }
  }
  if(G.tourCost(cycles[0])<bestCost){
    bestTour = cycles[0];
    bestCost = G.tourCost(cycles[0]);
  }
}

void printTours()
{
  ofstream plot;
  plot.open("plot.txt");
  plot.close();
  
  while(1){
    auto start = std::chrono::steady_clock::now();
    plot.open ("plot.txt", std::ios_base::app);
    plot<<bestCost<<",";
    plot.close();
    while(std::chrono::steady_clock::now() - start < std::chrono::milliseconds{100}){
      ;
    }
  }

}

void runGenetic(Graph G) {
  
  
  int n = G.getN();
  srand(time(0));
  vector<vector<int>> cycles;
  vector<int> tem(n);
  bestTour = tem;
  iota(bestTour.begin(), bestTour.end(), 0);
  bestCost = G.tourCost(bestTour);
  thread t1(printTours);
  generate_cycles(GEN_CNT, n, cycles, G, false);
  printTour(bestTour);
  if(printcost) cout<<"cost = "<<G.tourCost(bestTour)<<endl;
  if(grandify){
    std::uniform_int_distribution<int> uni2(0,cycles[0].size()-2); // guaranteed unbiased
    int k = 0;
    int tot = 0;
    while(1){
      tot++;
      int i1 = uni2(rng1);
      int i2 = uni2(rng1);
      if(i1 > i2) swap(i1, i2);
      vector<int>oldCycle(cycles[0]);
      reverse(cycles[0].begin() + i1, cycles[0].begin() + i2);
      if(G.tourCost(cycles[0])>=G.tourCost(oldCycle)){
          cycles[0] = oldCycle;
      }
      else{
        // plot<<G.tourCost(cycles[0])<<",";
        k = tot;
        double cos = G.tourCost(cycles[0]);
        if(bestCost>cos){
          bestCost = cos;
          bestTour = cycles[0];
          printTour(bestTour);
          if(printcost) cout<<"cost = "<<bestCost;
        }
      }
      if(tot - k > 1000000) break;
    }
  }
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
  //plot<<firstCost<<",";
  auto start = std::chrono::steady_clock::now();
  int r = 0;
  int g = 0;
  int bestRound = -1;
  while(true){ //make this while true
      if(g>1) generate_cycles(GEN_CNT, n, cycles, G, (g - bestRound > 5000) && g > 2*bestRound);
      if(g-bestRound>5000 && g > 2*bestRound) {
        bestRound = g;
      } 
      r = bestRound;
      while (1) {
          r++;
          g++;
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
              printTour(bestTour);
              if(printcost) cout<<"cost = "<<bestCost<<endl;
              bestRound = r;
            }
            //plot<<bestCost<<",";
            
          } else {
            float costi = G.tourCost(crossed[(*final.begin()).second.second]);
            if(costi < bestCost){
              bestCost = costi;
              bestTour = crossed[(*final.begin()).second.second];
              printTour(bestTour);
              bestRound = r;
              if(printcost) cout<<"cost = "<<bestCost<<endl;

            }
            //plot<<bestCost<<",";
          }
          vector<vector<int>> new_cycles;
          auto fi = final.begin();
          while ((int) new_cycles.size() < GEN_CNT) {
            if ((*fi).second.first == 1) {
              new_cycles.push_back(cycles[(*fi).second.second]);
            } else {
              new_cycles.push_back(crossed[(*fi).second.second]);
            }
            fi = next(fi);
          }
          cycles = new_cycles;
          crossed.clear();
          if(r - bestRound > 200){
            break;
          }
    }
  
  if(std::chrono::steady_clock::now() - start > std::chrono::seconds{TIMEOUT}) break;
  }
  exit(0);
}
