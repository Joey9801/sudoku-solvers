#pragma once

#include <vector>
#include <algorithm>
#include <cstdlib> //srand
#include <ctime>   //time
#include <cmath>

#include "sudoku.h"
#include "base-solver.h"


class SimAnneal : public BaseSolver {
public:
    SimAnneal(void);

    bool solve(Board*);

    std::vector<int> scoreHistory;
    int iteration;
    int finalScore;

    int   score(Board*);

    void  fillRandom(Board*);
    void  swapRandom(Board*);
    void  undoSwap(Board*);

    //Defines the cooling schedule
    float getTemperature(int);

    //Returns true if the swap is to be kept
    //Definges the acceptance probability function
    bool shouldWeAccept(int oldScore, int newScore, unsigned int iteration);
private:
    int _lastSwapY;
    int _lastSwapX1;
    int _lastSwapX2;
};
