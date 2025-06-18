#include "Minimax.hpp"
using namespace std;
using namespace Heuristic;

int Minimax::eval(const GameState &s, Player me) const{
    switch(hID){
        case 1: return orb_diff(s,me);
        case 2: return threat(s,me);
        case 3: return positional(s,me);
        case 4: return stability(s,me);
        case 5: return frontier(s,me);
        default: return composite(s,me);
    }
}

int Minimax::recurse(GameState &s,int depth,int alpha,int beta,bool maxP,Player me){
    Player winner; 
    if(s.is_terminal(winner) || depth==0){
        if(s.is_terminal(winner)){
            if(winner==me) return numeric_limits<int>::max()/2;
            else return numeric_limits<int>::min()/2;
        }
        return eval(s,me);
    }
    if(maxP){
        int best = numeric_limits<int>::min();
        for(auto [r,c]: s.legal_moves(me)){
            GameState child = s; child.play(r,c,me);
            int val = recurse(child,depth-1,alpha,beta,false,me);
            best = max(best,val);
            alpha = max(alpha,val);
            if(beta<=alpha) break;
        }
        return best;
    }else{
        Player opp = other(me);
        int best = numeric_limits<int>::max();
        for(auto [r,c]: s.legal_moves(opp)){
            GameState child = s; child.play(r,c,opp);
            int val = recurse(child,depth-1,alpha,beta,true,me);
            best = min(best,val);
            beta = min(beta,val);
            if(beta<=alpha) break; 
        }
        return best;
    }
}

SearchResult Minimax::search(GameState &state, Player me){
    int alpha = numeric_limits<int>::min();
    int beta  = numeric_limits<int>::max();
    int bestVal = numeric_limits<int>::min();
    pair<int,int> bestMove{-1,-1};
    for(auto [r,c]: state.legal_moves(me)){
        GameState child = state; child.play(r,c,me);
        int val = recurse(child,max_depth-1,alpha,beta,false,me);
        if(val>bestVal){ bestVal=val; bestMove={r,c}; }
        alpha = max(alpha,val);
    }
    return {bestVal,bestMove};
}