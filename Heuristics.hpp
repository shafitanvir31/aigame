#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP
#include "GameState.hpp"

namespace Heuristic{
    int orb_diff   (const GameState &s, Player me);
    int threat     (const GameState &s, Player me);
    int stability  (const GameState &s, Player me);
    int frontier   (const GameState &s, Player me);
    int positional (const GameState &s, Player me);
    int composite  (const GameState &s, Player me);   
}

#endif