#include <map>
#include <algorithm>
#include <iostream>
#include "../../include/data_structures/twograph.h"
/*
TwoGraph::TwoGraph(Graph graph)
{
    this->vertexes = graph.getVertexes();

    for (auto i : this->vertexes)
    {
        // combines every triple of vertex
        std::map<std::tuple<int, int, int>, int> tripletResult;

        // get all adjacent vertex to i.
        // The first pair of the vector is the edge, the second is the weight
        std::vector<std::pair<int, int>> firstVertexesLevel = graph.getAdjList().find(i)->second;
        std::map<std::pair<int, int>, int> edges = graph.getEdges(); // syntactic sugar to edges

        for (std::pair<int, int> firstNeighbor : firstVertexesLevel)
        {
            // get adjacent vertexes from vertexes that were adjacent to i
            std::vector<std::pair<int, int>> secondVertexesLevel = graph.getAdjList().find(firstNeighbor.first)->second;

            for (std::pair<int, int> secondNeighbor : secondVertexesLevel)
            {
                int weight1 = firstNeighbor.second;
                int weight2 = secondNeighbor.second;

                if (secondNeighbor.first != i)
                {
                    this->edges[std::make_tuple(i, firstNeighbor.first, secondNeighbor.first)] = weight1 + weight2;
                    this->adjList[i].push_back(std::make_tuple(firstNeighbor.first, secondNeighbor.first, weight1 + weight2));
                }
            }
        }
    }
}

TwoGraph::~TwoGraph()
{
}

std::set<int> TwoGraph::getVertexes()
{
    return this->vertexes;
}

std::map<std::tuple<int, int, int>, int> TwoGraph::getEdges()
{
    return this->edges;
}

std::map<int, std::vector<std::tuple<int, int, int>>> TwoGraph::getAdjList()
{
    return this->adjList;
}
*/