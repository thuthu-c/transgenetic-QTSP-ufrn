#ifndef _TRANSQTSPV2_H_
#define _TRANSQTSPV2_H_

#include "../algorithms/trans_qtsp_v1.h"
#include <vector>

struct Individual {
    std::vector<int> tour;
    long long int cost;
    int costRank; 
    double diversityContribution;
    int diversityRank;
    double biasedFitness;
};

class TransQTSPV2 : public TransQTSPProbT 
{
public: 
    TransQTSPV2(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize);
    ~TransQTSPV2() = default;

    std::vector<int> run(Graph& graphInput) override;

protected:
    std::vector<Individual> generate_population(Graph *graph);
    int numberOfSuccesivesPairs(const std::vector<int>& Pi, const std::vector<int>& Pj);
    std::vector<std::pair<int, int>> generateAllNodesPairs(Individual individual);
    long int numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj(Individual Pi, Individual Pj);
    double normalizeBronkenPairsDistance(Individual Pi, Individual Pj);
    double dc(Individual& Pi);
    void individualDiversityRank();
    std::vector<Individual> population;
};

#endif