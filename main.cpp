#include "Minimax.hpp"
#include <iostream>
#include <filesystem>
#include <cstdlib>
using namespace std;

int main(int argc,char* argv[]){
    if(argc<5){
        cerr<<"Usage: "<<argv[0]<<" <gamestate.txt> <R|B> <depth> <heuristic_id>\n";
        return 1;
    }
    const string path = argv[1];
    const Player myColour = (argv[2][0]=='R')?Player::RED:Player::BLUE;
    const int depth = atoi(argv[3]);
    const int hID   = atoi(argv[4]);

   
    string header;
    GameState state;
    while(true){
        if(!load_from_file(path,state,header)){
            this_thread::sleep_for(chrono::milliseconds(50));
            continue; }
        
        if((myColour==Player::RED && header=="AI Move:") || (myColour==Player::BLUE && header=="Human Move:")) break;
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    state.stabilise();

    Minimax engine(depth,hID);
    auto [val,move] = engine.search(state,myColour);
    if(move.first==-1){ 
        cerr<<"No move found!\n"; return 2; }
    state.play(move.first,move.second,myColour);
    const string next_header = (myColour == Player::RED) ? "Human Move:" : "AI Move:";
    dump_to_file(path, state, next_header);
    
    return 0;
}

