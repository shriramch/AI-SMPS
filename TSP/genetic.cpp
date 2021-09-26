/**
 * 
 * Definitions and assumptions
 * 1. The number of cities 'numCities' is known
 * 2. A 'city' is an integer between [0, numCities-1]
 * 3. A 'tour' (also referred to as a 'chromosome') is a vector of length numCities containing numCities many distinct cities
 * 4. The 'fitness' of a tour is defined to be some function that varies inversly with the length of the tour. (Define this later)
 * 5. A 'population' is a collection of tours
 * 
 * 
 * We will assume that we are given a population of P (an even number) tours ("Chromosomes")
 * Step 1. Evaluate the fitness of each chromosome
 * Step 2. Select a 'mating pool' of size P based on fitnesses
 * Step 3. Mate pairs from the mating pool to generate a new 'generation' and return this
*/

#include "Graph.hpp"

bool debug = false; //set this to true if you want debug messages


//--------------------------------------------------
//OVERLOADED Prints. These should probably live in a different file.

void print(vector <int> tour){
    if(!debug) return;
    for (auto e : tour){
        cout<<e<<" ";
    }
    cout<< endl;
}

void print(vector <float> tour){
    if(!debug) return;
    for (auto e : tour){
        cout<<e<<" ";
    }
    cout<< endl;
}

void print(string k){
    if(!debug) return;
    cout<<k;
}

//--------------------------------------------------

float calcFitness(vector<int> &tour){
    //this function needs to be written
    return tour[0]+1;
}

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

void newGeneration(vector< vector <int> >&popIn, vector< vector <int> >&popOut){
    int popSize = popIn.size(); //must be even
    srand(time(0));

    vector <float> fitness; //will contain fitness of ith tour
    float totalFitness = 0;
    for (auto v: popIn){
        float k = calcFitness(v); //calcFitness needs to be written
        fitness.push_back(k);
        totalFitness += k;
    }
    
    vector <float> probab; //probab that ith tour gets chosen 
    for (auto f : fitness){
        probab.push_back(f/totalFitness);
    }

    vector <int> matingPool;
    double rndNumber;
    double offset;
    print("probab is ");
    print(probab);
    for (int i = 0; i<popSize; i++){
        rndNumber = rand() / (double) RAND_MAX;
        offset = 0.0;
        for(int j = 0; j<popSize; j++){
            offset+=probab[j];
            if(rndNumber < offset){
                matingPool.push_back(j);
                break;
            }
        }
        print("mating pool is ");
        print(matingPool);
    }
    
    for(int i = 0; i<popSize - 1; i+=2){
        cycle_crossover(popIn[matingPool[i]], popIn[matingPool[i+1]], popOut[i], popOut[i+1]);
    }
}

int main() {
    vector<int> p1 = {0, 5, 2, 3, 1, 4, 6};
    vector<int> p2 = {5, 1, 4, 6, 2, 0, 3};
    vector<int> p3 = {0, 1, 2, 3, 4, 5, 6};
    vector<int> p4 = {0, 2, 4, 6, 1, 3, 5};
    vector< vector <int> > pop1 = {p1, p2, p3, p4};
    vector< vector <int> > pop2 = {p1, p2, p3, p4};
    newGeneration(pop1, pop2);
    debug = true;
    for(auto k : pop2){
        print(k);
    }
    return 0;
}