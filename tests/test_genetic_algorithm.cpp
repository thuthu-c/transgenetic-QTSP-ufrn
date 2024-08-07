#include <iostream>
#include <algorithm>
#include "../libs/doctest.h"
#include "../include/data_structures/graph.h"
#include "../include/helpers/graphio.h"
#include "../include/benchmark/graph_generator.h"
#include "../include/algorithms/genetic_algorithm.h"

TEST_CASE("It can perform genetic operations")
{
    int numVertex = 4;
    GraphGenerator graphGenerator(numVertex);
    Graph graph = graphGenerator.generate();

    int maxGenerations = 1;
    int populationSize = 100;
    float crossoverRate = 0.25;
    float mutationRate = 0.25;
    int elitimsNumber = 1;

    GeneticAlgorithm ga(
        maxGenerations,
        populationSize,
        crossoverRate,
        mutationRate,
        elitimsNumber
    );

    SUBCASE("It can mutate") {
        std::vector<int> individual = {0, 3, 1, 2};

        std::vector<int> mutatedIndividual = ga.mutate(individual);
        bool atLeastOneMutation = false;

        for(int i = 0; i < (int)individual.size(); i++) {
            if(individual[i] != mutatedIndividual[i]) {
                atLeastOneMutation = true;
                break;
            }
        }

        CHECK(std::find(mutatedIndividual.begin(), mutatedIndividual.end(), -1) == mutatedIndividual.end());
        CHECK(atLeastOneMutation == true);
    }

    SUBCASE("It can init population") {
        std::vector<std::vector<int>> population = ga.initializePopulation(graph);
        CHECK_EQ(population.size(), 100);
        CHECK_EQ(population[0].size(), 4);

        for(auto p: population) {
            auto it = std::unique(p.begin(), p.end());
            bool wasUnique = (it == p.end());
            CHECK(wasUnique);

            for(auto pp: p){
                if(pp == -1) {
                    CHECK(false);
                }
            }
        }
    }

    SUBCASE("It can crossover") {
        std::vector<int> parent1 = {0, 3, 1, 2};
        std::vector<int> parent2 = {2, 1, 3, 0};
        
        auto offspring = ga.crossover(parent1, parent2);
        CHECK_EQ(offspring.size(), numVertex);

        // doesnt exist -1 on offspring
        CHECK(std::find(offspring.begin(), offspring.end(), -1) == offspring.end());
        
        auto it = std::unique(offspring.begin(), offspring.end());
        bool wasUnique = (it == offspring.end());
        CHECK(wasUnique);
    }

    SUBCASE("It can evaluate fitness") {
        std::vector<std::vector<int>> population = ga.initializePopulation(graph);
        ga.evaluateFitness(population, graph);
        CHECK_EQ(population.size(), populationSize);
    }
}

TEST_CASE("It can find elitism")
{
    std::vector<std::vector<int>> population;
    std::vector<int> a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    std::vector<int> b;
    b.push_back(4);
    b.push_back(5);
    b.push_back(6);
    population.push_back(a);
    population.push_back(b);

    int maxGenerations = 1;
    int populationSize = 100;
    float crossoverRate = 0.25;
    float mutationRate = 0.25;
    int elitimsNumber = 0.1;

    GeneticAlgorithm ga(
        maxGenerations,
        populationSize,
        crossoverRate,
        mutationRate,
        elitimsNumber
    );

    CHECK(ga.isElitism(0) == true);
    CHECK(ga.isElitism(1) == false);
}

TEST_CASE("It can run genetic algorithm")
{
    int numVertex = 5;
    GraphGenerator graphGenerator(numVertex);
    Graph graph = graphGenerator.generate();

    int maxGenerations = 1;
    int populationSize = 100;
    float crossoverRate = 0.25;
    float mutationRate = 0.25;
    int elitimsNumber = 0.1;

    GeneticAlgorithm ga(
        maxGenerations,
        populationSize,
        crossoverRate,
        mutationRate,
        elitimsNumber
    );

    auto path = ga.run(graph);

    CHECK_EQ(path.size(), numVertex);

    for(auto p: path) {
        CHECK(p < numVertex);
    }
}
