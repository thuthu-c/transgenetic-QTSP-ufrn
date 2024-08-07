#include <iostream>
#include <algorithm>
#include "../libs/doctest.h"
#include "../include/data_structures/graph.h"
#include "../include/helpers/graphio.h"
#include "../include/benchmark/graph_generator.h"
#include "../include/algorithms/genetic_cheapest.h"

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

    GeneticCheapest gg(
        maxGenerations,
        populationSize,
        crossoverRate,
        mutationRate,
        elitimsNumber
    );

    auto path = gg.run(graph);

    std::set<int> s;
    for(auto p: path) {
        s.insert(p);
    }

    CHECK_EQ(path.size(), s.size()); // check if vertex are unique
    CHECK_EQ(path.size(), numVertex);

    for(auto p: path) {
        CHECK(p < numVertex);
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

    GeneticCheapest ga(
        maxGenerations,
        populationSize,
        crossoverRate,
        mutationRate,
        elitimsNumber
    );

    CHECK(ga.isElitism(0) == true);
    CHECK(ga.isElitism(1) == false);
}
