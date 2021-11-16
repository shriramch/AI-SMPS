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

// ld eval(Move move, OthelloBoard board, Turn turn){
//     //assumes that move is a valid move for the given turn
//     OthelloBoard temp = board;
//     temp.makeMove(turn, move); //temp is the board after the move was made
//     ld m;
//     int my_mobility = num_valid_moves(turn, temp);
//     int opp_mobility = num_valid_moves(other(turn), temp);
// 	if(my_mobility > opp_mobility)
// 		m = (100.0 * my_mobility)/(my_mobility + opp_mobility);
// 	else if(my_mobility < opp_mobility)
// 		m = -(100.0 * opp_mobility)/(my_mobility + opp_mobility);
// 	else m = 0;
//     return m;
// }

ld eval(Move move, OthelloBoard board, Turn turn){
    //assumes that move is a valid move for the given turn
    OthelloBoard temp = board;
    temp.makeMove(turn, move); //temp is the board after the move was made
   
    // mobility criteria 
    ld m;
    int my_mobility = num_valid_moves(turn, temp);
    int opp_mobility = num_valid_moves(other(turn), temp);
	if(my_mobility > opp_mobility)
		m = (100.0 * my_mobility)/(my_mobility + opp_mobility);
	else if(my_mobility < opp_mobility)
		m = -(100.0 * opp_mobility)/(my_mobility + opp_mobility);
	else m = 0;
    
    ld d = 0.0, p = 0.0, f = 0.0;
    int my_front_tiles = 0, opp_front_tiles = 0, my_tiles = 0, opp_tiles = 0;
    int i,j,k, x, y;
    
    int X1[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int Y1[] = {0, 1, 1, 1, 0, -1, -1, -1};
	//int V[8][8];
    vector< vector <int> > V;

	V.push_back({20, -3, 11, 8, 8, 11, -3, 20});
    V.push_back({-3, -7, -4, 1, 1, -4, -7, -3});
    V.push_back({11, -4, 2, 2, 2, 2, -4, 11});
    V.push_back({8, 1, 2, -3, -3, 2, 1, 8});
    V.push_back({8, 1, 2, -3, -3, 2, 1, 8});
    V.push_back({11, -4, 2, 2, 2, 2, -4, 11});
    V.push_back({-3, -7, -4, 1, 1, -4, -7, -3});
    V.push_back({20, -3, 11, 8, 8, 11, -3, 20});

// Piece difference, frontier disks and disk squares
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)  {
			if(temp.get(i,j) == turn)  {
				d += V[i][j];
				my_tiles++;
			} else if(temp.get(i,j) == other(turn))  {
				d -= V[i][j];
				opp_tiles++;
			}
			if(temp.get(i,j) != '-')   {
				for(k=0; k<8; k++)  {
					x = i + X1[k]; y = j + Y1[k];
					if(x >= 0 && x < 8 && y >= 0 && y < 8 && temp.get(x,y) == '-') {
						if(temp.get(i,j) == turn)  my_front_tiles++;
						else opp_front_tiles++;
						break;
					}
				}
			}
		}
	if(my_tiles > opp_tiles)
		p = (100.0 * my_tiles)/(my_tiles + opp_tiles);
	else if(my_tiles < opp_tiles)
		p = -(100.0 * opp_tiles)/(my_tiles + opp_tiles);
	else p = 0;

	if(my_front_tiles > opp_front_tiles)
		f = -(100.0 * my_front_tiles)/(my_front_tiles + opp_front_tiles);
	else if(my_front_tiles < opp_front_tiles)
		f = (100.0 * opp_front_tiles)/(my_front_tiles + opp_front_tiles);
	else f = 0;



    // corner occupancy
    ld c;
    my_tiles = 0; 
    opp_tiles = 0;
	if(temp.get(0,0) == turn) my_tiles++;
	else if(temp.get(0,0) == other(turn)) opp_tiles++;
	if(temp.get(0,7) == turn) my_tiles++;
	else if(temp.get(0,7) == other(turn)) opp_tiles++;
	if(temp.get(7,0) == turn) my_tiles++;
	else if(temp.get(7,0) == other(turn)) opp_tiles++;
	if(temp.get(7,7) == turn) my_tiles++;
	else if(temp.get(7,7) == other(turn)) opp_tiles++;
	c = 25 * (my_tiles - opp_tiles);
    
    //corner closeness
    ld l;
    my_tiles = opp_tiles = 0;
	if(temp.get(0,0) == '-'){
		if(temp.get(0,1) == turn) my_tiles++;
		else if(temp.get(0,1) == other(turn)) opp_tiles++;
		if(temp.get(1,1) == turn) my_tiles++;
		else if(temp.get(1,1) == other(turn)) opp_tiles++;
		if(temp.get(1,0) == turn) my_tiles++;
		else if(temp.get(1,0) == other(turn)) opp_tiles++;
	}
	if(temp.get(0,7) == '-'){
		if(temp.get(0,6) == turn) my_tiles++;
		else if(temp.get(0,6) == other(turn)) opp_tiles++;
		if(temp.get(1,6) == turn) my_tiles++;
		else if(temp.get(1,6) == other(turn)) opp_tiles++;
		if(temp.get(1,7) == turn) my_tiles++;
		else if(temp.get(1,7) == other(turn)) opp_tiles++;
	}
	if(temp.get(7,0) == '-'){
		if(temp.get(7,1) == turn) my_tiles++;
		else if(temp.get(7,1) == other(turn)) opp_tiles++;
		if(temp.get(6,1) == turn) my_tiles++;
		else if(temp.get(6,1) == other(turn)) opp_tiles++;
		if(temp.get(6,0) == turn) my_tiles++;
		else if(temp.get(6,0) == other(turn)) opp_tiles++;
	}
	if(temp.get(7,7) == '-'){
		if(temp.get(6,7) == turn) my_tiles++;
		else if(temp.get(6,7) == other(turn)) opp_tiles++;
		if(temp.get(6,6) == turn) my_tiles++;
		else if(temp.get(6,6) == other(turn)) opp_tiles++;
		if(temp.get(7,6) == turn) my_tiles++;
		else if(temp.get(7,6) == other(turn)) opp_tiles++;
	}
	l = -12.5 * (my_tiles - opp_tiles);
    
    return (10 * p) + (801.724 * c) + (382.026 * l) + (78.922 * m) + (74.396 * f) + (10 * d);
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


