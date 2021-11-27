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
  int moveNo;
  vector<int> X1;
  vector<int> Y1;
  vector<vector<vector<ld>>> cellValue;
  clock_t start;
  short max_depth;
  ld P, D[3], C, L, M, F;
  short phase;
};

MyBot::MyBot(Turn turn) : OthelloPlayer(turn) {
  max_depth = 0;
  if (turn == BLACK)
    moveNo = 0;
  else
    moveNo = 1;
  X1 = {-1, -1, 0, 1, 1, 1, 0, -1};
  Y1 = {0, 1, 1, 1, 0, -1, -1, -1};

  cellValue = {
      {{0, 0, 0, 0, 0, 0, 0, 0},
       {0, -0.02231, 0.05583, 0.02004, 0.02004, 0.5583, -0.02231, 0},
       {0, 0.05583, 0.10126, -0.10927, -0.10927, 0.10126, 0.05583, 0},
       {0, 0.02004, -0.10927, -0.10155, -0.10155, -0.10927, 0.02004, 0},
       {0, 0.02004, -0.10927, -0.10155, -0.10155, -0.10927, 0.02004, 0},
       {0, 0.05583, 0.10126, -0.10927, -0.10927, 0.10126, 0.05583, 0},
       {0, -0.02231, 0.05583, 0.02004, 0.02004, 0.5583, -0.02231, 0},
       {0, 0, 0, 0, 0, 0, 0, 0}},
      {
          {6.32711, -3.32813, 0.33907, -2.00512, -2.00512, 0.33907, -3.32813,
           6.32711},
          {-3.32813, -1.52928, -1.87550, -0.18176, -0.18176, -1.87550, -1.52928,
           -3.32813},
          {0.33907, -1.87550, 1.06939, 0.62415, 0.62415, 1.06939, -1.87550,
           0.33907},
          {-2.00512, -0.18176, 0.62415, 0.10539, 0.10539, 0.62415, -0.18176,
           -2.00512},
          {-2.00512, -0.18176, 0.62415, 0.10539, 0.10539, 0.62415, -0.18176,
           -2.00512},
          {0.33907, -1.87550, 1.06939, 0.62415, 0.62415, 1.06939, -1.87550,
           0.33907},
          {-3.32813, -1.52928, -1.87550, -0.18176, -0.18176, -1.87550, -1.52928,
           -3.32813},
          {6.32711, -3.32813, 0.33907, -2.00512, -2.00512, 0.33907, -3.32813,
           6.32711},
      },
      {
          {5.50062, -0.17812, -2.58948, -0.59007, -0.59007, -2.58948, -0.17812,
           5.50062},
          {-0.17812, 0.96804, -2.16084, -2.01723, -2.01723, -2.16084, 0.96804,
           -0.17812},
          {-2.58948, -2.16084, 0.94062, -1.07055, -1.07055, 0.94062, -2.16084,
           -2.58948},
          {-0.59007, -2.01723, -1.07055, 0.73486, 0.73486, -1.07055, -2.01723,
           -0.59007},
          {-0.59007, -2.01723, -1.07055, 0.73486, 0.73486, -1.07055, -2.01723,
           -0.59007},
          {-2.58948, -2.16084, 0.94062, -1.07055, -1.07055, 0.94062, -2.16084,
           -2.58948},
          {-0.17812, 0.96804, -2.16084, -2.01723, -2.01723, -2.16084, 0.96804,
           -0.17812},
          {5.50062, -0.17812, -2.58948, -0.59007, -0.59007, -2.58948, -0.17812,
           5.50062},
      }};
  P = 7, D[0] = 14, D[1] = 13, D[2] = 15;
  C = 533, L = 540, M = 57, F = 96;
}

int MyBot::numValidMoves(OthelloBoard &board, Turn turn) {
  return board.getValidMoves(turn).size();
}

ld MyBot::evaluate(OthelloBoard &board, Turn turn) {
  Turn opp_turn = other(turn);

  int myTiles = 0, oppTiles = 0, myFrontTiles = 0, oppFrontTiles = 0;

  ld d = 0.0;
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      if (board.get(i, j) == opp_turn) {
        d += cellValue[phase][i][j];
        ++myTiles;
      } else if (board.get(i, j) == turn) {
        d -= cellValue[phase][i][j];
        ++oppTiles;
      }
      if ((board.get(i, j) == turn) || (board.get(i, j) == opp_turn)) {
        for (int k = 0; k < 8; k++) {
          int x = i + X1[k];
          int y = j + Y1[k];
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

  ld p = 0.0;

  if (myTiles > oppTiles)
    p = (100.0 * myTiles) / (myTiles + oppTiles);
  else if (myTiles < oppTiles)
    p = -(100.0 * oppTiles) / (myTiles + oppTiles);

  if (moveNo > 54) {
    return p;
  }

  ld f = 0.0;

  if (myFrontTiles > oppFrontTiles)
    f = -(100.0 * myFrontTiles) / (myFrontTiles + oppFrontTiles);
  else if (myFrontTiles < oppFrontTiles)
    f = (100.0 * oppFrontTiles) / (myFrontTiles + oppFrontTiles);

  myTiles = 0, oppTiles = 0;
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
  ld c = 25 * (myTiles - oppTiles);

  myTiles = oppTiles = 0;
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
  ld l = 10 * (myTiles - oppTiles);

  myTiles = numValidMoves(board, opp_turn);
  oppTiles = numValidMoves(board, turn);
  ld m = 0.0;
  if (myTiles > oppTiles)
    m = (100.0 * myTiles) / (myTiles + oppTiles);
  else if (myTiles < oppTiles)
    m = -(100.0 * oppTiles) / (myTiles + oppTiles);

  ld score = (P * p) + (C * c) + (L * l) + (M * m) + (F * f) + (D[phase] * d);
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
  ld result = (depth & 1) ? INF : -INF;
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
  max_depth = 2;
  moves.sort([&](Move a, Move b) {
    OthelloBoard b1 = board, b2 = board;
    b1.makeMove(turn, a);
    b2.makeMove(turn, b);
    return evalMove(b1, turn) > evalMove(b2, turn);
  });
  max_depth = (moveNo < 54) ? 6 : 20;
  if (phase == 0) {
    int res = board.get(0, 0) != EMPTY
                  ? 1
                  : 0 + board.get(0, 7) != EMPTY
                        ? 1
                        : 0 + board.get(7, 0) != EMPTY
                              ? 1
                              : 0 + board.get(7, 7) != EMPTY ? 1 : 0;
    if (res > 0) {
      phase = 1;
    }
  } else if (phase == 1) {
    int res = board.get(0, 0) == BLACK
                  ? 1
                  : 0 + board.get(0, 7) == BLACK
                        ? 1
                        : 0 + board.get(7, 0) == BLACK
                              ? 1
                              : 0 + board.get(7, 7) == BLACK ? 1 : 0;
    if (res > 1) {
      phase = 2;
    }
    res = board.get(0, 0) == RED
              ? 1
              : 0 + board.get(0, 7) == RED
                    ? 1
                    : 0 + board.get(7, 0) == RED
                          ? 1
                          : 0 + board.get(7, 7) == RED ? 1 : 0;
    if (res > 1) {
      phase = 2;
    }
  }
  ld bestScoreAtDepth = -INF;
  Move bestMoveAtDepth(moves.front());
  ld beta = INF, alpha = -INF;
  for (auto move : moves) {
    OthelloBoard temp = board;
    ld score = minimax(temp, move, turn, 1, alpha, beta);
    if (score > bestScoreAtDepth) {
      bestScoreAtDepth = score;
      bestMoveAtDepth = move;
    }
    if (((ld)(clock() - start) / CLOCKS_PER_SEC) >= TLF) {
      break;
    }
    temp = board;
  }
  moveNo += 2;
  return bestMoveAtDepth;
}

// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
OthelloPlayer *createBot(Turn turn) { return new MyBot(turn); }

void destroyBot(OthelloPlayer *bot) { delete bot; }
}
