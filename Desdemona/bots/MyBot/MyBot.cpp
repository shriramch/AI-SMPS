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
#define FRAC 0.975

inline ld min(ld &a, ld &b) {
    if (a < b) return a; else return b;
}

inline ld max(ld &a, ld &b) {
    if (a > b) return a; else return b;
}

vector<int> X1;
vector<int> Y1;
vector<vector<int>> cell_V;

clock_t start, finish;
short int max_depth = 0;

// 18 14 697 668 59 76
int P = 18, D = 14;
ld C = 697, L = 668, M = 59, F = 76;

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
  X1 = {-1, -1, 0, 1, 1, 1, 0, -1};
  Y1 = {0, 1, 1, 1, 0, -1, -1, -1};

  cell_V.push_back({20, -3, 11, 8});
  cell_V.push_back({-3, -7, -4, 1});
  cell_V.push_back({11, -4, 2, 2});
  cell_V.push_back({8, 1, 2, -3});

  for (int i = 0; i < 4; ++i) {

    for (int j = 0; j < 4; ++j) {
      cell_V[i].push_back(cell_V[i][3 - j]);
    }
  }
  for (int i = 0; i < 4; ++i) {
    cell_V.push_back(cell_V[3 - i]);
  }

  /*
  cout << "Enter P: ";
  cin >> P;
  cout << "Enter D: ";
  cin >> D;
  cout << "Enter C: ";
  cin >> C;
  cout << "Enter L: ";
  cin >> L;
  cout << "Enter M: ";
  cin >> M;
  cout << "Enter F: ";
  cin >> F;
  int P = 10, D = 10;
  ld C = 801.724, L = 382.026, M = 78.922, F = 74.396;
  */
}

inline int num_valid_moves(Turn turn, OthelloBoard &board){
    return board.getValidMoves(turn).size();
}


ld eval(Move move, OthelloBoard board, Turn turn) {
  // assumes that move is a valid move for the given turn
  OthelloBoard temp = board;
  temp.makeMove(turn, move); // temp is the board after the move was made

  ld d = 0.0, p = 0.0, f = 0.0;
  int my_front_tiles = 0, opp_front_tiles = 0, my_tiles = 0, opp_tiles = 0;
  int i, j, k, x, y;

  // Piece difference, frontier disks and disk squares
  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++) {
      if (temp.get(i, j) == turn) {
        d += cell_V[i][j];
        my_tiles++;
      } else if (temp.get(i, j) == other(turn)) {
        d -= cell_V[i][j];
        opp_tiles++;
      }
      if (temp.get(i, j) != '-') {
        for (k = 0; k < 8; k++) {
          x = i + X1[k];
          y = j + Y1[k];
          if (x >= 0 && x < 8 && y >= 0 && y < 8 && temp.get(x, y) == '-') {
            if (temp.get(i, j) == turn)
              my_front_tiles++;
            else
              opp_front_tiles++;
            break;
          }
        }
      }
    }
  if (my_tiles > opp_tiles)
    p = (100.0 * my_tiles) / (my_tiles + opp_tiles);
  else if (my_tiles < opp_tiles)
    p = -(100.0 * opp_tiles) / (my_tiles + opp_tiles);
  else
    p = 0;

  if (my_front_tiles > opp_front_tiles)
    f = -(100.0 * my_front_tiles) / (my_front_tiles + opp_front_tiles);
  else if (my_front_tiles < opp_front_tiles)
    f = (100.0 * opp_front_tiles) / (my_front_tiles + opp_front_tiles);
  else
    f = 0;

  // corner occupancy
  ld c;
  my_tiles = 0;
  opp_tiles = 0;
  if (temp.get(0, 0) == turn)
    my_tiles++;
  else if (temp.get(0, 0) == other(turn))
    opp_tiles++;
  if (temp.get(0, 7) == turn)
    my_tiles++;
  else if (temp.get(0, 7) == other(turn))
    opp_tiles++;
  if (temp.get(7, 0) == turn)
    my_tiles++;
  else if (temp.get(7, 0) == other(turn))
    opp_tiles++;
  if (temp.get(7, 7) == turn)
    my_tiles++;
  else if (temp.get(7, 7) == other(turn))
    opp_tiles++;
  c = 25 * (my_tiles - opp_tiles);

  // corner closeness
  ld l;
  my_tiles = opp_tiles = 0;
  if (temp.get(0, 0) == '-') {
    if (temp.get(0, 1) == turn)
      my_tiles++;
    else if (temp.get(0, 1) == other(turn))
      opp_tiles++;
    if (temp.get(1, 1) == turn)
      my_tiles++;
    else if (temp.get(1, 1) == other(turn))
      opp_tiles++;
    if (temp.get(1, 0) == turn)
      my_tiles++;
    else if (temp.get(1, 0) == other(turn))
      opp_tiles++;
  }
  if (temp.get(0, 7) == '-') {
    if (temp.get(0, 6) == turn)
      my_tiles++;
    else if (temp.get(0, 6) == other(turn))
      opp_tiles++;
    if (temp.get(1, 6) == turn)
      my_tiles++;
    else if (temp.get(1, 6) == other(turn))
      opp_tiles++;
    if (temp.get(1, 7) == turn)
      my_tiles++;
    else if (temp.get(1, 7) == other(turn))
      opp_tiles++;
  }
  if (temp.get(7, 0) == '-') {
    if (temp.get(7, 1) == turn)
      my_tiles++;
    else if (temp.get(7, 1) == other(turn))
      opp_tiles++;
    if (temp.get(6, 1) == turn)
      my_tiles++;
    else if (temp.get(6, 1) == other(turn))
      opp_tiles++;
    if (temp.get(6, 0) == turn)
      my_tiles++;
    else if (temp.get(6, 0) == other(turn))
      opp_tiles++;
  }
  if (temp.get(7, 7) == '-') {
    if (temp.get(6, 7) == turn)
      my_tiles++;
    else if (temp.get(6, 7) == other(turn))
      opp_tiles++;
    if (temp.get(6, 6) == turn)
      my_tiles++;
    else if (temp.get(6, 6) == other(turn))
      opp_tiles++;
    if (temp.get(7, 6) == turn)
      my_tiles++;
    else if (temp.get(7, 6) == other(turn))
      opp_tiles++;
  }
  l = -12.5 * (my_tiles - opp_tiles);

  // mobility criteria
  ld m;
  int my_mobility = num_valid_moves(turn, temp);
  int opp_mobility = num_valid_moves(other(turn), temp);
  if (my_mobility > opp_mobility)
    m = (100.0 * my_mobility) / (my_mobility + opp_mobility);
  else if (my_mobility < opp_mobility)
    m = -(100.0 * opp_mobility) / (my_mobility + opp_mobility);
  else
    m = 0;

  /*int fact = 0;
  Turn opposite = other(turn);
  int i = 0;
  int sum = 0;
      while(i < 8) {
      int j = 0;
              while(j < 8) {
                      if(board.get(i,j) == turn)
                              sum += cell_V[i][j];
                      else if(board.get(i,j) == opposite)
                              sum -= cell_V[i][j];
          ++j;
              }
      ++i;
      }

  int xvals[4] = {shu, shu, saat, saat};
  int yvals[4] = {shu, saat, shu, saat};

      int Is[12] = {shu,ek,ek, shu,ek,ek, che,saat,che, che,saat,che};
      int Js[12] = {ek,shu,ek, che,saat,che, shu,ek,ek, saat,che,che};

  i = 0;
      while(i < 12){
          if(board.get(xvals[i/3],yvals[i/3]) != EMPTY) {
              if(board.get(Is[i],Js[i]) == turn)
                  sum += (5 - cell_V[Is[i]][Js[i]]);

          else if(board.get(Is[i],Js[i]) == opp)
                  sum -= (5 - cell_V[Is[i]][Js[i]]);
          }
      ++i;
      }
      return sum;*/

  return (P * p) + (C * c) + (L * l) + (M * m) + (F * f) + (D * d);
}

ld alpha_beta(OthelloBoard board, short int depth, Move move, Turn turn,
              ld alpha, ld beta) {
  if ((ld)(clock() - start) / CLOCKS_PER_SEC >= FRAC * TL) {
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

Move MyBot::play(const OthelloBoard &board) {
  start = clock();
  list<Move> moves_list = board.getValidMoves(turn);
  vector<Move> moves(moves_list.begin(), moves_list.end());
  auto it = moves.begin();
  max_depth = 2;
  Move best_of_best = *it;
  ld best_val = -INF;
  vector<ld> scores;
  vector<int> indices(moves.size());
  iota(indices.begin(), indices.end(), 0);
  while (true) {
    Move best_move = *it;
    ld best = -INF;
    ld alpha = -INF, beta = +INF;
    bool flag = 0;
    for (auto move : moves) {
      if ((ld)(clock() - start) / CLOCKS_PER_SEC >= FRAC * TL) {
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
    sort(indices.begin(), indices.end(),
         [&](int A, int B) -> bool { return scores[A] < scores[B]; });
    vector<Move> moves_set;
    for (int i : indices) {
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


