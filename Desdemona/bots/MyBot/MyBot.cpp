/*
* @file botTemplate.cpp
* @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
* @date 2010-02-04
* Template for users to create their own bots
*/

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

#define INF 1e18
#define ld long double
using namespace std;
using namespace Desdemona;
#define TL 2
#define F 0.95

ld min(ld &a, ld &b) {
    if (a < b) return a; else return b;
}

ld max(ld &a, ld &b) {
    if (a > b) return a; else return b;
}

clock_t start, finish;
short int max_depth = 0;

class MyBot: public OthelloPlayer
{
    public:
        /**
         * Initialisation routines here
         * This could do anything from open up a cache of "best moves" to
         * spawning a background processing thread. 
         */
        MyBot( Turn turn );

        /**
         * Play something 
         */
        virtual Move play( const OthelloBoard& board );
    private:
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
}

int num_valid_moves(Turn turn, OthelloBoard board){
    return board.getValidMoves(turn).size();
}

ld eval(Move move, OthelloBoard board, Turn turn){
    //assumes that move is a valid move for the given turn
    OthelloBoard temp = board;
    temp.makeMove(turn, move); //temp is the board after the move was made
    ld m;
    int my_mobility = num_valid_moves(turn, temp);
    int opp_mobility = num_valid_moves(other(turn), temp);
	if(my_mobility > opp_mobility)
		m = (100.0 * my_mobility)/(my_mobility + opp_mobility);
	else if(my_mobility < opp_mobility)
		m = -(100.0 * opp_mobility)/(my_mobility + opp_mobility);
	else m = 0;
    return m;
}

ld alpha_beta(OthelloBoard board, short int depth, Move move, Turn turn, ld alpha,
              ld beta) {
  if ((ld)(clock() - start) / CLOCKS_PER_SEC > F * TL) {
      if (depth & 1) {
          return -INF;
      } else {
          return INF;
      }
  }
  while (true) {
    if (depth == max_depth) {
      return eval(move, board, turn);
    } else {
      board.makeMove(turn, move);
      turn = other(turn);
      list<Move> moves = board.getValidMoves(turn);
      if (moves.empty()) {
          if (depth & 1) {
              return -INF;
          } else {
              return INF;
          }
      }
      if (depth & 1) {
        for (auto nextMove : moves) {
          beta =
              min(beta, alpha_beta(board, depth, nextMove, turn, alpha, beta));
          if (alpha >= beta) {
            return alpha;
          }
        }
        return beta;
      } else {
        for (auto nextMove : moves) {
          alpha =
              max(alpha, alpha_beta(board, depth, nextMove, turn, alpha, beta));
          if (alpha >= beta) {
            return beta;
          }
        }
        return alpha;
      }
    }
  }
}

Move MyBot::play( const OthelloBoard& board )
{
    start = clock();
    list<Move> moves_list = board.getValidMoves( turn );
    vector<Move> moves(moves_list.begin(), moves_list.end());
    auto it = moves.begin();
    max_depth = 2;
    Move best_of_best = *it;
    ld best_val = -INF;
    vector<ld> scores;
    vector<int> indices(moves.size());
    iota(indices.begin(), indices.end(), 0);
    while(true) {
        Move best_move = *it;
        ld best = -INF;
        ld alpha = -INF, beta = +INF;
        bool flag = 0;
        for (auto move: moves) {
            if ((ld)(clock() - start) / CLOCKS_PER_SEC > F * TL) {
                flag = 1;
                break;
            }
            ld eval = alpha_beta(board, 0, move, turn, alpha, beta);
            if (eval > best) {
                eval = best;
                best_move = move;
            }
            scores.push_back(eval);
        }
        if (best > best_val) {
            best_of_best = best_move;
            best_val = best;
        }
        max_depth += 1;
        if (flag) {
            break;
        }
        sort(indices.begin(), indices.end(), [&](int A, int B) -> bool {
            return scores[A] < scores[B];
        });
        vector<Move> moves_set;
        for(int i: indices) {
            moves_set.push_back(moves[i]);
        }
        moves = moves_set;
    }
    return best_of_best;
}

// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
    OthelloPlayer* createBot( Turn turn )
    {
        return new MyBot( turn );
    }

    void destroyBot( OthelloPlayer* bot )
    {
        delete bot;
    }
}


