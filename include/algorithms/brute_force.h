#ifndef _BRUTE_FORCE_H_
#define _BRUTE_FORCE_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"

class BruteForce : public TspSolver
{
public:
    BruteForce();
    ~BruteForce();
    int tourLength(std::vector<int> &vertexes, Graph &graph);
    std::vector<int> run(Graph &graph);
};

#endif
