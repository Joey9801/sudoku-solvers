#include "sudoku.h"

Element::Element() :
    value(0),
    _fixed(false)
{
    resetCandidates(true);
}

bool Element::getFixed(void){
    return _fixed;
}
void Element::setFixed(bool f){
    _fixed = f;
}

std::vector<int> Element::getCandidates(void){
//    if(fixed)
//        return std::vector<int>(9, 9);
    std::vector<int> c(9);
    int count=0;

    for(int i=0; i<9; i++){
        if(_candidates[i]){
            c[count]=i+1;
            count++;
        }
    }

    c.resize(count);
    return c;
}

void Element::resetCandidates(bool state){
    for(int i=0; i<9; i++)
        _candidates[i] = state;

    return;
}

void Element::setCandidates(std::vector<int> candidates){
    for(int i=0; i<9; i++)
        _candidates[i] = false;
    for(auto c : candidates)
        _candidates[c-1] = true;
}

void Element::setCandidate(int val, bool state){
    _candidates[val-1] = state;

    return;
}

int Element::countCandidates(void){
    int count=0;
    for(int i=0; i<9; i++)
        if(_candidates[i])
            count++;
    if(count==1)
        for(int i=0; i<9; i++)
            if(_candidates[i]){
                value = i+1;
                break;
            }

    return count;
}

Board::Board(){
    
}

bool Board::populate(int values[9][9]){

    for(int x=0; x<9; x++)
        for(int y=0; y<9; y++){
            elements[x][y].value = values[y][x];
            if(values[y][x] != 0)
                elements[x][y].setFixed(true);
            else
                elements[x][y].setFixed(false);
        }

    if(!isBoardConsistent()){
        std::cout << "Input board is not consistent" << std::endl;
        std::cout << "  - Board failed to populate" << std::endl;
        return false;
    }
    return true;
}

void Board::printBoard(void){
    
    for(int y=0; y<9; y++){

        for(int x=0; x<9; x++){
            if(elements[x][y].getFixed())
                std::cout << " \033[92m" << elements[x][y].value << "\033[0m ";
            else if (elements[x][y].value == 0)
                std::cout << "   ";
            else
                std::cout << " \033[0m" << elements[x][y].value << "\033[0m ";

            if(x==2 || x==5){
                std::cout << "|";
            }
        }

        std::cout << std::endl;
        if(y==2 || y==5){
            std::cout << "---------+---------+--------" << std::endl;
        }
    }

}


bool Board::isSquareConsistent(int x, int y){
    bool found[9] = {false, false, false, false, false, false, false, false, false};

    //Check the sub-block
    for(int u=(x/3)*3; u<((x/3)*3+3); u++){
        for(int v=(y/3)*3; v<((y/3)*3+3); v++){
            if(elements[u][v].value == 0)
                continue;

            if(found[elements[u][v].value-1])
                return false;
            else
                found[elements[u][v].value-1] = true;
        }
    }

    //reset found list
    for(int i=0; i<9; i++)
        found[i] = false;

    //check rows
    for(int u=0; u<9; u++){
        if(elements[u][y].value == 0)
            continue;

        if(found[elements[u][y].value-1])
            return false;
        else
            found[elements[u][y].value-1] = true;
    }
    
    //reset found list
    for(int i=0; i<9; i++)
        found[i] = false;

    //check columns
    for(int v=0; v<9; v++){
        if(elements[x][v].value == 0)
            continue;

        if(found[elements[x][v].value-1])
            return false;
        else
            found[elements[x][v].value-1] = true;
    }

    return true;
}

bool Board::isBoardConsistent(void){
    bool found[9] = {false, false, false, false, false, false, false, false, false};
    
    //check blocks
    for(int i=0; i<9; i++){
        //reset found list
        for(int j=0; j<9; j++)
            found[j] = false;
        
        for(int u=(i%3)*3; u<((i%3)*3+3); u++){
            for(int v=(i/3)*3; v<((i/3)*3+3); v++){
                if(elements[u][v].value == 0)
                    continue;

                if(found[elements[u][v].value-1])
                    return false;
                else
                    found[elements[u][v].value-1] = true;
            }
        }
    }
    
    //Check rows
    for(int y=0; y<9; y++){
        //reset found list
        for(int j=0; j<9; j++)
            found[j] = false;

        //Check the row
        for(int x=0; x<9; x++){
            if(elements[x][y].value == 0)
                continue;

            if(found[elements[x][y].value-1])
                return false;
            else
                found[elements[x][y].value-1] = true;
        }
    }

    //Check columns - same as rows, but with x and y switched
    for(int x=0; x<9; x++){
        //reset found list
        for(int j=0; j<9; j++)
            found[j] = false;

        //Check the column
        for(int y=0; y<9; y++){
            if(elements[x][y].value == 0)
                continue;

            if(found[elements[x][y].value-1])
                return false;
            else
                found[elements[x][y].value-1] = true;
        }
    }


    return true;
}

bool Board::isSolved(void){
    //Check if all the values are filled in
    for(int x=0; x<9; x++)
        for(int y=0; y<9; y++)
            if(elements[x][y].value==0)
                return false;

    //All values are filled in, check global consistency
    return isBoardConsistent();
}

void Board::setCandidates(void){
    //Reset all _candidates to true
    for(int x=0; x<9; x++)
        for(int y=0; y<9; y++)
            elements[x][y].resetCandidates(true);

    for(int x=0; x<9; x++){
        for(int y=0; y<9; y++){
            if(elements[x][y].value!=0)
                continue;
            
            //clear _candidates in row
            for(int u=0; u<9; u++)
                elements[u][y].setCandidate(elements[x][y].value-1, false);
            
            //clear _candidates in column
            for(int v=0; v<9; v++)
                elements[x][v].setCandidate(elements[x][y].value-1, false);
            
            //clear _candidates in subsquare
            for(int u=(x/3)*3; u<((x/3)*3+3); u++)
                for(int v=(y/3)*3; v<((y/3)*3+3); v++)
                    elements[u][v].setCandidate(elements[x][y].value-1, false);
        }
    }

    //fill in all the obvious ones
    static unsigned int numberSolved = 0;
    bool altered = false;
    std::vector<int> c;
    for(int x=0; x<9; x++){
        for(int y=0; y<9; y++){
            c = elements[x][y].getCandidates();
            
            if(c.size()==1){
                elements[x][y].value = c[0];
                altered=true;
                numberSolved++;
            }

        }
    }
    if(altered){
        setCandidates();
    }
    
    return;
}

int Board::findLeastCandidates(void){
    unsigned int min=10;
    int minLoc = 0;
    for(int i=0; i<81; i++){
        if(elements[i%9][i/9].value == 0){
            if(elements[i%9][i/9].getCandidates().size() < min){
                min = elements[i%9][i/9].getCandidates().size();
                minLoc = i;
            }
        }

    }
    return minLoc;
}
