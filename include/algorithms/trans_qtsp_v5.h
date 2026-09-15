#ifndef _TRANSQTSPV5_H_
#define _TRANSQTSPV5_H_

#include "../algorithms/trans_qtsp_v3.h"
#include <vector>


class TransQTSPV5 : public RemTransp
{
public: 
     TransQTSPV5(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize, int plasmidBank, double plasmidMin, double plasmidMax);
     TransQTSPV5(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize, int plasmidBank);
     TransQTSPV5(int maxEvaluations, int populationSize, double probT, double stepProb,double plasmidSize, int plasmidBank,  double plasmidMin, double plasmidMax, double probTmin, int variacaoSteps);
    virtual ~TransQTSPV5() = default;
    int plasmidBank;
    double plasmidMin, plasmidMax;
    bool taNoPop{false}, taNoGir{false}; 
    double probTmin;
    int variacaoSteps;
    
    // std::vector<int> transposon_4OPT(const std::vector<int>& tour) override;

    std::vector<int> run(Graph& graphInput) override; 



protected:
   
};

#endif