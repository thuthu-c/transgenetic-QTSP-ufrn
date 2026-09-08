#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <queue>
#include <bits/stdc++.h>
#include "../../include/algorithms/rcl.h"

extern std::mt19937 engine;


int RCL::eval(std::vector<int> &tour, Graph &graph)
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

std::tuple<int, int, int> RCL::getInitialSubtour(Graph &graph)
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

            int tourVal = eval(tour, graph);

            if (tourVal < bestCost)
            {
                bestCost = tourVal;
                v3 = x;
            }
        }
    }

    return std::make_tuple(v1, v2, v3);
}

std::vector<int> RCL::run(Graph &graph)
{

    std::cout<<"to entrando no rcl sim" << std::endl;
    alpha= graph.getNumVertex()*0.5;
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

    std::vector<std::tuple<int, int, int>> rcl;

    while (verticesToCheck.size() > 0) {
        int bestInsertionCost = INT_MAX;
        int bestInsertionVertex = -1;
        int bestInsertionIndex = -1;

        for (int k : verticesToCheck) {
            for (auto i{0u}; i < tour.size(); i++) {
                std::vector<int> tmpTour = tour;
                tmpTour.insert(tmpTour.begin() + i, k);
                int localEval = eval(tmpTour, graph);

                if (localEval < bestInsertionCost) {
                    bestInsertionCost = localEval;
                    bestInsertionVertex = k;
                    bestInsertionIndex = i;
                }

                rcl.emplace_back(std::make_tuple(localEval, k, i));
            }
        }

         std::sort(rcl.begin(), rcl.end(),
              [](const std::tuple<int, int, int> &a, const std::tuple<int, int, int> &b)
              {
                  return std::get<0>(a) < std::get<0>(b);
              });
        
        if (rcl.empty())
            break;

     
        const int lastRclIndex = std::min(
            static_cast<int>(rcl.size()) - 1,
            std::max(0, static_cast<int>(alpha) - 1));
        std::uniform_int_distribution<int> distribution(0, lastRclIndex);
        int randIdx = distribution(engine);
        std::cout<<"o idx eh " << randIdx << std::endl;
        std::tuple<int, int,int> electedCandidate = rcl[randIdx];
        const int idxElected = std::get<2>(electedCandidate);
        const int vertexElected = std::get<1>(electedCandidate);
        std::cout<< "o idx do eleito eh " << idxElected << " o vertice eh " <<  vertexElected << std::endl;

        
        tour.insert(tour.begin() + idxElected,  vertexElected);

        verticesToCheck.erase(std::remove(verticesToCheck.begin(), verticesToCheck.end(), vertexElected), verticesToCheck.end());
        rcl.clear();
    }

    return tour;
}