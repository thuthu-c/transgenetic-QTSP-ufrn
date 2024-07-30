#ifndef _TSPSOLVER_H_
#define _TSPSOLVER_H_

#include "graph.h"
#include <vector>

class TspSolver
{
public:
    virtual std::vector<int> run(Graph &graph) = 0;
};

#endif