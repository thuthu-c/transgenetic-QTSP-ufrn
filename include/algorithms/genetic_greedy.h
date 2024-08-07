#ifndef _GENETIC_GREEDY_H_
#define _GENETIC_GREEDY_H_

#include "./genetic_algorithm.h"
#include "../data_structures/graph.h"

class GeneticGreedy : public GeneticAlgorithm
{
using GeneticAlgorithm::GeneticAlgorithm;

private:
    int eval(std::vector<int> &tour, Graph &graph);
    std::vector<int> getNearestNeighbors(int cityIndex, Graph &graph, int neighborNumber);
    std::vector<int> greedyPermuting(std::vector<int> &cities);
public:
    std::vector<std::vector<int>> initializePopulation(Graph &graph);
    std::vector<int> run(Graph &graph) override;
};

#endif