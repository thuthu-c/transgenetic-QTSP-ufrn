#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <queue>
#include <bits/stdc++.h>
#include "../../include/algorithms/cheapest_insertion.h"

int CheapestInsertion::eval(std::vector<int> &tour, Graph &graph)
{
    int tourVal = 0;
    int n = tour.size();

    tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
    tourVal += graph.custo[tour[n - 1]][tour[0]][tour[1]];

    for (int j = 0; j < (n - 2); j++)
    {
        tourVal += graph.custo[tour[j]][tour[j + 1]][tour[j + 2]];
    }

    return tourVal;
}

std::tuple<int, int, int> CheapestInsertion::getInitialSubtour(Graph &graph)
{
    int n = graph.getMaxM();

    int v1, v2, v3;
    int bestCost = INT_MAX;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                int xijMin = INT_MAX;
                for (int x = 0; x < n; x++)
                {
                    if (x != i && x != j)
                    {
                        if (graph.custo[x][i][j] < xijMin)
                        {
                            xijMin = graph.custo[x][i][j];
                        }
                    }
                }

                int ijxMin = INT_MAX;
                for (int x = 0; x < n; x++)
                {
                    if (x != i && x != j)
                    {
                        if (graph.custo[i][j][x] < ijxMin)
                        {
                            ijxMin = graph.custo[i][j][x];
                        }
                    }
                }

                if (xijMin + ijxMin < bestCost)
                {
                    bestCost = xijMin + ijxMin;
                    v1 = i;
                    v2 = j;
                }
            }
        }
    }

    bestCost = INT_MAX;
    for (int x = 0; x < n; x++)
    {
        if (x != v1 && x != v2)
        {
            std::vector<int> tour;
            tour.push_back(v1);
            tour.push_back(v2);
            tour.push_back(x);

            int tourVal = this->eval(tour, graph);

            if (tourVal < bestCost)
            {
                bestCost = tourVal;
                v3 = x;
            }
        }
    }

    return std::make_tuple(v1, v2, v3);
}

std::vector<int> CheapestInsertion::run(Graph &graph)
{
    auto initial = this->getInitialSubtour(graph);
    int v1 = std::get<0>(initial);
    int v2 = std::get<1>(initial);
    int v3 = std::get<2>(initial);

    std::vector<int> tour = {v1, v2, v3};

    std::vector<int> verticesToCheck;
    for (int i = 0; i < graph.getMaxM(); ++i) {
        if (std::find(tour.begin(), tour.end(), i) == tour.end()) {
            verticesToCheck.push_back(i);
        }
    }

    while (verticesToCheck.size() > 0) {
        int bestInsertionCost = INT_MAX;
        int bestInsertionVertex = -1;
        int bestInsertionIndex = -1;

        for (int k : verticesToCheck) {
            for (int i = 0; i < tour.size(); i++) {
                std::vector<int> tmpTour = tour;
                tmpTour.insert(tmpTour.begin() + i, k);

                int localEval = this->eval(tmpTour, graph);

                if (localEval < bestInsertionCost) {
                    bestInsertionCost = localEval;
                    bestInsertionVertex = k;
                    bestInsertionIndex = i;
                }
            }
        }

        tour.insert(tour.begin() + bestInsertionIndex, bestInsertionVertex);

        verticesToCheck.erase(std::remove(verticesToCheck.begin(), verticesToCheck.end(), bestInsertionVertex), verticesToCheck.end());
    }

    return tour;
}