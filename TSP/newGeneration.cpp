/**
 * 
 * Definitions and assumptions
 * 1. The number of cities 'numCities' is known
 * 2. A 'city' is an integer between [0, numCities-1]
 * 3. A 'tour' (also referred to as a 'chromosome') is a vector of length numCities containing numCities many distinct cities
 * 4. The 'fitness' of a tour is defined to be some function that varies inversely with the length of the tour. (Define this later)
 * 5. A 'population' is a collection of tours
 * 
 * 
 * We will assume that we are given a population of P (an even number) tours ("Chromosomes")
 * Step 1. Evaluate the fitness of each chromosome
 * Step 2. Select a 'mating pool' of size P based on fitness
 * Step 3. Mate pairs from the mating pool to generate a new 'generation' and return this
*/

#include "GraphTSP.hpp"

bool debug = false; //set this to true if you want debug messages
std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

std::uniform_real_distribution<float> uniReal(0.0,1.0);
//--------------------------------------------------
//OVERLOADED Prints. These should probably live in a different file.

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

//--------------------------------------------------

void mutate(vector <int> &pop){
  std::uniform_int_distribution<int> uni(0,pop.size()-2); // guaranteed unbiased
  double rndNumber = uniReal(rng);
  if(rndNumber < 0.4){
    print("mutating");
    print("initial ");
    print(pop);
    int i1 = uni(rng);
    int i2 = uni(rng);
    if(i1>i2) swap(i1, i2);
    //cout<<"the indexes are "<<i1<<" "<<i2<<endl;
    reverse( pop.begin() + i1, pop.begin() + i2 );
    print("after mut ");
    print(pop);
  }
}

void newGeneration(vector<vector<int>> &popIn, vector<vector<int>> &popOut, Graph G) {
  int popSize = popIn.size(); // must be even
  //srand(time(0));

  vector<double> fitness; // will contain fitness of ith tour
  double totalFitness = 0;
  for (auto v: popIn) {
    double k = G.tourCost(v);
    fitness.push_back(1/k);
    totalFitness += (1/k);
  }

  vector<double> probab; // probability that ith tour gets chosen
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
  for (auto pop : popOut){
    print(pop);
  }
  for(int i = 0; i < popSize - 1; i++){
    mutate(popOut[i]);
  }
  print("popout post mutation");
  for (auto pop : popOut){
    print(pop);
  }
}

// int main() {
// vector<int> p1 = {0, 5, 2, 3, 1, 4, 6};
// vector<int> p2 = {5, 1, 4, 6, 2, 0, 3};
// vector<int> p3 = {0, 1, 2, 3, 4, 5, 6};
// vector<int> p4 = {0, 2, 4, 6, 1, 3, 5};
// vector<vector<int>> pop1 = {p1, p2, p3, p4};
// vector<vector<int>> pop2 = {p1, p2, p3, p4};
// newGeneration(pop1, pop2);
// debug = true;
// for (auto k: pop2) {
// print(k);
// }
// return 0;
// }