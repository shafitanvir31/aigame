#ifndef MINIMAX_HPP
#define MINIMAX_HPP
#include "GameState.hpp"
#include "Heuristics.hpp"
#include <limits>

struct SearchResult{ int value; std::pair<int,int> action; };

class Minimax{
public:
    Minimax(int depth,int heuristic_id):max_depth(depth),hID(heuristic_id){}
    SearchResult search(GameState &state, Player me);
private:
    int max_depth;
    int hID;
    int eval(const GameState &s, Player me) const;
    int recurse(GameState &s,int depth,int alpha,int beta,bool maxP,Player me);
};

#endif