#include "grid.h"
#include <iostream>
#include "colors.h"

using namespace std;


Grid::Grid(int nbrows, int nbcols, int cell): numRows(nbrows), numCols(nbcols), cellSize(cell), colors(GetCellColors()) {Initialize();}

void Grid::Initialize(){grid.clear();grid.resize(numRows ,vector<int>(numCols));}

void Grid::Print(){
    for (int row = 6; row < numRows ; row++){
        for (int column = 0; column < numCols; column++)cout << grid[row][column] << " ";
        cout << endl;
    }
}

void Grid::Draw(int posx, int posy){
    posy -= 6 * cellSize;
    for (int row = 0; row < numRows ; row++){
        for (int column = 0; column < numCols; column++){
            int cellValue = grid[row][column];
            if (row>=6)
                DrawRectangle(column * cellSize + posx, row * cellSize + posy, cellSize - 1, cellSize - 1 , colors[cellValue]);
            else {
                if (cellValue)
                    DrawRectangle(column * cellSize + posx, row * cellSize + posy, cellSize - 1, cellSize - 1 , colors[cellValue]);
            }
        }
    }
}

bool Grid::IsCellOutside(int row, int column) const{
    if (row >= 0 && row < numRows  && column >= 0 && column < numCols) return false;
    return true;
}

bool Grid::IsCellEmpty(int row, int column){
    if (grid[row][column] == 0) return true;
    return false;
}
bool Grid::FullRow(){
    for(int i = 0; i < numCols; i++){
        if (!IsCellEmpty(5, i)) return true;
    }
    return false;
}
void Grid::dfs(vector<vector<bool>>& array, int i,int j, int& count){
    array[i][j] = true;
    int h[] = {1, -1, 0, 0}, v[] = {0, 0, 1, -1};
    for(int x = 0; x < 4; x++){
        int dx = i+h[x], dy = j+v[x];
        if(0 <= dx && dx < numRows && 0 <= dy && dy < numCols && !array[dx][dy] && grid[dx][dy] == 0) {
            count++;
            dfs(array, dx, dy, count);
        }
    }
}
pair<int,int> Grid::calculate_holes_wells(){
    vector<int> array(10, 26), array1(26, 10);
    pair<int,int> ans = make_pair(0, 0);
    int cumul = 0;
    for(int column = 0; column < numCols; column++){
        for(int row = 6; row < numRows; row++){
            if (column == 0){
                if (grid[row][column + 1]) cumul++;
            }
            else if (column == numCols - 1){
                if (grid[row][column - 1]) cumul++;
            }
            else{
                if (grid[row][column - 1] && grid[row][column + 1]) cumul++;
            }
            if (grid[row][column]) cumul = 0;
            ans.first += cumul;
        }
        cumul = 0;
    }
    for(int column = 0; column < numCols; column++){
        for(int row = numRows - 1; row >= 0; row--){
            if (grid[row][column] == 0) cumul++;
            else{
                ans.second += cumul;
                cumul = 0;
            }
        }
        cumul = 0;
    }
    return ans;
}
int Grid::ClearFullRows(){
    int completed = 0;
    for (int row = numRows - 1; row >= 6; row--){
        if (IsRowFull(row)){
            ClearRow(row);
            completed++;
        }
        else if (completed > 0) MoveRowDown(row, completed);
    }
    return completed;
}
int Grid::Column_variation(){
    int count = 0;
    for(int column = 0; column < numCols; column++){
        for(int row = 1; row < numRows; row++){
            if (grid[row][column]){
                if (grid[row - 1][column] == 0) count++;
            }
            else if (grid[row - 1][column]) count++;
        }
        if (grid[6][column] == 0) count++;
        if (grid[numRows - 1][column] == 0) count++;
    }
    return count;
}
int Grid::Row_variation(){
    int count = 0;
    for(int row = 6; row < numRows; row++){
        for(int column = 1; column < numCols; column++){
            if (grid[row][column]){
                if (grid[row][column - 1] == 0) count++;
            }
            else if (grid[row][column - 1]) count++;
        }
        if (grid[row][0] == 0) count++;
        if (grid[row][numCols - 1] == 0) count++;
    }
    return count;
}
pair<int,int> Grid::FullRowContribute(Block& piece){
    pair<int,int> completed = make_pair(0, 0);
    for (int row = numRows - 1; row >= 6; row--){
        completed.first += IsRowFull(row, piece).first;
        completed.second += IsRowFull(row, piece).second;
    }
    return completed;
}
pair<int,int> Grid::IsRowFull(int row, Block& piece){
    pair<int,int> ans = make_pair(1, 0);
    bool check = true;
    vector<Position> tiles = piece.GetCellPositions();
    for(int column = 0; column < numCols; column++){
        if (grid[row][column] == 0){
            check = false;
            break;
        }
        for (Position item : tiles){
            if (item.column == column && item.row == row) ans.second++;
        }
    }
    if (check) return ans;
    return make_pair(0, 0);
}
bool Grid::IsRowFull(int row){
    for (int column = 0; column < numCols; column++){
        if (grid[row][column] == 0) return false;
    }
    return true;
}

void Grid::ClearRow(int row){
    for (int column = 0; column < numCols; column++) grid[row][column] = 0;
}

void Grid::MoveRowDown(int row, int numRows){
    for (int column = 0; column < numCols; column++){
        grid[row + numRows][column] = grid[row][column];
        grid[row][column] = 0;
    }
}