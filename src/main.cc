#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "sudoku.h"
#include "backtrack.h"
#include "sim-anneal.h"

int puzzle[9][9] = {
    {0, 0, 5,   3, 0, 0,   0, 0, 0},
    {8, 0, 0,   0, 0, 0,   0, 2, 0},
    {0, 7, 0,   0, 1, 0,   5, 0, 0},

    {4, 0, 0,   0, 0, 5,   3, 0, 0},
    {0, 1, 0,   0, 7, 0,   0, 0, 6},
    {0, 0, 3,   2, 0, 0,   0, 8, 0},

    {0, 6, 0,   5, 0, 0,   0, 0, 9},
    {0, 0, 4,   0, 0, 0,   0, 3, 0},
    {0, 0, 0,   0, 0, 9,   7, 0, 0},
};

int main(int argc, char** argv){
    Board board;
    SimAnneal solver;

    if(!board.populate(puzzle))
        return -1;

    std::cout << "Loaded puzzle: " << std::endl;
    board.printBoard();

   solver.solve(&board);

    //write the score history to the 
    std::ofstream outLog("./out.log");
    std::ostream_iterator<int> out_iterator(outLog, "\n");
    std::copy(solver.scoreHistory.begin(), solver.scoreHistory.end(), out_iterator);

    board.printBoard();
}
