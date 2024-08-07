#include "../libs/doctest.h"
#include <iostream>
#include "../include/algorithms/brute_force.h"

TEST_CASE("It can check shortest path")
{
    Graph g(4);
    g.addEdge(0, 1, 2, 35);
    g.addEdge(0, 2, 1, 35);
    g.addEdge(1, 2, 0, 35);
    g.addEdge(2, 1, 0, 35);
    g.addEdge(0, 2, 3, 27);
    g.addEdge(3, 2, 0, 27);
    g.addEdge(1, 0, 3, 32);
    g.addEdge(3, 0, 1, 32);
    g.addEdge(1, 0, 2, 30);
    g.addEdge(2, 0, 1, 30);
    g.addEdge(0, 1, 3, 31);
    g.addEdge(3, 1, 0, 31);
    g.addEdge(1, 2, 3, 32);
    g.addEdge(3, 2, 1, 32);
    g.addEdge(1, 3, 2, 28);
    g.addEdge(2, 3, 1, 28);
    g.addEdge(1, 3, 0, 23);
    g.addEdge(0, 3, 1, 23);

    // expected vertexes
    std::vector<int> vertexes;
    vertexes.push_back(0);
    vertexes.push_back(2);
    vertexes.push_back(1);
    vertexes.push_back(3);

    BruteForce bf;
    auto result = bf.run(g);

    CHECK_EQ(result.size(), 4);
    for(int i = 0; i < (int)result.size(); i++) {
        CHECK_EQ(result[i], vertexes[i]);
    }
}
