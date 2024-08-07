#include "../../include/algorithms/brute_force.h"
#include <algorithm>
#include <set>
#include <limits>
#include <iostream>

BruteForce::BruteForce() {}

BruteForce::~BruteForce() {}

std::vector<int> BruteForce::run(Graph &graph)
{
    std::vector<int> vertexes;
    std::vector<int> minPath;

    for (int i = 0; i < graph.getMaxM(); i++)
    {
        vertexes.push_back(i);
    }

    int shortestPathWeight = std::numeric_limits<int>::max();


    do
    {
        int currentWeight = tourLength(vertexes, graph);
        if(currentWeight < shortestPathWeight)
        {
            minPath = vertexes;
            shortestPathWeight = currentWeight;   
        }
    } while (std::next_permutation(vertexes.begin(), vertexes.end()));

    return minPath;
}


int BruteForce::tourLength(std::vector<int> &vertexes, Graph &graph)
{
    int tourVal = 0;
    int n = graph.getMaxM();

    tourVal += graph.custo[vertexes[n-2]][vertexes[n-1]][vertexes[0]];
    tourVal += graph.custo[vertexes[n-1]][vertexes[0]][vertexes[1]];

    for(int j = 0; j < (n-2); j++)
    {
        tourVal += graph.custo[vertexes[j]][vertexes[j+1]][vertexes[j+2]];
    }

    return tourVal;
}
