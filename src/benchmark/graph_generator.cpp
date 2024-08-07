#include "../../include/benchmark/graph_generator.h"
#include <set>
#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>

GraphGenerator::GraphGenerator(int nVertex, int maxWeight /*= 10000*/)
{
    this->nVertex = nVertex;
    this->maxWeight = maxWeight;
}

Graph GraphGenerator::generate()
{
    std::uniform_int_distribution<int> weightDistribution(1, this->maxWeight);
    std::random_device rd;
    std::mt19937 engine(rd());

    // init **graph
    // this generates a 2d complete graph
    int **graph;
    graph = new int*[this->nVertex];
    for(int i = 0; i < this->nVertex; i++) {
        graph[i] = new int[this->nVertex];
        for(int j = 0; j < this->nVertex; ++j) {
            if(i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = weightDistribution(engine) % this->maxWeight;
            }
        }
    }

    // casting the 2d graph to 3d graph
    Graph resultGraph(nVertex);
    for (int i = 0; i < this->nVertex; i++)
    {
        // get all adjacent vertex to i
        std::vector<std::pair<int, int>> firstVertexesLevel;

        for(int v = 0; v < this->nVertex; v++) {
            if(i != v && graph[i][v] > 0) {
                firstVertexesLevel.push_back(std::make_pair(v, graph[i][v]));
            }
        }

        for (std::pair<int, int> firstNeighbor : firstVertexesLevel)
        {  
            std::vector<std::pair<int, int>> secondVertexesLevel;
            for(int v = 0; v < this->nVertex; v++) {
                if(firstNeighbor.first != v && graph[firstNeighbor.first][v] > 0) {
                    secondVertexesLevel.push_back(std::make_pair(v, graph[firstNeighbor.first][v]));
                }
            }

            for (std::pair<int, int> secondNeighbor : secondVertexesLevel)
            {
                int weight1 = firstNeighbor.second;
                int weight2 = secondNeighbor.second;

                if (firstNeighbor.first != i && firstNeighbor.first != secondNeighbor.first && secondNeighbor.first != i)
                {
                    resultGraph.addEdge(i, firstNeighbor.first, secondNeighbor.first, weight1 + weight2); // i j k
                    resultGraph.addEdge(secondNeighbor.first, firstNeighbor.first, i, weight1 + weight2); // k j i
                }
            }
        }
    }

    return resultGraph;
}

GraphGenerator::~GraphGenerator()
{
}
