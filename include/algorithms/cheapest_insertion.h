#ifndef _CHEAPEST_INSERTION_H_
#define _CHEAPEST_INSERTION_H_

#include "./genetic_algorithm.h"
#include "../data_structures/graph.h"

class CheapestInsertion : public TspSolver
{
private:
    int eval(std::vector<int> &tour, Graph &graph);
    std::tuple<int, int, int> getInitialSubtour(Graph &graph);
    std::vector<int> getCheapestNeighbors(Graph &graph);

public:
    std::vector<int> run(Graph &graph);
};

#endif