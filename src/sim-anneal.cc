#include "sim-anneal.h"

SimAnneal::SimAnneal(){
    //Seed the random number generator with the current time
    srand( time(NULL) );
}

bool SimAnneal::solve(Board* board){
    std::cout << std::endl
              << "Solving with the Simulated Annealing algorithm" << std::endl;

    solveObvious(board);

    fillRandom(board);

    for(int i=0; i<1000; i++){
        swapRandom(board);
        if(board->elements[0][0].getFixed()){
            std::cout << "became fixed on i=" << i << std::endl;
            break;
        }
    }
    if(!board->elements[0][0].getFixed()){
        std::cout << "Not fixed" << std::endl;
    }

    std::cout << "Board score = " << score(board) << std::endl;

    return true;
}


void  SimAnneal::iterate(Board*){

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
    if(x1>=x2)
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

    //std::cout << "y=" << y << ", x1=" << x1 << ", x2=" << x2 << std::endl;

    _lastSwapY  = y;
    _lastSwapX1 = x1;
    _lastSwapX2 = x2;

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

float SimAnneal::score(Board* board){
    //score the board
    //a score of 0 -> board is solved
    //a higher score indicates 'less solved' than a lower
    //
    //Each unique value in a column/subsquare is worth -1
    //Minimum is therefor -9x9x2=162 -> set initial score to 162
    float score = 162;

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
