#include "../libs/doctest.h"
#include <iostream>
#include "../include/data_structures/graph.h"

TEST_CASE("It can init graph")
{
    int numVertex = 3;
    
    Graph g(numVertex);
    
    for(int i = 0; i < numVertex; i++) {
        for(int j = 0; j < numVertex; j++) {
            for(int k = 0; j < numVertex; j++) {
                CHECK_EQ(g.custo[i][j][k], 0);
            }
        }
    }
}

TEST_CASE("It can assign value to graph")
{
    int numVertex = 2;
    
    Graph g(numVertex);
    
    g.addEdge(0, 1, 2, 10);
    
    CHECK_EQ(g.custo[0][1][2], 10);

    for(int i = 0; i < numVertex; i++) {
        for(int j = 0; j < numVertex; j++) {
            for(int k = 0; j < numVertex; j++) {
                if(! (i == 0 && j == 1 && k == 2)) {
                    CHECK_EQ(g.custo[i][j][k], 0);
                }
            }
        }
    }
}

TEST_CASE("It can get num Edges")
{
    int numVertex = 3;
    
    Graph g(numVertex);
    
    g.addEdge(0, 1, 2, 10);
    g.addEdge(1, 2, 0, 5);

    CHECK_EQ(g.getNumEdges(), 2);
}
