#include "../libs/doctest.h"
#include "../include/data_structures/graph.h"
#include "../include/data_structures/twograph.h"

TEST_CASE("It can convert graph to twograph")
{
    // 1, 2, 3 - 3
    // 1, 3, 2 - 5
    // 2, 1, 3 - 4
    // 2, 3, 1 - 5
    // 3, 2, 1 - 3
    // 3, 1, 2 - 4

    Graph graph;
    graph.addEdge(1,2, 1);
    graph.addEdge(2,3, 2);
    graph.addEdge(3,1, 3);

    // edges expected
    std::map<std::tuple<int, int, int>, int> edgesExpected;
    edgesExpected[std::make_tuple(1, 2, 3)] = 3;
    edgesExpected[std::make_tuple(1, 3, 2)] = 5;
    edgesExpected[std::make_tuple(2, 1, 3)] = 4;
    edgesExpected[std::make_tuple(2, 3, 1)] = 5;
    edgesExpected[std::make_tuple(3, 2, 1)] = 3;
    edgesExpected[std::make_tuple(3, 1, 2)] = 4;

    // adj list expected
    std::map<int, std::vector<std::tuple<int, int, int>>> adjListExpected;
    std::vector<std::tuple<int, int, int>> v1, v2, v3;
    v1.push_back(std::make_tuple(2,3,3));
    v1.push_back(std::make_tuple(3,2,5));
    v2.push_back(std::make_tuple(1,3,4));
    v2.push_back(std::make_tuple(3,1,5));
    v3.push_back(std::make_tuple(2,1,3));
    v3.push_back(std::make_tuple(1,2,4));
    adjListExpected[1] = v1; 
    adjListExpected[2] = v2; 
    adjListExpected[3] = v3; 

    // asserts
    TwoGraph twoGraph(graph);

    CHECK_EQ(twoGraph.getEdges(), edgesExpected);
    CHECK_EQ(twoGraph.getVertexes(), graph.getVertexes());
    CHECK_EQ(twoGraph.getAdjList(), adjListExpected);
}