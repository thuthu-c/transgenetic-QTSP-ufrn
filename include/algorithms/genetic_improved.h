#ifndef _GENETIC_IMPROVED_H_
#define _GENETIC_IMPROVED_H_

#include <functional>
#include <queue>
#include <unordered_map>
#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"

class GeneticImproved : public TspSolver
{
protected:
    int maxEvaluations;
    int populationSize;
    float crossoverRate;
    int mutationRate;
    int numEvaluation;
    int numBestIndividuals;
public:
    GeneticImproved(
        int maxEvaluations,
        int populationSize,
        float crossoverRate,
        float mutationRate
    );
    ~GeneticImproved();

    std::vector<int> run(Graph &graph);
    void setCrossverRate(float crossRate);
    void setMutationRate(float mutationRate);
    int eval(std::vector<int> &tour, Graph &graph);

    //Warning::I wish all this methods were private. But testing private methods sucks in C++
    std::vector<int> generateOffspring(std::vector<int> &mother, std::vector<int> &father);

    std::vector<int> greedyPermuting(std::vector<int> &individual, Graph &graph, int n);
    std::vector<int> getNearestNeighbors(int cityIndex, Graph &graph, int neighborNumber);
    std::vector<std::vector<int>> initializeNearestNeighbors(int populationSize, Graph &graph);
    
    std::vector<std::vector<int>> initializeRandomPopulation(int populationSize, Graph &graph);
    
    std::vector<std::vector<int>> initializePopulation(Graph &graph);
    
    std::vector<std::pair<int, std::vector<int>>> evaluatePopulation(std::vector<std::vector<int>> &population, Graph &graph);
    
    std::vector<int> mutate(std::vector<int> &individual);
    
    std::pair<int, std::vector<int>> selectParent(
        std::vector<std::pair<int, std::vector<int>>> &populationEvaluated
    );

    // 2-point crossover operator
    std::pair<int, std::vector<int>> crossover(std::vector<int> &parent1, std::vector<int> &parent2, Graph &graph);
};

#endif