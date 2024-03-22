#include "robot.hpp"

Bot::Bot(int nbrows, int nbcols, int cell, Font fnt, long long W_Eroded_Piece_Cells, long long W_Landing_height, long long W_Board_Wells, long long W_Row_transitions, long long W_Column_transitions, long long W_Buried_Holes): Game(nbrows, nbcols, cell, fnt), lastUpdateTime(0), started(false), gameOver_bot(false){
    thread decision_robot(&Bot::decision, this, W_Eroded_Piece_Cells, W_Landing_height, W_Board_Wells, W_Row_transitions, W_Column_transitions, W_Buried_Holes);
    decision_robot.detach();
};

void Bot::initialize(){
    Game::initialize();
    started = false;
    gameOver_bot = false;

}
bool Bot::wait(double duration){
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= duration){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool Bot::BlockFits(Grid& grid1, Block& piece){
    vector<Position> tiles = piece.GetCellPositions();
    for(Position item : tiles){
        if (!grid1.IsCellEmpty(item.row, item.column)) return false;
    }
    return true;
}
bool Bot::IsBlockOutside(Grid& grid1, Block& piece){
    vector<Position> tiles = piece.GetCellPositions();
    for(Position item : tiles){
        if (grid1.IsCellOutside(item.row, item.column)) return true;
    }
    return false;
}
void Bot::movecheck(Grid& grid1, Block& piece, int translation) {
    int pas = 1;
    if (translation <= 0) pas = -1;
    for(int i = 0; i < abs(translation); i++) {
        piece.Move(0, pas);
        if (IsBlockOutside(grid1, piece) || !BlockFits(grid1, piece)) {
            piece.Move(0, pas * (-1));
            break;
        }
    }
}
void Bot::FullDrop(Grid& grid1, Block& piece) {
    while(!IsBlockOutside(grid1, piece) && BlockFits(grid1, piece)) piece.Move(1, 0);
    piece.Move(-1, 0);
    vector<Position> tiles = piece.GetCellPositions();
    for (Position item : tiles) grid1.grid[item.row][item.column] = piece.id;
}
void Bot::decision(long long W_Eroded_Piece_Cells, long long W_Landing_height, long long W_Board_Wells, long long W_Row_transitions, long long W_Column_transitions, long long W_Buried_Holes){
    while(true) {
        while (!gameOver_bot && started) {
            int bestRotationDelta = 0;
            int bestTranslationDelta = 0;
            long long bestMerit = LLONG_MIN; // Initialize to a very low value
            long long merit;
            Grid tempBoard = grid;
            Block tempPiece = currentBlock;

            // Iterate over all possible rotations and translations
            for (int rotationDelta = 0; rotationDelta < currentBlock.cells.size(); rotationDelta++) {
                for (int translationDelta = -4; translationDelta <= 7; translationDelta++) {
                    // Copy the current board and piece
                    tempBoard = grid;
                    tempPiece = currentBlock;

                    // Apply rotation and translation and Check if the move is valid
                    tempPiece.Rotate(rotationDelta);
                    movecheck(tempBoard, tempPiece, translationDelta);

                    // Drop the piece and evaluate the move

                    FullDrop(tempBoard, tempPiece);
                    merit = EvaluateMove(tempBoard, tempPiece, W_Eroded_Piece_Cells, W_Landing_height, W_Board_Wells, W_Row_transitions, W_Column_transitions, W_Buried_Holes);

                    // Update best move if this one is better
                    if (merit > bestMerit) {
                        bestMerit = merit;
                        bestRotationDelta = rotationDelta;
                        bestTranslationDelta = translationDelta;
                    }
                }
            }
            // Apply the best move
            if (wait(0.3)) {
                currentBlock.Rotate(bestRotationDelta);
                movecheck(grid, currentBlock, bestTranslationDelta);
                DashBlockDown();
            }
            //testing
            /*currentBlock.Rotate(bestRotationDelta);
            movecheck(grid, currentBlock, bestTranslationDelta);
            DashBlockDown();*/
            if (grid.FullRow()) gameOver_bot = true;
        }
    }
}

long long Bot::EvaluateMove(Grid& grid1, Block& piece, long long W_Eroded_Piece_Cells, long long W_Landing_height, long long W_Board_Wells, long long W_Row_transitions, long long W_Column_transitions, long long W_Buried_Holes){
    long long Landing_height = (long long) (26 - piece.rowOffset);
    int nbligne = grid1.FullRowContribute(piece).first, nbcellule = grid1.FullRowContribute(piece).second;
    long long Eroded_Piece_Cells = (long long)nbligne * (long long)nbcellule;
    int _ = grid1.ClearFullRows();
    long long Row_transitions = (long long)grid1.Row_variation(), Column_transitions = (long long)grid1.Column_variation();
    pair<int,int> value = grid1.calculate_holes_wells();
    long long Buried_Holes = (long long)value.first, Board_Wells = (long long)value.second;
    return W_Eroded_Piece_Cells * Eroded_Piece_Cells - W_Landing_height * Landing_height - W_Board_Wells * Board_Wells - W_Row_transitions * Row_transitions - W_Column_transitions * Column_transitions - W_Buried_Holes * Buried_Holes;
}

void Bot::Bdraw(int posx, int posy, bool done, double& interval){
    if (!gameOver) {
        Draw(posx, posy);
        speed_currentBlock((interval));
        if (done) MoveBlockDown();
    }
}