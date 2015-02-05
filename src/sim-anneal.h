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
    

    void  iterate(Board*);
    float score(Board*);

    void  fillRandom(Board*);
    void  swapRandom(Board*);
private:
    int _lastSwapY;
    int _lastSwapX1;
    int _lastSwapX2;
};
