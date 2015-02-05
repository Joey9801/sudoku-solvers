#pragma once

#include <iostream>
#include <vector>

class Element {
public:
    Element();

    int value;

    bool getFixed();
    void setFixed(bool);

    std::vector<int> getCandidates(void);
    void setCandidate(int, bool);
    void setCandidates(std::vector<int>);
    void resetCandidates(bool);

    int countCandidates(void);

private:
    bool _fixed;
    bool _candidates[9];
};

class Board {
public:
    Board();
    Element elements[9][9];
    
    bool populate(int[9][9]);
    void printBoard(void);

    bool isSquareConsistent(int, int);
    bool isBoardConsistent(void);
    bool isSolved(void);

    void setCandidates(void);
    int findLeastCandidates(void);
};
