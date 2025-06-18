#include "Heuristics.hpp"
namespace {
  
    constexpr int DR[4]{-1, 1, 0, 0};
    constexpr int DC[4]{ 0, 0,-1, 1};

   
    inline int orb_count(const GameState &s, Player p)
    {
        int total = 0;
        for (auto &row : s.board)
            for (auto &c : row)
                if (c.owner == p) total += c.count;
        return total;
    }


    inline bool has_neighbour(const GameState &s,int r,int c,Player who)
    {
        for (int k = 0; k < 4; ++k) {
            int nr = r + DR[k], nc = c + DC[k];
            if (!s.in_bounds(nr, nc)) continue;
            if (s.board[nr][nc].owner == who) return true;
        }
        return false;
    }
}

namespace Heuristic {

int orb_diff(const GameState &s, Player me)
{
    return orb_count(s, me) - orb_count(s, other(me));
}

int threat(const GameState &s, Player me)
{
   
    int cnt   = 0;
    Player opp = other(me);
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c) {
            const Cell &cell = s.board[r][c];
            if (cell.owner != opp) continue;
            if (cell.count == GameState::critical(r, c) - 1 && has_neighbour(s, r, c, me))
                ++cnt;
        }
    return cnt;  
}

int stability(const GameState &s, Player me)
{
    int danger = 0;
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c) {
            const Cell &cell = s.board[r][c];
            if (cell.owner != me) continue;
            if (cell.count == GameState::critical(r, c) - 1)
                ++danger;
        }
    return -danger;
}

int frontier(const GameState &s, Player me)
{
    int score = 0;
    Player opp = other(me);
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            if (s.board[r][c].owner == me && has_neighbour(s, r, c, opp))
                ++score;
    return score;
}

int positional(const GameState &s, Player me)
{
    
    int val = 0;
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c) {
            if (s.board[r][c].owner != me) continue;
            int crit = GameState::critical(r, c);
            val += (5 - crit);
        }
    return val;
}

int composite(const GameState &s, Player me)
{
    
    return  10 * orb_diff(s, me)
          +  5 * threat(s, me)
          +  5 * stability(s, me)
          +  3 * frontier(s, me)
          +  1 * positional(s, me);
}

} 