#include "block.h"

using namespace std;


Block::Block(int cell): cellSize(cell), rotationState(0), colors(GetCellColors()), rowOffset(0), columnOffset(0){}

void Block::Draw(int offsetX, int offsetY){
    vector<Position> tiles = GetCellPositions();
    for (Position item : tiles){
        DrawRectangle(item.column * cellSize + offsetX, item.row * cellSize + offsetY, cellSize - 1, cellSize - 1, colors[id]);
    }
}

void Block::Move(int rows, int columns){
    rowOffset += rows;
    columnOffset += columns;
}

vector<Position> Block::GetCellPositions(){
    vector<Position> tiles = cells[rotationState];
    vector<Position> movedTiles;
    for (Position item : tiles){
        Position newPos = Position(item.row + rowOffset, item.column + columnOffset);
        movedTiles.push_back(newPos);
    }
    return movedTiles;
}

void Block::Rotate(){
    rotationState++;
    rotationState %= (int) cells.size();
}
void Block::Rotate(int rotation){
    rotationState = rotation % ((int) cells.size());
}
void Block::UndoRotation(){
    rotationState--;
    rotationState += (int) cells.size();
    rotationState %= (int) cells.size();
}