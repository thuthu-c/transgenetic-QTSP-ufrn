#ifndef _RCL_H_
#define _RCL_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"
class RCL : public TspSolver
{
    std::tuple<int, int, int> getInitialSubtour(Graph &graph);
    int alpha;

public:
    int eval(std::vector<int> &tour, Graph &graph);
    std::vector<int> run(Graph &graph);
    

};


#endif