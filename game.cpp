#include "game.h"
#include <bits/stdc++.h>
#include "Menu/Background.h"


Game::Game(int nbrows, int nbcols, int cell, Font fnt): font(fnt), cell_size(cell), grid(Grid(nbrows, nbcols, cell)), currentBlock(LBlock(cell)), holdBlock(LBlock(cell)), nb_ligne_total(0),
                                                        blocks({LBlock(cell), JBlock(cell), IBlock(cell), OBlock(cell), SBlock(cell), TBlock(cell), ZBlock(cell)}), first_time(true), beginning(0), ending(0),
                                                        reference_char_id({mk('L', 1), mk('J', 2), mk('I', 3), mk('O', 4), mk('S', 5), mk('T', 6), mk('Z', 7)}),
                                                        reference_id_char({mk(1, 'L'), mk(2, 'J'), mk(3, 'I'), mk(4, 'O'), mk(5, 'S'), mk(6, 'T'), mk(7, 'Z')}),
                                                        blocks_shuffle({LBlock(cell), JBlock(cell), IBlock(cell), OBlock(cell), SBlock(cell), TBlock(cell), ZBlock(cell)}), nextBlock(LBlock(cell)), hold(false),
                                                        gameOver(false), score(0), rotateSound(LoadSound("Sounds/rotate.mp3")), clearSound(LoadSound("Sounds/clear.mp3")), oldlvl(0), lvl(0) {initialize();}

void Game::initialize(){
    seq_block = generate();
    holdBlock = blocks[reference_char_id[seq_block[0]] - 1];
    currentBlock = blocks[reference_char_id[seq_block[0]] - 1];
    nextBlock = blocks[reference_char_id[seq_block[1]] - 1];
    gameOver = false;
    hold = false;
    score = 0;
    lvl = 0;
    oldlvl = 0;
    beginning = 0;
    ending = 1;
    nb_ligne_total = 0;
    first_time = true;
    grid.Initialize();
}
Game::~Game(){
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
}
void Game::shuffle(std::vector<Block>& vec) {
    auto now = std::chrono::high_resolution_clock::now();
    auto seed = now.time_since_epoch().count();
    srand(seed);
    // Fisher-Yates shuffle algorithm
    for (int i = (int)vec.size() - 1; i > 0; --i) {
        int j = rand() % 7;
        std::swap(vec[i], vec[j]);
    }
}
string Game::generate(){
    string ans;
    for(int i = 0; i < 143; i++){
        shuffle(blocks_shuffle);
        for(const auto& it:blocks_shuffle) ans += reference_id_char[it.id];
    }
    return ans;
}

void Game::Draw(int posx, int posy){
    grid.Draw(posx,posy);
    DrawTextEx(font, "Score", {553, 240}, 38, 2, BLUE);
    DrawTextEx(font, "Hold", {63, 240}, 38, 2, BLUE);
    DrawTextEx(font, "Next", {563, 460}, 38, 2, BLUE);
    DrawTextEx(font, "Lvl", {73, 460}, 38, 2, BLUE);


    char scoreText[10], lvlText[10];
    sprintf(scoreText, "%d", score);
    sprintf(lvlText, "%d", lvl);
    Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
    Vector2 textSizes = MeasureTextEx(font, lvlText, 38, 2);

    DrawTextEx(font, scoreText, {518 + (170 - textSize.x) / 2, 335}, 38, 2, BLUE);
    DrawTextEx(font, lvlText, {20 + (170 - textSizes.x) / 2, 555}, 38, 2, BLUE);
    if (grid.FullRow()) gameOver = true;
    else{
        currentBlock.Draw(posx, posy - 6 * cell_size);
        if (!first_time) {
            switch (holdBlock.id) {
                case 3:
                    holdBlock.Draw(-48, 252);
                    break;
                case 4:
                    holdBlock.Draw(-48, 234);
                    break;
                default:
                    holdBlock.Draw(-32, 232);
                    break;
            }
        }

        switch (nextBlock.id) {
            case 3:
                nextBlock.Draw(453, 470);
                break;
            case 4:
                nextBlock.Draw(452, 452);
                break;
            default:
                nextBlock.Draw(468, 452);
                break;
        }
    }

}

void Game::shuffle(){
    if (first_time){
        first_time = false;
        beginning++;
        ending++;
        holdBlock = blocks[currentBlock.id - 1];
        currentBlock = nextBlock;
        nextBlock = blocks[reference_char_id[seq_block[ending]] - 1];
        hold = true;
    }
    if (!hold){
        int ind = currentBlock.id - 1;
        currentBlock = blocks[holdBlock.id - 1];
        holdBlock = blocks[ind];
        hold = true;
    }
}
void Game::speed_currentBlock(double& interval){
    if (lvl-oldlvl){
        oldlvl = lvl;
        interval -= 0.05;
    }
}
void Game::HandleInput(float& timer, double& interval, bool down){
    if (lvl-oldlvl){
        oldlvl = lvl;
        interval -= 0.05;
    }
    int keyPressed = GetKeyPressed();
    if (keyPressed) available = true;
    if (down) MoveBlockDown();
    switch(keyPressed){
        case KEY_LEFT:
            timer = 0;
            MoveBlockLeft();
            break;
        case KEY_RIGHT:
            timer = 0;
            MoveBlockRight();
            break;
        case KEY_DOWN:
            timer += GetFrameTime();
            MoveBlockDown();
            break;
        case KEY_UP:
            timer = 0;
            RotateBlock();
            break;
        case KEY_SPACE:
            timer = 0;
            shuffle();
            break;
    }

    if (available) {
        if (IsKeyDown(KEY_DOWN)) {
            if (key == KEY_DOWN) {
                timer += GetFrameTime();
                if (timer >= 0.25f) {
                    DashBlockDown();
                    timer = 0;
                }
            } else {
                key = KEY_DOWN;
                timer = GetFrameTime();
            }
        } else if (IsKeyDown(KEY_LEFT)) {
            if (key == KEY_LEFT) {
                timer += GetFrameTime();
                if (timer >= 0.1f) {
                    MoveBlockLeft();
                    timer = 0;
                }
            } else {
                key = KEY_LEFT;
                timer = GetFrameTime();
            }
        } else if (IsKeyDown(KEY_RIGHT)) {
            if (key == KEY_RIGHT) {
                timer += GetFrameTime();
                if (timer >= 0.1f) {
                    MoveBlockRight();
                    timer = 0;
                }
            } else {
                key = KEY_RIGHT;
                timer = GetFrameTime();
            }
        } else if (IsKeyDown(KEY_UP)) {
            if (key == KEY_UP) {
                timer += GetFrameTime();
                if (timer >= 0.3f) {
                    RotateBlock();
                    timer = 0;
                }
            } else {
                key = KEY_UP;
                timer = GetFrameTime();
            }
        } else{
            available = false;
            timer = 0;
        }
    }

}
void Game::DashBlockDown(){
    if (!gameOver){
        while(!IsBlockOutside() && BlockFits()) currentBlock.Move(1, 0);
        currentBlock.Move(-1, 0);
        LockBlock();
    }
}
void Game::MoveBlockLeft(){
    if (!gameOver){
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || !BlockFits()) currentBlock.Move(0, 1);

    }
}

void Game::MoveBlockRight()
{
    if (!gameOver){
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || !BlockFits()) currentBlock.Move(0, -1);
    }
}

void Game::MoveBlockDown(){
    if (!gameOver){
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || !BlockFits()){
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside()
{
    vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item : tiles){
        if (grid.IsCellOutside(item.row, item.column)) return true;
    }
    return false;
}

void Game::RotateBlock(){
    if (!gameOver){
        currentBlock.Rotate();
        if (IsBlockOutside() || !BlockFits()) currentBlock.UndoRotation();
        else PlaySound(rotateSound);
    }
}

void Game::LockBlock(){
    available = false;
    vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles) grid.grid[item.row][item.column] = currentBlock.id;
    currentBlock = nextBlock;
    hold = false;
    beginning++;
    ending++;
    nextBlock = blocks[reference_char_id[seq_block[ending]] - 1];
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0){
        PlaySound(clearSound);
        UpdateScore(rowsCleared);
        nb_ligne_total += rowsCleared;
    }
}

bool Game::BlockFits(){
    vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item : tiles){
        if (!grid.IsCellEmpty(item.row, item.column)) return false;
    }
    return true;
}


void Game::UpdateScore(int linesCleared){
    switch (linesCleared){
        case 1:
            score += (40 * (lvl + 1));
            break;
        case 2:
            score += (100 * (lvl + 1));
            break;
        case 3:
            score += (300 * (lvl + 1));
            break;
        case 4:
            score += (1200 * (lvl + 1));
            break;
        default:
            break;
    }
    if (nb_ligne_total/10> lvl) lvl++;
}

string Game::getgrid() {
    vector<vector<int>> my_grid = grid.grid;
    string res;
    for (int i = 0; i < my_grid.size(); i++){
        for (int j = 0; j < my_grid[i].size(); j++){
            res += to_string(my_grid[i][j]);
        }
    }
    return res;
}

void Game::multiplayer_init() {
    holdBlock = blocks[reference_char_id[seq_block[0]] - 1];
    currentBlock = blocks[reference_char_id[seq_block[0]] - 1];
    nextBlock = blocks[reference_char_id[seq_block[1]] - 1];
    gameOver = false;
    hold = false;
    score = 0;
    lvl = 1;
    oldlvl = 1;
    beginning = 0;
    ending = 1;
    first_time = true;
    grid.Initialize();
}

