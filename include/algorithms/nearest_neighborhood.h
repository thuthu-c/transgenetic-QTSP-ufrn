#ifndef _NEARESTNEIGHBORHOOD_H_
#define _NEARESTNEIGHBORHOOD_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"

class NearestNeighborhood : public TspSolver
{
private:
    int findMinimumNeighborhoods(int i, Graph &graph, std::vector<bool> &visited);
    std::pair<int, int> getInitialEdge(Graph &graph);
    int eval(std::vector<int> &tour, Graph &graph);
public:
    NearestNeighborhood();
    ~NearestNeighborhood();
    std::vector<int> run(Graph &graph);
    void setVisited(int v);
};

#endif
