#ifndef _AG_H_
#define _AG_H_

#include "graph.h"
#include "tsp_solver.h"
#include <vector>
#include <algorithm>
#include <random>

class AnotherGenetic : public TspSolver
{
public:
    AnotherGenetic(int popSize, int maxGen, double mutRate, double crossRate);
    ~AnotherGenetic();

    std::vector<int> run(Graph& graph) override;

    // Make calculateTour public for testing
    int calculateTour(const std::vector<int>& tour);

private:
    struct Individual {
        std::vector<int> tour;
        int cost;
    };

    int populationSize;
    int maxGenerations;
    double mutationRate;
    double crossoverRate;
    std::vector<Individual> population;
    Graph* graph;

    void initializePopulation();
    void evaluatePopulation();
    std::pair<Individual, Individual> tournamentSelection(const std::vector<Individual>& population);
    Individual crossover(const Individual& parent1, const Individual& parent2);
    void mutate(Individual& individual);
    void transgenesis(Individual& individual);
};

#endif
