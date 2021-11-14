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
#define MAX_DEPTH 6
#define INF 1e18
#define ld long double
using namespace std;
using namespace Desdemona;

ld min(ld &a, ld &b) {
    if (a < b) return a; else return b;
}

ld max(ld &a, ld &b) {
    if (a > b) return a; else return b;
}

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

ld alpha_beta(OthelloBoard board, int depth, Move move, Turn turn, ld alpha, ld beta) {
    if (depth == MAX_DEPTH) {
        return eval(move);
    } else {
        board.makeMove(turn, move);
        turn = other(turn);
        list<Move> moves = board.getValidMoves(turn);
        ld res = -INF;
        if (depth & 1) {
            for (auto nextMove: moves) {
                beta = min(beta, alpha_beta(board, depth, nextMove, turn, alpha, beta));
                if (alpha >= beta) {
                    return alpha;
                }
            }
            return beta;
        } else {
            for (auto nextMove: moves) {
                alpha = max(alpha, alpha_beta(board, depth, nextMove, turn, alpha, beta));
                if (alpha >= beta) {
                    return beta;
                }
            }
            return alpha;
        }
    }
}

Move MyBot::play( const OthelloBoard& board )
{
    list<Move> moves = board.getValidMoves( turn );
    int randNo = rand() % moves.size();
    list<Move>::iterator it = moves.begin();

    return *it;
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


