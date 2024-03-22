#pragma once
#include <raylib.h>
#include <vector>

using namespace std;

///////  Color Class  ///////

extern const Color darkGrey;
extern const Color green;
extern const Color red;
extern const Color orange;
extern const Color yellow;
extern const Color purple;
extern const Color cyan;
extern const Color blue;
extern const Color lightBlue;
extern const Color darkBlue;
extern const Color lightestblue;

// vector for cell colors typically for the blocks and grid cells
vector<Color> GetCellColors();