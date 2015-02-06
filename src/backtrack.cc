#include "backtrack.h"

bool Backtrack::solve(Board* board){
//    std::cout << std::endl
//              << "Solving with the backtrack algorithm" << std::endl;

    solveObvious();

    recurse();

    return true;
}

void Backtrack::recurse(){
    counter++;

    int loc = board->findLeastCandidates();
    int x = loc%9;
    int y = loc/9;
    std::vector<int> candidates = board->elements[x][y].getCandidates();

    for(auto val : candidates){
        board->elements[x][y].value = val;

        if(board->isSolved()){
            return;
        }

        if(board->isSquareConsistent(x, y)){
            recurse();

            if(board->isSolved()){
                return;
            }
        }
    }
    board->elements[x][y].value = 0;
}
