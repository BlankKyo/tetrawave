#pragma once
#include "grid.h"
#include "blocks.cpp"
#include <bits/stdc++.h>

#define mk make_pair

using namespace std;

class Game{
protected:
    map<char,int> reference_char_id;
    map<int,char> reference_id_char;
    // moving block
    void MoveBlockLeft();
    void MoveBlockRight();

    // generating random sequence of blocks;


    // checking block outside the grid or not
    bool IsBlockOutside();

    // rotate the block
    void RotateBlock();

    void LockBlock();
    bool BlockFits();

    // update score
    void UpdateScore(int linesCleared);

    //
    vector<Block> blocks;
    vector<Block> blocks_shuffle;
    Block currentBlock;
    Block nextBlock;
    Block holdBlock;
    bool hold;
    bool first_time;

    // sound effects
    Sound rotateSound{};
    Sound clearSound{};

    // font
    Font font;

    // current state
    int beginning;
    int ending;

public:
    void speed_currentBlock(double& interval);
    void multiplayer_init();
    Grid grid;
    string seq_block;
    string getgrid();
    string generate();
    // constructor
    Game(int nbrows, int nbcols, int cell, Font fnt);

    // destructor
    ~Game();

    virtual // initialize game
    void initialize();

    // draw the game
    void Draw(int posx, int posy);

    // handle input function
    void HandleInput(float& timer, double& interval, bool down);

    // moving block down
    void MoveBlockDown();

    // dashing block down
    void DashBlockDown();

    // permutation between current_block and hold_block
    void shuffle();

    static void shuffle(vector<Block>& vec);

    // state of the game
    bool gameOver;

    // score
    int score;
    int lvl;
    int oldlvl;
    int nb_ligne_total;

    // cell size
    int cell_size;

    int key = 0;

    bool available = false;
};
