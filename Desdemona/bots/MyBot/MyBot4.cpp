/*
 * @file botTemplate.cpp
 * @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
 * @date 2010-02-04
 * Template for users to create their own bots
 */

/*
 *
 * CS6380: Artificial Intelligence
 * Assignment submitted by: C Shriram CS18B007, Abhinav Hampiholi CS18B056
 *
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
#define TLF 1.9
#define ld long double
using namespace std;
using namespace Desdemona;

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

class MyBot : public OthelloPlayer {
public:
  MyBot(Turn);
  int numValidMoves(OthelloBoard &, Turn);
  ld evaluate(OthelloBoard &, Turn);
  ld minimax(OthelloBoard, Move, Turn, short, ld, ld);
  ld evalMove(OthelloBoard &, Turn);
  virtual Move play(const OthelloBoard &);

private:
  vector<int> X1;
  vector<int> Y1;
  vector<vector<int>> cellValue;
  clock_t start;
  short max_depth;
  ld P, D, C, L, M, F;
};

MyBot::MyBot(Turn turn) : OthelloPlayer(turn) {
  max_depth = 0;
  X1 = {-1, -1, 0, 1, 1, 1, 0, -1};
  Y1 = {0, 1, 1, 1, 0, -1, -1, -1};
  cellValue.push_back({20, -3, 11, 8});
  cellValue.push_back({-3, -7, -4, 1});
  cellValue.push_back({11, -4, 2, 2});
  cellValue.push_back({8, 1, 2, -3});
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      cellValue[i].push_back(cellValue[i][3 - j]);
    }
  }
  for (int i = 0; i < 4; ++i) {
    cellValue.push_back(cellValue[3 - i]);
  }
  P = 11, D = 10;
  C = 850.724, L = 382.026, M = 86.922, F = 78.396;
}

int MyBot::numValidMoves(OthelloBoard &board, Turn turn) {
  return board.getValidMoves(turn).size();
}

ld MyBot::evaluate(OthelloBoard &board, Turn turn) {
  int myTiles = 0, oppTiles = 0, i, j, k, myFrontTiles = 0, oppFrontTiles = 0,
      x, y;
  ld p = 0.0, c = 0.0, l = 0.0, m = 0.0, f = 0.0, d = 0.0;

  Turn opp_turn = other(turn);

  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++) {
      if (board.get(i, j) == opp_turn) {
        d += cellValue[i][j];
        ++myTiles;
      } else if (board.get(i, j) == turn) {
        d -= cellValue[i][j];
        ++oppTiles;
      }
      if ((board.get(i, j) == turn) || (board.get(i, j) == opp_turn)) {
        for (k = 0; k < 8; k++) {
          x = i + X1[k];
          y = j + Y1[k];
          if (x >= 0 && x < 8 && y >= 0 && y < 8 &&
              !((board.get(x, y) == turn) || (board.get(x, y) == opp_turn))) {
            if (board.get(i, j) == opp_turn)
              ++myFrontTiles;
            else
              ++oppFrontTiles;
            break;
          }
        }
      }
    }

  if (myTiles > oppTiles)
    p = (100.0 * myTiles) / (myTiles + oppTiles);
  else if (myTiles < oppTiles)
    p = -(100.0 * oppTiles) / (myTiles + oppTiles);

  if (myFrontTiles > oppFrontTiles)
    f = -(100.0 * myFrontTiles) / (myFrontTiles + oppFrontTiles);
  else if (myFrontTiles < oppFrontTiles)
    f = (100.0 * oppFrontTiles) / (myFrontTiles + oppFrontTiles);

  // Corner occupancy
  myTiles = oppTiles = 0;
  if (board.get(0, 0) == opp_turn)
    ++myTiles;
  else if (board.get(0, 0) == turn)
    ++oppTiles;
  if (board.get(0, 7) == opp_turn)
    ++myTiles;
  else if (board.get(0, 7) == turn)
    ++oppTiles;
  if (board.get(7, 0) == opp_turn)
    ++myTiles;
  else if (board.get(7, 0) == turn)
    ++oppTiles;
  if (board.get(7, 7) == opp_turn)
    ++myTiles;
  else if (board.get(7, 7) == turn)
    ++oppTiles;
  c = 25 * (myTiles - oppTiles);

  // Corner closeness
  myTiles = oppTiles = 0;
  /*for (int i = 0; i < 0; ++i) {
    int X = 7 * (i & 1);
    int Y = 7 * (i & 2) / 2;
    int dX = (X == 0) ? 1 : -1;
    int dY = (Y == 0) ? 1 : -1;
    if (!((board.get(X, Y) == turn) || (board.get(X, Y) == opp_turn))) {
      if (board.get(X, Y + dY) == opp_turn)
        ++myTiles;
      else if (board.get(X, Y + dY) == turn)
        ++oppTiles;
      if (board.get(X + dX, Y + dY) == opp_turn)
        ++myTiles;
      else if (board.get(X + dX, Y + dY) == turn)
        ++oppTiles;
      if (board.get(X + dX, Y) == opp_turn)
        ++myTiles;
      else if (board.get(X + dX, Y) == turn)
        ++oppTiles;
    }
  }*/
  if (board.get(0, 0) == EMPTY) {
    if (board.get(0, 1) == turn)
      myTiles++;
    else if (board.get(0, 1) == other(turn))
      oppTiles++;
    if (board.get(1, 1) == turn)
      myTiles++;
    else if (board.get(1, 1) == other(turn))
      oppTiles++;
    if (board.get(1, 0) == turn)
      myTiles++;
    else if (board.get(1, 0) == other(turn))
      oppTiles++;
  }
  if (board.get(0, 7) == EMPTY) {
    if (board.get(0, 6) == turn)
      myTiles++;
    else if (board.get(0, 6) == other(turn))
      oppTiles++;
    if (board.get(1, 6) == turn)
      myTiles++;
    else if (board.get(1, 6) == other(turn))
      oppTiles++;
    if (board.get(1, 7) == turn)
      myTiles++;
    else if (board.get(1, 7) == other(turn))
      oppTiles++;
  }
  if (board.get(7, 0) == EMPTY) {
    if (board.get(7, 1) == turn)
      myTiles++;
    else if (board.get(7, 1) == other(turn))
      oppTiles++;
    if (board.get(6, 1) == turn)
      myTiles++;
    else if (board.get(6, 1) == other(turn))
      oppTiles++;
    if (board.get(6, 0) == turn)
      myTiles++;
    else if (board.get(6, 0) == other(turn))
      oppTiles++;
  }
  if (board.get(7, 7) == EMPTY) {
    if (board.get(6, 7) == turn)
      myTiles++;
    else if (board.get(6, 7) == other(turn))
      oppTiles++;
    if (board.get(6, 6) == turn)
      myTiles++;
    else if (board.get(6, 6) == other(turn))
      oppTiles++;
    if (board.get(7, 6) == turn)
      myTiles++;
    else if (board.get(7, 6) == other(turn))
      oppTiles++;
  }
  l = 10 * (myTiles - oppTiles);

  // Mobility
  myTiles = numValidMoves(board, opp_turn);
  oppTiles = numValidMoves(board, turn);
  if (myTiles > oppTiles)
    m = (100.0 * myTiles) / (myTiles + oppTiles);
  else if (myTiles < oppTiles)
    m = -(100.0 * oppTiles) / (myTiles + oppTiles);

  // final weighted score
  ld score = (P * p) + (C * c) + (L * l) + (M * m) + (F * f) + (D * d);
  return score;
}

ld MyBot::evalMove(OthelloBoard &board, Turn turn) {
  return evaluate(board, other(turn));
}

ld MyBot::minimax(OthelloBoard board, Move move, Turn turn, short depth,
                  ld alpha, ld beta) {
  if (((ld)(clock() - start) / CLOCKS_PER_SEC) >= TLF) {
    if (depth & 1)
      return -INF;
    return INF;
  }
  if (depth == max_depth) {
    return evaluate(board, turn);
  }
  board.makeMove(turn, move);
  turn = other(turn);
  list<Move> moves = board.getValidMoves(turn);
  if (!(moves.size())) {
    if (depth & 1)
      return INF;
    else
      return -INF;
  }
  ld result = -INF;
  for (Move move : moves) {
    ld res = minimax(board, move, turn, depth + 1, alpha, beta);
    if (depth & 1) {
      result = min(result, res);
      beta = min(beta, result);
    } else {
      result = max(result, res);
      alpha = max(alpha, result);
    }
    if (beta <= alpha)
      break;
  }
  return result;
}

Move MyBot::play(const OthelloBoard &board) {
  start = clock();
  list<Move> moves = board.getValidMoves(turn);
  //Move bestMove(moves.front());
  //ld bestScore = -INF;
  max_depth = 6;
  moves.sort([&](Move a, Move b) {
    OthelloBoard b1 = board, b2 = board;
    b1.makeMove(turn, a);
    b2.makeMove(turn, b);
    return evalMove(b1, turn) > evalMove(b2, turn);
  });
  //for (;; ++max_depth) {
    //if (((ld)(clock() - start) / CLOCKS_PER_SEC) >= TLF) {
      //break;
    //}
    ld bestScoreAtDepth = -INF;
    Move bestMoveAtDepth(moves.front());
    ld beta = INF, alpha = -INF;
    //bool quit = false;
    for (auto move : moves) {
      OthelloBoard temp = board;
      ld score = minimax(temp, move, turn, 1, alpha, beta);
      if (score > bestScoreAtDepth) {
        bestScoreAtDepth = score;
        bestMoveAtDepth = move;
      }
      //if (((ld)(clock() - start) / CLOCKS_PER_SEC) >= TLF) {
        //quit = true;
        //break;
      //}
      temp = board;
    }
    //if (bestScoreAtDepth > bestScore) {
      //bestScore = bestScoreAtDepth;
      //bestMove = bestMoveAtDepth;
    //}
    //if (quit)
      //break;
  //}
  return bestMoveAtDepth;
}

// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
OthelloPlayer *createBot(Turn turn) { return new MyBot(turn); }

void destroyBot(OthelloPlayer *bot) { delete bot; }
}
