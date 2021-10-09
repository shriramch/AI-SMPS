#include "GraphTSP.hpp"
#include <thread>

int main() {
  Graph G = input();
  thread genetic_thread(runGenetic, G);
  thread random_thread(random_tour, G);
  genetic_thread.join();
  random_thread.join();
  return 0;
}