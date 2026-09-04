#ifndef _TRANSQTSPV3_H_
#define _TRANSQTSPV3_H_

#include "../algorithms/trans_qtsp_v2.h"
#include <vector>


class  RemTransp: public TransQTSPV2
{
public: 
     RemTransp(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize, int plasmidBank);
    ~RemTransp() = default;
    int plasmidBank;
    
    std::vector<int> transposon_4OPT(const std::vector<int>& tour) override;
    int eval(std::vector<int> &tour, Graph &graph);
    std::vector<int> cheapest_insertion(std::vector<int> &tour, std::vector<int> vertexToBeInserted, Graph &graph);



protected:
   
};

#endif