/*
 * @file botTemplate.cpp
 * @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
 * @date 2010-02-04
 * Template for users to create their own bots
 */

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <vector>

#define INF 1e18
#define ld long double
using namespace std;
using namespace Desdemona;
#define TL 2
#define FRAC 0.975

inline ld min(ld &a, ld &b) {
  if (a < b)
    return a;
  else
    return b;
}

inline ld max(ld &a, ld &b) {
  if (a > b)
    return a;
  else
    return b;
}

class MoveSet {
public:
  Move move;
  ld score;

  MoveSet(Move m, ld s) : move(m), score(s) {}
};

class MyBot : public OthelloPlayer {
public:
  /**
   * Initialisation routines here
   * This could do anything from open up a cache of "best moves" to
   * spawning a background processing thread.
   */

  ld eval(Move, OthelloBoard, Turn);
  ld alpha_beta(OthelloBoard, short int, Move, Turn, ld, ld);

  MyBot(Turn turn);
  virtual Move play(const OthelloBoard &board);

private:
  vector<int> X1;
  vector<int> Y1;
  vector<vector<int>> cell_V;
  clock_t start;
  short int max_depth = 0;
  int P = 18, D = 14;
  ld C = 697, L = 668, M = 59, F = 76;
};

MyBot::MyBot(Turn turn) : OthelloPlayer(turn) {
  max_depth = 0;
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
}

inline int num_valid_moves(Turn turn, OthelloBoard &board) {
  return board.getValidMoves(turn).size();
}

ld MyBot::eval(Move move, OthelloBoard board, Turn turn) {
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

  return (P * p) + (C * c) + (L * l) + (M * m) + (F * f) + (D * d);
}

ld MyBot::alpha_beta(OthelloBoard board, short int depth, Move move, Turn turn,
                     ld alpha, ld beta) {
  if ((ld)(clock() - start) / CLOCKS_PER_SEC >= FRAC * TL) {
    if (depth & 1) {
      return -INF;
    } else {
      return INF;
    }
  }
  if (depth == max_depth) {
    return eval(move, board, turn);
  } else {
    board.makeMove(turn, move);
    turn = other(turn);
    list<Move> moves = board.getValidMoves(turn);
    if (moves.empty()) {
      if (depth & 1) {
        return INF;
      } else {
        return -INF;
      }
    }
    for (auto nextMove : moves) {
      if (depth & 1) {
        alpha = min(alpha,
                    alpha_beta(board, depth + 1, nextMove, turn, alpha, beta));
        if (alpha <= beta) {
          return alpha;
        }
      } else {
        beta = max(beta,
                   alpha_beta(board, depth + 1, nextMove, turn, alpha, beta));
        if (alpha <= beta) {
          return beta;
        }
      }
    }
    if (depth & 1) {
      return alpha;
    } else {
      return beta;
    }
  }
}

Move MyBot::play(const OthelloBoard &board) {
  start = clock();
  list<Move> moves_list = board.getValidMoves(turn);
  vector<MoveSet> moves;
  for (Move move : moves_list) {
    moves.push_back(MoveSet(move, -INF));
  }
  MoveSet dbest_moveSet = moves[0];
  // Perform alpha-beta search with iterative deepening
  bool quit_flag = false;
  for (int i = 2; !quit_flag; i++) {
    MoveSet best_moveSet = moves[0];
    max_depth = i;
    for (MoveSet moveSet : moves) {
      if ((ld)(clock() - start) / CLOCKS_PER_SEC >= FRAC * TL) {
        quit_flag = true;
        break;
      }
      ld score = alpha_beta(board, 1, moveSet.move, turn, -INF, INF);
      if (score > best_moveSet.score) {
        best_moveSet = moveSet;
      }
    }
    if (dbest_moveSet.score < best_moveSet.score) {
      dbest_moveSet = best_moveSet;
    }
    sort(moves.begin(), moves.end(),
         [](const MoveSet &a, const MoveSet &b) { return a.score > b.score; });
  }
  return dbest_moveSet.move;
}

// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
OthelloPlayer *createBot(Turn turn) { return new MyBot(turn); }

void destroyBot(OthelloPlayer *bot) { delete bot; }
}
