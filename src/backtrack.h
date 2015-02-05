#pragma once

#include "sudoku.h"
#include "base-solver.h"


class Backtrack : public BaseSolver {
public:
    bool solve(Board*);
    void recurse(Board*);
};
