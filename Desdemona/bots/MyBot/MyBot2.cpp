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
#define TLF 0.95
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

  ld eval(OthelloBoard, Turn);
  ld alpha_beta(OthelloBoard, short int, Move, Turn, ld, ld);
  ld minimax(OthelloBoard, int, ld, ld, Turn, Move);
  ld search(OthelloBoard, int, ld, ld, Turn);
  ld iterativeDeepeningSearch(OthelloBoard, Turn);

  MyBot(Turn turn);
  virtual Move play(const OthelloBoard &board);

private:
  bool searchCutoff;
  vector<int> X1;
  vector<int> Y1;
  vector<vector<int>> cell_V;
  clock_t start;
  int max_depth = 0;
  int P = 18, D = 14;
  ld C = 697, L = 668, M = 59, F = 76;
  ld TL;
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

ld MyBot::eval(OthelloBoard board, Turn turn) {
  OthelloBoard temp = board;

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

/*

private AIGameMove chooseMove(AIGameState state) {
                long startTime = System.currentTimeMillis();
                boolean aiMove = state.aiMove();
                int maxScore = Integer.MIN_VALUE;
                AIGameMove bestMove = null;

                ArrayList<AIGameMove> moves = state.validMoves();

                for (AIGameMove move : moves) {
                        //
                        // Copy the current game state
                        //
                        AIGameState newState = state.clone();

                        newState.makeMove(move);

                        //
                        // Compute how long to spend looking at each move
                        //
                        long searchTimeLimit = ((TIME_LIMIT_MILLIS - 1000) /
(moves.size()));

                        int score = iterativeDeepeningSearch(newState,
searchTimeLimit);

                        //
                        // If the search finds a winning move
                        //
                        if (score >= winCutoff) {
                                return move;
                        }

                        if (score > maxScore) {
                                maxScore = score;
                                bestMove = move;
                        }
                }

                return bestMove;
        }

//
        // Run an iterative deepening search on a game state, taking no longrer
than the given time limit
        //
        private int iterativeDeepeningSearch(AIGameState state, long timeLimit)
{ long startTime = System.currentTimeMillis(); long endTime = startTime +
timeLimit; int depth = 1; int score = 0; searchCutoff = false;

                while (true) {
                        long currentTime = System.currentTimeMillis();

                        if (currentTime >= endTime) {
                                break;
                        }

                        int searchResult = search(state, depth,
Integer.MIN_VALUE, Integer.MAX_VALUE, currentTime, endTime - currentTime);

                        //
                        // If the search finds a winning move, stop searching
                        //
                        if (searchResult >= winCutoff) {
                                return searchResult;
                        }

                        if (!searchCutoff) {
                                score = searchResult;
                        }

                        depth++;
                }

                return score;
        }

        //
        // search() will perform minimax search with alpha-beta pruning on a
game state, and will cut off if the given time
        // limit has elapsed since the beginning of the search
        //
        private int search(AIGameState state, int depth, int alpha, int beta,
long startTime, long timeLimit) { ArrayList<AIGameMove> moves =
state.validMoves(); boolean myMove = state.aiMove(); int savedScore = (myMove) ?
Integer.MIN_VALUE : Integer.MAX_VALUE; int score = Evaluator.eval(state); long
currentTime = System.currentTimeMillis(); long elapsedTime = (currentTime -
startTime);

                if (elapsedTime >= timeLimit) {
                        searchCutoff = true;
                }

                //
                // If this is a terminal node or a win for either player, abort
the search
                //
                if (searchCutoff || (depth == 0) || (moves.size() == 0) ||
(score >= winCutoff) || (score <= -winCutoff)) { return score;
                }

                if (state.aiMove) {
                        for (AIGameMove move : moves) {
                                AIGameState childState = state.clone();
                                childState.makeMove(move);

                                alpha = Math.max(alpha, search(childState, depth
- 1, alpha, beta, startTime, timeLimit));

                                if (beta <= alpha) {
                                        break;
                                }
                        }

                        return alpha;
                } else {
                        for (AIGameMove move : moves) {
                                AIGameState childState = state.clone();
                                childState.makeMove(move);

                                beta = Math.min(beta, search(childState, depth -
1, alpha, beta, startTime, timeLimit));

                                if (beta <= alpha) {
                                        break;
                                }
                        }

                        return beta;
                }
        }

*/

Move MyBot::play(const OthelloBoard &board) {
  /*start = clock();
  ld maxScore = -INF;
  Move bestMove = Move(-1, -1);

  list<Move> moveList = board.getValidMoves(turn);
  vector<MoveSet> moves;
  for (Move move : moveList) {
    moves.push_back(MoveSet(move, -INF));
  }

  while (1) {
    max_depth = 1;
    Move bestMoveNow = Move(-1, -1);
    ld maxScoreNow = -INF;
    for (MoveSet moveSet : moves) {
      OthelloBoard newBoard = board;
      newBoard.makeMove(turn, moveSet.move);
      ld score = iterativeDeepeningSearch(newBoard, other(turn));
      if (searchCutoff) {
        break;
      }
      if (score > maxScore) {
        maxScoreNow = score;
        bestMoveNow = moveSet.move;
      }
      moveSet.score = score;
    }
    if (maxScore < maxScoreNow) {
      maxScore = maxScoreNow;
      bestMove = bestMoveNow;
    }
    if (searchCutoff) {
      break;
    }
    sort(moves.begin(), moves.end(), [&](MoveSet move0, MoveSet move1) {
      return move0.score > move1.score;
    });
  }
  return bestMove;*/

  start = clock();
  ld maxScore = -INF;
  Move bestMove = Move(-1, -1);
  list<Move> moves = board.getValidMoves(turn);
  cout << "moves" << endl;
  for (auto move: moves) {
    cout << move.x << " " << move.y << endl;
  }
  cout << "end moves" << endl;
  for (Move move : moves) {
    OthelloBoard newBoard = board;
    newBoard.makeMove(turn, move);
    cout << "MOVE " << move.x << " " << move.y << " " << max_depth << endl;
    TL = TLF / moves.size();
    ld score = iterativeDeepeningSearch(newBoard, other(turn));
    if (score > maxScore) {
      maxScore = score;
      bestMove = move;
    }
  }
  return bestMove;
}

ld MyBot::iterativeDeepeningSearch(OthelloBoard board, Turn turn) {
  max_depth = 0;
  ld score = 0;
  searchCutoff = false;
  while (true) {
    if ((ld)(clock() - start) / CLOCKS_PER_SEC >= TL) {
      break;
    }
    cout << "lel " << max_depth << endl;
    ld searchResult = search(board, max_depth, -INF, INF, turn);
    if (!searchCutoff) {
      score = searchResult;
    }
    ++max_depth;
  }
  return score;
}

ld MyBot::search(OthelloBoard board, int depth, ld alpha, ld beta, Turn turn) {
  list<Move> moves = board.getValidMoves(turn);
  cout << "moves" << endl;
  for (auto move: moves) {
    cout << move.x << " " << move.y << endl;
  }
  cout << "end moves" << endl;

  if ((ld)(clock() - start) / CLOCKS_PER_SEC >= TL) {
    searchCutoff = true;
  }

  if (searchCutoff || (depth == 0) || (moves.size() == 0)) {
    return eval(board, turn);
  }

  if (!((max_depth - depth) & 1)) {
    //cout << "depth = " << depth << " min " << endl;
    for (Move move : moves) {
      board.makeMove(turn, move);
      cout << "MOVE " << move.x << " " << move.y << " " << depth << endl;
      alpha = max(alpha, search(board, depth - 1, alpha, beta, other(turn)));
      if (beta <= alpha) {
        break;
      }
    }
    //cout << "final value " << alpha << endl;
    return alpha;
  } else {
    //cout << "depth = " << depth << " max " << endl;
    for (Move move : moves) {
      cout << "Starting move " << move.x << move.y << endl;
      board.makeMove(turn, move);
      cout << "MOVE " << move.x << " " << move.y << " " << depth << endl;
      cout << "made move" << endl;
      beta = min(beta, search(board, depth - 1, alpha, beta, other(turn)));
      cout << "new beta " << beta << endl;
      if (beta <= alpha) {
        //cout << "exit" << endl;
        break;
      }
      //cout << "next" << endl;
    }
    //cout << "final value " << beta << endl;
    return beta;
  }
}

// The following lines are _very_ important to create a bot module for
// Desdemona

extern "C" {
OthelloPlayer *createBot(Turn turn) { return new MyBot(turn); }

void destroyBot(OthelloPlayer *bot) { delete bot; }
}
