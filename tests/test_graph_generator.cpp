#include "../libs/doctest.h"
#include <iostream>
#include "../include/data_structures/graph.h"
#include "../include/helpers/graphio.h"
#include "../include/benchmark/graph_generator.h"

TEST_CASE("It can generate graph")
{
    int nVertex = 3;
    GraphGenerator graphGenerator(nVertex);
    Graph graph = graphGenerator.generate();
    auto m = graph.custo;

    for(int i = 0; i < nVertex; i++) {
        for(int j = 0; j < nVertex; j++) {
            for(int k = 0; k < nVertex; k++) {
                bool case1 = i == 0 && j == 1 && k == 2;
                bool case2 = i == 2 && j == 1 && k == 0;
                bool case3 = i == 1 && j == 2 && k == 0;
                bool case4 = i == 0 && j == 2 && k == 1;
                bool case5 = i == 2 && j == 0 && k == 1;
                bool case6 = i == 1 && j == 0 && k == 2;
                
                if(case1 || case2 || case3 || case4 || case5 || case6){
                    CHECK(m[i][j][k] != 0);
                } else {
                    CHECK_EQ(m[i][j][k], 0);   
                }
            }
        }
    }
}
