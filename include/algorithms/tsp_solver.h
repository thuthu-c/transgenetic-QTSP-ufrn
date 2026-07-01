#ifndef _TSPSOLVER_H_
#define _TSPSOLVER_H_

#include "../data_structures/graph.h"
#include <vector>

class TspSolver
{
public:
    virtual ~TspSolver() {} 
    
    std::vector<int> b_ini_p;
    int b_ini_c;
    virtual std::vector<int> run(Graph &graph) = 0;
};

#endif
