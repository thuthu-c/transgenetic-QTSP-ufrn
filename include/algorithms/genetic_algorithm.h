#ifndef _GENETIC_ALGORITHM_H_
#define _GENETIC_ALGORITHM_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"

class GeneticAlgorithm : public TspSolver
{
protected:
    int maxGenerations;
    int populationSize;
    float crossoverRate;
    int mutationRate;
    float elitimsNumber;
public:
    int compareFitness(std::vector<int> &a, std::vector<int> &b);
    GeneticAlgorithm(
        int maxGenerations,
        int populationSize,
        float crossoverRate,
        float mutationRate,
        float elitimsNumber
    );
    ~GeneticAlgorithm();
    std::vector<int> run(Graph &graph);
    void setCrossverRate(float crossRate);
    void setMutationRate(float mutationRate);
    int eval(std::vector<int> &tour, Graph &graph);

    //Warning::I wish all this methods were private. But testing private methods sucks in C++

    // verifies if it's a good solution. Avoiding to mutate/crossver the solution
    bool isElitism(int individualIndex);

    std::vector<std::vector<int>> initializePopulation(Graph &graph);
    void evaluateFitness(std::vector<std::vector<int>> &population, Graph &graph);
    std::vector<int> mutate(std::vector<int> &individual);
    std::vector<int> selectParent(std::vector<std::vector<int>> &population, Graph &graph);

    // 2-point crossover operator
    std::vector<int> crossover(std::vector<int> &parent1, std::vector<int> &parent2);
};

#endif