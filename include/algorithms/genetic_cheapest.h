#ifndef _GENETIC_CHEAPEST_H_
#define _GENETIC_CHEAPEST_H_

#include "./genetic_algorithm.h"
#include "../data_structures/graph.h"

class GeneticCheapest : public GeneticAlgorithm
{
using GeneticAlgorithm::GeneticAlgorithm;

private:
    int getK();
    std::vector<int> getCheapestNeighbors(Graph &graph);
public:
    std::vector<std::vector<int>> initializePopulation(Graph &graph);
    std::vector<int> run(Graph &graph) override;
};

#endif