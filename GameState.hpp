#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP
#include <array>
#include <vector>
#include <string>
#include <optional>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

using namespace std;

constexpr int ROWS = 9;
constexpr int COLS = 6;

enum class Player : uint8_t { NONE = 0, RED = 1, BLUE = 2 };
inline Player other(Player p){ return p==Player::RED ? Player::BLUE : Player::RED; }

struct Cell{
    uint8_t count = 0;
    Player  owner = Player::NONE;
};

struct GameState{
    Cell board[ROWS][COLS]{};
    Player to_move = Player::RED;
    static int critical(int r, int c) {
    int crit = 4;
    if (r == 0 || r == ROWS-1) --crit;
    if (c == 0 || c == COLS-1) --crit;
    return crit;
}

bool in_bounds(int r, int c) const {
    return 0 <= r && r < ROWS && 0 <= c && c < COLS;
}



    vector<pair<int,int>> legal_moves(Player p) const {
        vector<pair<int,int>> mv;
        for(int r=0;r<ROWS;++r) for(int c=0;c<COLS;++c){
            const Cell &cell = board[r][c];
            if(cell.owner==Player::NONE || cell.owner==p)
                mv.emplace_back(r,c);
        }
        return mv;
    }

    bool is_terminal(Player &winner) const {
        bool red=false, blue=false;
        for(auto &row: board) for(const Cell &c: row){
            red  |= (c.owner==Player::RED);
            blue |= (c.owner==Player::BLUE);
        }
        if(!red && blue){ winner = Player::BLUE; return true; }
        if(!blue && red){ winner = Player::RED;  return true; }
        return false;
    }

    void stabilise(){
        vector<pair<int,int>> q;
        for(int r=0; r<ROWS; ++r) 
        for(int c=0; c<COLS; ++c){
            if(board[r][c].owner != Player::NONE && board[r][c].count >= critical(r,c)){
                q.emplace_back(r,c); 
            }
        }
        
        const int dr[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};

        while(!q.empty()){
            auto [r,c] = q.back();
            q.pop_back();

            if(board[r][c].count < critical(r,c)) continue;

            Player p = board[r][c].owner;
            int crit = critical(r,c);
            
            board[r][c].count -= crit;
            if(board[r][c].count == 0) board[r][c].owner = Player::NONE;
            
            for(int k=0;k<4;++k){
                int nr = r + dr[k], nc = c + dc[k];
                if(!in_bounds(nr,nc)) continue;

                Cell &nbr = board[nr][nc];
                nbr.count += 1;
                nbr.owner = p;
                
                if(nbr.count >= critical(nr,nc)){
                    q.emplace_back(nr,nc);
                }
            }
            
            if(board[r][c].count >= critical(r,c)){
                q.emplace_back(r,c);
            }
        }
    }
    
    bool play(int r,int c, Player p){
        Cell &cell = board[r][c];
        if(!(cell.owner==Player::NONE || cell.owner==p)) return false;
        
        cell.owner = p; 
        cell.count += 1;
        
        stabilise();

        to_move = other(p);
        return true;
    }
};


inline bool load_from_file(const string &path, GameState &st, string &header){
    ifstream fin(path);
    if(!fin) return false;
    getline(fin, header);
    string line; int row=0;
    while(row<ROWS && getline(fin,line)){
        istringstream iss(line); string tok; int col=0;
        while(col<COLS && iss>>tok){
            if(tok=="0"){ st.board[row][col]={0,Player::NONE}; }
            else {
                uint8_t cnt = tok[0]-'0'; Player owner = (tok[1]=='R'?Player::RED:Player::BLUE);
                st.board[row][col] = {cnt,owner};
            }
            ++col;
        }
        ++row;
    }
    return row==ROWS;
}

inline void dump_to_file(const string &path, const GameState &st, const string &header){
    ofstream fout(path,ios::trunc);
    fout<<header<<"\n";
    for(int r=0;r<ROWS;++r){
        for(int c=0;c<COLS;++c){
            const Cell &cell=st.board[r][c];
            if(cell.owner==Player::NONE) fout<<"0";
            else fout<<(int)cell.count<<(cell.owner==Player::RED?'R':'B');
            if(c!=COLS-1) fout<<' ';
        }
        fout<<"\n";
    }
    fout.flush();
}

#endif // GAME_STATE_HPP