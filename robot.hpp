#pragma once
#include <vector>
#include <raylib.h>
#include "game.h"

using namespace std;

class Bot : public Game{
public:
    bool gameOver_bot;
    bool started;
    double lastUpdateTime;
    bool wait(double duration);
    Bot(int nbrows, int nbcols, int cell, Font fnt, long long W_Eroded_Piece_Cells, long long W_Landing_height, long long W_Board_Wells, long long W_Row_transitions, long long W_Column_transitions, long long W_Buried_Holes);
    void Bdraw(int posx, int posy, bool done, double& interval);
    void movecheck(Grid& grid1, Block& piece, int translation);
    void FullDrop(Grid& grid1, Block& piece);
    bool IsBlockOutside(Grid& grid1, Block& piece);
    void initialize();
    bool BlockFits(Grid& grid1, Block& piece);
    void decision(long long W_Eroded_Piece_Cells, long long W_Landing_height, long long W_Board_Wells, long long W_Row_transitions, long long W_Column_transitions, long long W_Buried_Holes);
    long long EvaluateMove(Grid& grid1, Block& piece, long long W_Eroded_Piece_Cells, long long W_Landing_height, long long W_Board_Wells, long long W_Row_transitions, long long W_Column_transitions, long long W_Buried_Holes);

};
