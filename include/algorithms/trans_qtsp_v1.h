#ifndef _TRANSQTSPV1_H_
#define _TRANSQTSPV1_H_

#include "../algorithms/trans_qtsp.h"

class TransQTSPProbT : public TransQTSP 
{
public: 
    TransQTSPProbT(int maxEvaluations, int populationSize, double probT, double stepProb,  double plasmidSize);

    ~TransQTSPProbT() = default;

    double getProbT() const { return probT; }

    std::vector<int> run(Graph& graphInput) override;

protected:
    double probT;
    double stepProb;
    double plasmidSize;
};

#endif