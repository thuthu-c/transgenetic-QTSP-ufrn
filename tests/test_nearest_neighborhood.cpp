#include "../libs/doctest.h"
#include <iostream>
#include "../include/data_structures/graph.h"
#include "../include/algorithms/nearest_neighborhood.h"
#include "../include/algorithms/brute_force.h"

TEST_CASE("It can find minimum vertex")
{
    Graph graph(4);
    graph.addEdge(0, 1, 2, 2493);
    graph.addEdge(0, 1, 3, 4116);
    graph.addEdge(0, 2, 1, 3155);
    graph.addEdge(0, 2, 3, 5246);
    graph.addEdge(0, 3, 1, 7087);
    graph.addEdge(0, 3, 2, 4348);
    graph.addEdge(1, 0, 2, 4705);
    graph.addEdge(1, 0, 3, 5724);
    graph.addEdge(1, 2, 0, 3155);
    graph.addEdge(1, 2, 3, 3090);
    graph.addEdge(1, 3, 0, 7087);
    graph.addEdge(1, 3, 2, 2796);
    graph.addEdge(2, 0, 1, 4705);
    graph.addEdge(2, 0, 3, 4633);
    graph.addEdge(2, 1, 0, 2493);
    graph.addEdge(2, 1, 3, 2622);
    graph.addEdge(2, 3, 0, 4348);
    graph.addEdge(2, 3, 1, 2796);
    graph.addEdge(3, 0, 1, 5724);
    graph.addEdge(3, 0, 2, 4633);
    graph.addEdge(3, 1, 0, 4116);
    graph.addEdge(3, 1, 2, 2622);
    graph.addEdge(3, 2, 0, 5246);
    graph.addEdge(3, 2, 1, 3090);

    auto nn = NearestNeighborhood();

    auto result = nn.run(graph);
    // TODO::make assertions
}
