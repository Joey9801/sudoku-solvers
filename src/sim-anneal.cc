#include "sim-anneal.h"

SimAnneal::SimAnneal(){
    //Seed the random number generator with the current time
    srand( time(NULL) );
}

bool SimAnneal::solve(Board* board){

    solveObvious(board);

    fillRandom(board);

    int oldScore, newScore;
    oldScore = score(board);
    iteration = 0;
    scoreHistory.clear();

    while( iteration < 500000 ){
        swapRandom(board);
        newScore = score(board);
        
        if( newScore == 0 ){
            oldScore = newScore;
            scoreHistory.push_back(newScore);
            break;
        }

        if( shouldWeAccept(oldScore, newScore, iteration) )
            oldScore = score(board);
        else
            undoSwap(board);
        
        iteration++;
        scoreHistory.push_back(oldScore);
    }

    finalScore = oldScore;

    return true;
}

float SimAnneal::getTemperature(int iteration){
    //Defines the cooling schedule
   return 0.5; 
}

bool SimAnneal::shouldWeAccept(int oldScore, int newScore, unsigned int iteration){
    //Returns true if the swap is to be kept
    //Definges the acceptance probability function

    if(newScore < oldScore)
        return true;

    float temperature = getTemperature(iteration);
    float p = exp( -(newScore - oldScore) / temperature );
    
    return rand()/(RAND_MAX+1.0) < p;
}

void SimAnneal::swapRandom(Board* board){
    //Swap two unfixed elements within a row
    //We keep within the row to ensure on of the three constraints is always solved

    int y = rand() % 9;

    //Generate vector of non-fixed elements
    //TODO precompute these for dem gainz
    std::vector<int> free;
    free.clear();

    for(int x=0; x<9; x++){
        if( !board->elements[x][y].getFixed() )
            free.push_back(x);
    }

    int x1, x2;
    x1 = rand() % (free.size());
    x2 = rand() % (free.size()-1);
    if(x2>=x1)
        x2++;

    x1 = free[x1];
    x2 = free[x2];

    //We only need to swap the values
    //Everything on the row already has the same candidates
    //(Not that we're using those here)
    int temp;
    temp = board->elements[x1][y].value;
    board->elements[x1][y].value = board->elements[x2][y].value;
    board->elements[x2][y].value = temp;

    _lastSwapY  = y;
    _lastSwapX1 = x1;
    _lastSwapX2 = x2;

    return;
}

void SimAnneal::undoSwap(Board* board){
    //We only need to swap the values
    //Everything on the row already has the same candidates
    //(Not that we're using those here)
    int temp;

    temp = board->elements[_lastSwapX1][_lastSwapY].value;

    board->elements[_lastSwapX1][_lastSwapY].value
        = board->elements[_lastSwapX2][_lastSwapY].value;

    board->elements[_lastSwapX2][_lastSwapY].value = temp;

    return;
}

void SimAnneal::fillRandom(Board* board){
    //Fills all the non-fixed elements with a random number
    //Each row is guarenteed to have the numbers 1-9, solving one constraint for free

    std::vector<int> candidates;
    bool found[9];

    for(int y=0; y<9; y++){

        candidates.clear();
        for(int i=0; i<9; i++)
            found[i] = false;

        for(int x=0; x<9; x++){
            if( board->elements[x][y].getFixed() )
                found[board->elements[x][y].value-1] = true;
        }

        for(int i=0; i<9; i++){
            if( found[i] == false ){
                candidates.push_back(i+1);
            }
        }

        std::random_shuffle( candidates.begin(), candidates.end() );
        int i=0;
        for(int x=0; x<9; x++){
            if( !board->elements[x][y].getFixed() ){
                board->elements[x][y].setCandidates( candidates );
                board->elements[x][y].value = candidates[i++];
            }
        }
    }

}

int SimAnneal::score(Board* board){
    //score the board
    //a score of 0 -> board is solved
    //a higher score indicates 'less solved' than a lower
    //
    //Each unique value in a column/subsquare is worth -1
    //Minimum is therefor -9x9x2=162 -> set initial score to 162
    int score = 162;

    int found[9];

    //Score columns
    for(int x=0; x<9; x++){
        for(int i=0; i<9; i++)
            found[i] = 0;

        for(int y=0; y<9; y++)
            found[ board->elements[x][y].value - 1]++;

        for(int i=0; i<9; i++){
            if(found[i]>0)
                score--;
        }
    }

    //Score subsquares
    //Access as elements[3*u + x][3*v + y]
    for(int u=0; u<3; u++){
        for(int v=0; v<3; v++){

            for(int i=0; i<9; i++)
                found[i] = 0;

            for(int x=0; x<3; x++){
                for(int y=0; y<3; y++){
                    found[ board->elements[3*u + x][3*v + y].value - 1]++;
                }
            }

            for(int i=0; i<9; i++){
                if(found[i]>0)
                    score--;
            }
        }
    }

    return score;
}
