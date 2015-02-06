#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>

#include "sudoku.h"
#include "backtrack.h"
#include "sim-anneal.h"

std::vector<std::string> puzzles;

int main(int argc, char** argv){

    //Simple puzzles
    puzzles.push_back("147250890360008070000300060230000600416930028790000300000400080670009010854720930");
    puzzles.push_back("257106839000000000030208050370802091081050320500000004000000000025931460946587213");
    puzzles.push_back("407008060002060004003407028230600905164090287750800603001904056005010002806003090");
    puzzles.push_back("350478069200000003000302000123506487700010006004000500802000305036921870947030621");

    //Moderate puzzles
    puzzles.push_back("340000000100050000008097000705200008020000050400005102000970600000020004000000027");
    puzzles.push_back("200009300060000000300005020100090076090040080650020003030700002000000030007200001");
    puzzles.push_back("400236908000080600000050000100900005050000090300001006000070000004090000905812007");
    puzzles.push_back("000487000008109200400000001000070000300000006090040010000654000804701603006823100");

    //Diabolic puzzles
    puzzles.push_back("006038070200009830300400000000000900010907060005000000000003008034800005080510200");
    puzzles.push_back("137040009000000705000083000070000503002000100605000040000570000904000000700090381");
    puzzles.push_back("140209080008700000070006040400000069680407001500000034020004090004900000960802010");
    puzzles.push_back("050030080006179400307000201530604078040798030000000000000000000020000040800542007");

    Board board;
    SimAnneal ssSolver;
    Backtrack btSolver;

    int i=0;
    std::stringstream ss;
    for(auto p : puzzles){
        i++;
        ss.str("");
        ss << "puzzle" << i;
        if(!board.populate(p))
            continue;

        std::cout << "Loaded " << ss.str() << std::endl;

        ssSolver.solve(&board);
        if(board.isSolved())
            std::cout << "\tSimAnneal ssSolver succeeded: " << ssSolver.iteration << " iterations" << std::endl;
        else
            std::cout << "\tSimAnneal ssSolver failed   : Final score = " << ssSolver.finalScore << std::endl;

        board.populate(p);


        ss << ".log";
        //write the score history to the 
        std::ofstream outLog( ss.str(), std::ios_base::out );
        std::ostream_iterator<int> out_iterator(outLog, "\n");
        std::copy(ssSolver.scoreHistory.begin(), ssSolver.scoreHistory.end(), out_iterator);
    }
}
