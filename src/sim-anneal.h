#pragma once

#include <vector>
#include <algorithm>
#include <cstdlib> //srand
#include <ctime>   //time

#include "sudoku.h"
#include "base-solver.h"


class SimAnneal : public BaseSolver {
public:
    SimAnneal(void);

    bool solve(Board*);

    std::vector<int> scoreHistory;

    void  iterate(Board*);
    int   score(Board*);

    void  fillRandom(Board*);
    void  swapRandom(Board*);
    void  undoSwap(Board*);
private:
    int _lastSwapY;
    int _lastSwapX1;
    int _lastSwapX2;
};
