#include "GraphTSP.hpp"

bool debug = false;
std::random_device rd;
std::mt19937 rng(rd());

std::uniform_real_distribution<float> uniReal(0.0, 1.0);

void print(vector<int> tour) {
  if (!debug) return;
  for (auto e: tour) {
    cout << e << " ";
  }
  cout << endl;
}

void print(vector<double> tour) {
  if (!debug) return;
  for (auto e: tour) {
    cout << e << " ";
  }
  cout << endl;
}

void print(string k) {
  if (!debug) return;
  cout << k;
}

void mutate(vector<int> &pop) {
  std::uniform_int_distribution<int> uni(0, pop.size() - 2);
  double rndNumber = uniReal(rng);
  if (rndNumber < 0.5) {
    print("mutating");
    print("initial ");
    print(pop);
    int i1 = uni(rng);
    int i2 = uni(rng);
    if (i1 > i2) swap(i1, i2);
    reverse(pop.begin() + i1, pop.begin() + i2);
    print("after mut ");
    print(pop);
  }
}

void newGeneration(vector<vector<int>> &popIn, vector<vector<int>> &popOut, Graph G) {
  int popSize = popIn.size();
  vector<double> fitness;
  double totalFitness = 0;
  for (auto v: popIn) {
    double k = G.tourCost(v);
    fitness.push_back(1 / k);
    totalFitness += (1 / k);
  }

  vector<double> probab;
  for (auto f: fitness) {
    probab.push_back(f / totalFitness);
  }

  vector<int> matingPool;
  double rndNumber;
  double offset;
  print("probab is ");
  print(probab);
  print("fitness is ");
  print(fitness);
  for (int i = 0; i < popSize; i++) {
    rndNumber = uniReal(rng);
    offset = 0.0;
    for (int j = 0; j < popSize; j++) {
      offset += probab[j];
      if (rndNumber < offset) {
        matingPool.push_back(j);
        break;
      }
    }
  }
  print("mating pool is ");
  print(matingPool);
  for (int i = 0; i < popSize - 1; i += 2) {
    cycle_crossover(popIn[matingPool[i]], popIn[matingPool[i + 1]], popOut[i], popOut[i + 1]);
  }
  print("popOut pre mutation");
  for (auto pop: popOut) {
    print(pop);
  }
  for (int i = 0; i < popSize - 1; i++) {
    mutate(popOut[i]);
  }
  print("popout post mutation");
  for (auto pop: popOut) {
    print(pop);
  }
}