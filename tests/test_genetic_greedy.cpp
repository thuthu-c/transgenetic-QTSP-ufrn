#include <iostream>
#include <algorithm>
#include "../libs/doctest.h"
#include "../include/data_structures/graph.h"
#include "../include/helpers/graphio.h"
#include "../include/benchmark/graph_generator.h"
#include "../include/algorithms/genetic_greedy.h"

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

    GeneticGreedy gg(
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
