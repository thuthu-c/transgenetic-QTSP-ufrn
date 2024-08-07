#include "../libs/doctest.h"
#include <iostream>
#include "../include/data_structures/graph.h"
#include "../include/helpers/graphio.h"
#include "../include/benchmark/graph_generator.h"
#include "../include/algorithms/cheapest_insertion.h"

TEST_CASE("It can run cheapest insertion")
{
    Graph graph(4);
    graph.addEdge(0, 1, 2, 7);
    graph.addEdge(0, 1, 3, 5);
    graph.addEdge(0, 2, 1, 7);
    graph.addEdge(0, 2, 3, 5);
    graph.addEdge(0, 3, 1, 9);
    graph.addEdge(0, 3, 2, 10);
    graph.addEdge(1, 0, 2, 3);
    graph.addEdge(1, 0, 3, 8);
    graph.addEdge(1, 2, 0, 7);
    graph.addEdge(1, 2, 3, 9);
    graph.addEdge(1, 3, 0, 9);
    graph.addEdge(1, 3, 2, 7);
    graph.addEdge(2, 0, 1, 3);
    graph.addEdge(2, 0, 3, 7);
    graph.addEdge(2, 1, 0, 7);
    graph.addEdge(2, 1, 3, 8);
    graph.addEdge(2, 3, 0, 10);
    graph.addEdge(2, 3, 1, 7);
    graph.addEdge(3, 0, 1, 8);
    graph.addEdge(3, 0, 2, 7);
    graph.addEdge(3, 1, 0, 5);
    graph.addEdge(3, 1, 2, 8);
    graph.addEdge(3, 2, 0, 5);
    graph.addEdge(3, 2, 1, 9);

    CheapestInsertion cp;
    auto final = cp.run(graph);
    CHECK_EQ(final.size(), 4);
}