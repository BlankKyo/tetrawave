#pragma once
#include <vector>
#include <map>
#include "position.h"
#include "colors.h"

using namespace std;

///////  Block Class  ///////


class Block
{
private:

    int cellSize;
    int rotationState;
    vector<Color> colors;


public:
    int rowOffset;
    int columnOffset;
    explicit Block(int cell);
    void Draw(int offsetX, int offsetY);
    void Move(int rows, int columns);
    vector<Position> GetCellPositions();
    void Rotate();
    void Rotate(int rotation);
    void UndoRotation();
    int id{};
    map<int, vector<Position>> cells;

};