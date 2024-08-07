#ifndef _TABU_H_
#define _TABU_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"
class Tabu : public TspSolver
{
public:
    int tempoMaximo;
    int aspiracaoMaximo;
    int numIter;
    Tabu(int a,int b,int c){
        tempoMaximo = a;
        aspiracaoMaximo = b;
        numIter = c;
    }

    Tabu();
    
    ~Tabu();

    std::vector<int> run(Graph& graph);

};


#endif