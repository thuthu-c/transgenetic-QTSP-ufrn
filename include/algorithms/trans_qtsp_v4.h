#ifndef _TRANSQTSPV4_H_
#define _TRANSQTSPV4_H_

#include "../algorithms/trans_qtsp_v3.h"
#include <vector>


class TransQTSPV4 : public RemTransp
{
public: 
     TransQTSPV4(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize, int plasmidBank, double plasmidMin, double plasmidMax);
     TransQTSPV4(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize, int plasmidBank);
    virtual ~TransQTSPV4() = default;
    int plasmidBank;
    double plasmidMin, plasmidMax;
    bool taNoPop{false}, taNoGir{false}; 
    
    // std::vector<int> transposon_4OPT(const std::vector<int>& tour) override;

    std::vector<int> run(Graph& graphInput) override; 



protected:
   
};

#endif