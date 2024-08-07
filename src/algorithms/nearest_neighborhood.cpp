#include "../../include/algorithms/nearest_neighborhood.h"
#include <limits>
#include <random>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>

NearestNeighborhood::NearestNeighborhood() {}

NearestNeighborhood::~NearestNeighborhood()
{
}

int NearestNeighborhood::eval(std::vector<int> &tour, Graph &graph)
{
    int tourVal = 0;
    int n = graph.getMaxM();

    tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
    tourVal += graph.custo[tour[n - 1]][tour[0]][tour[1]];

    for (int j = 0; j < (n - 2); j++)
    {
        tourVal += graph.custo[tour[j]][tour[j + 1]][tour[j + 2]];
    }

    return tourVal;
}

std::pair<int, int> NearestNeighborhood::getInitialEdge(Graph &graph)
{
    int n = graph.getMaxM();

    // choosing the v1 and v2
    int v1 = -1;
    int v2 = -1;

    int bestCost = INT_MAX;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                int acumulator = 0;
                for (int x = 0; x < n; x++)
                {
                    acumulator += graph.custo[x][i][j];
                }

                int localBestCost = INT_MAX;
                for (int x = 0; x < n; x++)
                {
                    if (graph.custo[i][j][x] < localBestCost)
                    {
                        localBestCost = graph.custo[i][j][x];
                    }
                }

                acumulator += localBestCost;

                if (acumulator < bestCost)
                {
                    bestCost = acumulator;
                    v1 = i;
                    v2 = j;
                }
            }
        }
    }

    return std::make_pair(v1, v2);
}

std::vector<int> NearestNeighborhood::run(Graph &graph)
{
    std::vector<int> vertexes;
    int n = graph.getMaxM();
    for (int i = 0; i < n; i++)
    {
        vertexes.push_back(i);
    }

    auto initial = getInitialEdge(graph);
    int v1, v2;
    v1 = initial.first;
    v2 = initial.second;

    std::vector<int> tour;
    tour.push_back(v1);
    tour.push_back(v2);

    while ((int) tour.size() < n)
    {
        // remaining vertexes
        std::vector<int> vertexToCheck;

        std::copy_if(
            vertexes.begin(),
            vertexes.end(),
            std::back_inserter(vertexToCheck),
            [&](int needle)
            {
                return std::find(tour.begin(), tour.end(), needle) == tour.end();
            }
        );

        std::pair<int, int> bestNeighbor(INT_MAX, -1);
        for (auto x : vertexToCheck)
        {
            bestNeighbor = std::min(std::make_pair(graph.custo[v1][v2][x], x), bestNeighbor);
        }

        v1 = v2;
        v2 = bestNeighbor.second;

        tour.push_back(bestNeighbor.second);
    }

    return tour;
}
