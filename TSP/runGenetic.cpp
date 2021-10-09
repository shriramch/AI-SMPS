#include "GraphTSP.hpp"

#define GEN_CNT 200
#define TIMEOUT 300

std::random_device rd1;     // only used once to initialise (seed) engine
std::mt19937 rng1(rd1());    // random-number engine used (Mersenne-Twister in this case)

bool randify = true;
bool debug1 = true;
bool printcost = true;

void printTour(vector<int> &tour) {
  int N = (int) tour.size();
  for (int i = 0; i < N; ++i) {
    cout << tour[i] << " ";
  }
  cout << endl;
}

void generate_cycles(int cnt, int N, vector<vector<int>> &cycles, Graph G, bool randify = false) {
  if(cycles.size()==0 || randify){
    if(debug1) cout<<"NEW POOL!\n";
    vector<int> temp(N);
    iota(temp.begin(), temp.end(), 0);
    cycles.clear();
    for (int i = 0; i < cnt; ++i) {
      cycles.push_back(temp);
      random_shuffle(cycles[i].begin(), cycles[i].end());
    }
  }
  
  std::uniform_int_distribution<int> uni1(0,cycles[0].size()-2); // guaranteed unbiased
  for(auto cycle : cycles){
    int i = 100;
    while(--i){
    int i1 = uni1(rng1);
    int i2 = uni1(rng1);
    if(i1 > i2) swap(i1, i2);
    auto oldCycle = cycle;
    reverse(cycle.begin() + i1, cycle.begin() + i2);
      if(G.tourCost(cycle)>G.tourCost(oldCycle)){
        cycle = oldCycle;
      }
    }
  }
  
}


void runGenetic(Graph G) {
  ofstream plot;
  plot.open ("plot.txt");
  int n = G.getN();
  srand(time(0));
  vector<vector<int>> cycles;
  generate_cycles(GEN_CNT, n, cycles, G);
  if(randify){
    std::uniform_int_distribution<int> uni2(0,cycles[0].size()-2); // guaranteed unbiased
    int k = 0;
    while(1){
      k++;
      int i1 = uni2(rng1);
      int i2 = uni2(rng1);
      if(i1 > i2) swap(i1, i2);
      auto oldCycle = cycles[0];
      reverse(cycles[0].begin() + i1, cycles[0].begin() + i2);
      if(G.tourCost(cycles[0])>=G.tourCost(oldCycle)){
          cycles[0] = oldCycle;
      }
      else{
        printTour(cycles[0]);
        if(debug1) cout<<"\ncost is = "<<G.tourCost(cycles[0])<<" "<<k<<endl;
        plot<<G.tourCost(cycles[0])<<",";
      }
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
  plot<<firstCost<<",";
  vector <int> bestTour;
  float bestCost = numeric_limits<float>::infinity();
  auto start = std::chrono::steady_clock::now();
  int r = 0;
  int g = 0;
  int bestRound = -1;
  while(true){ //make this while true
      if(debug1) {
        cout<<"round = "<<g<<" best round = "<<bestRound<<endl;
        cout<<"health check:";
        set <vector <int> > s;
        double cos = 0;
        double maxcost = -1;
        double mincost = 100000000000000;
        for(auto c : cycles){
          s.insert(c);
          maxcost = max(maxcost, G.tourCost(c));
          mincost = min(mincost,G.tourCost(c));
          cos+=G.tourCost(c);
        }
        cout<<s.size()<<"/"<<cycles.size()<<endl;
        cout<<"Average cost = "<<cos/cycles.size()<<endl;
        cout<<"Min cost = "<<mincost<<endl;
        cout<<"Max cost = "<<maxcost<<endl;
      }
      generate_cycles(GEN_CNT, n, cycles, G, (g - bestRound > 5000) && g > 2*bestRound);
      if(debug1){
        cout<<"after gencycles:";
        cout<<"health check:";
        set <vector <int> > s;
        double cos = 0;
        double maxcost = -1;
        double mincost = 100000000000000;
        for(auto c : cycles){
          s.insert(c);
          maxcost = max(maxcost, G.tourCost(c));
          mincost = min(mincost,G.tourCost(c));
          cos+=G.tourCost(c);
        }
        cout<<s.size()<<"/"<<cycles.size()<<endl;
        cout<<"Average cost = "<<cos/cycles.size()<<endl;
        cout<<"Min cost = "<<mincost<<endl;
        cout<<"Max cost = "<<maxcost<<endl;
        //exit(0);
      }
      if(g-bestRound>5000 && g > 2*bestRound) {
        bestRound = g;
      } 
      //cout<<"size of new cycles = "<<cycles.size()<<endl;
      r = bestRound;
      while (1) {
          r++;
          g++;
          vector<vector<int>> crossed(cycles);
          newGeneration(cycles, crossed, G);
          //cout<<"size of cycles = "<<cycles.size()<<endl;
          if(debug1){
            cout<<"after one crossover:";
            cout<<"health check:";
            set <vector <int> > s;
            double cos = 0;
            double maxcost = -1;
            double mincost = 100000000000000;
            for(auto c : crossed){
              s.insert(c);
              maxcost = max(maxcost, G.tourCost(c));
              mincost = min(mincost,G.tourCost(c));
              cos+=G.tourCost(c);
            }
            cout<<s.size()<<"/"<<crossed.size()<<endl;
            cout<<"Average cost = "<<cos/crossed.size()<<endl;
            cout<<"Min cost = "<<mincost<<endl;
            cout<<"Max cost = "<<maxcost<<endl;
          }
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
              if(printcost) cout<<"\nCost = "<<bestCost<<" "<<g<<endl;
              bestRound = r;
            }
            plot<<bestCost<<",";
            
          } else {
            float costi = G.tourCost(crossed[(*final.begin()).second.second]);
            if(costi < bestCost){
              bestCost = costi;
              bestTour = crossed[(*final.begin()).second.second];
              printTour(bestTour);
              if(printcost) cout<<"\nCost = "<<bestCost<<" "<<g<<endl;
              bestRound = r;
            }
            plot<<bestCost<<",";
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
          //if(std::chrono::steady_clock::now() - start > std::chrono::seconds{1}) break;
    }
  
  if(std::chrono::steady_clock::now() - start > std::chrono::seconds{TIMEOUT}) break;
  }
  plot.close();
}
