#ifndef _GREEDY_APPROACH_H_
#define _GREEDY_APPROACH_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"

class GreedyApproach : public TspSolver
{
private:
    std::set<int> visto;
    std::pair<int,int> getmin(int penultimo,int ultimo, Graph &graph);
    std::pair<int,std::vector<int>> run(int prim,int sec, Graph &graph);
    int calculaTour(std::vector<int>& ordem, Graph &graph);
public:
    std::vector<int> run(Graph graph);
};

#endif