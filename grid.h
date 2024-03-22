#pragma once
#include <vector>
#include <raylib.h>
#include "block.h"

using namespace std;


class Grid{
private:
    int numRows;
    int numCols;
    int cellSize;
    vector<Color> colors;

private:
    bool IsRowFull(int row);
    void ClearRow(int row);
    void MoveRowDown(int row, int numRows);

public:
    Grid(int nbrows, int nbcols, int cell);
    void Initialize();
    void Print();
    void Draw(int posx, int posy);
    bool IsCellOutside(int row, int column) const;
    bool FullRow();
    bool IsCellEmpty(int row, int column);
    int ClearFullRows();
    pair<int,int> FullRowContribute(Block& piece);
    pair<int,int> IsRowFull(int row, Block& piece);
    int Row_variation();
    int Column_variation();
    pair<int,int> calculate_holes_wells();
    void dfs(vector<vector<bool>>& visit, int i, int j, int& count);
    vector<vector<int>> grid;

};