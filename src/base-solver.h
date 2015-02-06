#pragma once

#include "sudoku.h"

class BaseSolver {
public:

    //Method must solve the Board pointed to inplace
    virtual bool solve(Board*) = 0;
    
    //Stat counter
    unsigned int counter=0;
protected:
    //to be overridden with recursive algorithms
    virtual void recurse(Board*) {};
    
    //to be overridden with stochastic algorithms
    virtual void  iterate(Board*) {};
    virtual int score(Board*) {return 0;};
    
    //solves any obvious squares
    void solveObvious(Board*);
};
