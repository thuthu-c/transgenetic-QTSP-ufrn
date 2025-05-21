#ifndef _TSM_H_
#define _TSM_H_

#include "../data_structures/graph.h"
#include "tsp_solver.h"
#include <vector>
#include <algorithm>
#include <random>

class TabuMemetic : public TspSolver
{
public:
    TabuMemetic(int popSize, int maxEval, double mutRate, double crossRate);
    ~TabuMemetic();

    std::vector<int> run(Graph& graph) override;
    struct Individual {
        std::vector<int> tour;
        int cost;
    };

    // Make calculateTour public for testing
    int calculateTour(const long long int& x, const long long int& y, const Individual& ind);
    Individual best_ini;
    std::mt19937 g;

private:

    int populationSize;
    int maxEvaluations;
    int currEvaluations = 0;
    double mutationRate;
    double crossoverRate;
    std::vector<Individual> population;
    std::vector<Individual> ini_population;
    Graph* graph;

    void initializePopulation();
    void evaluatePopulation();
    std::pair<Individual, Individual> tournamentSelection(const std::vector<Individual>& population);
    Individual crossover(const Individual& parent1, const Individual& parent2);
    void mutate(Individual& individual);
    void transgenesis(Individual& individual);
    void apply_tabu_search(Individual& ind);
};

#endif
