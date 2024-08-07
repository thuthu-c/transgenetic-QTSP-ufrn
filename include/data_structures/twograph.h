/*
A 2-Graph is a pair (V, E) consisting of 
a node set V = {1, ..., n} and a set of 
undirected 2-edges E.
A 2-edge (i, j, k) \in V^3 := {(i, j, k) = (k, j, i): i, j, k \in V, |{i, j, k}| = 3}
consists of a sequence of three distinct nodes.
*/

#ifndef _TWOGRAPH_H_
#define _TWOGRAPH_H_

#include "graph.h"
#include <vector>
#include <map>
#include <set>

class TwoGraph
{
private:
    // stores nodes i, j, k and its weight
    std::map<std::tuple<int, int, int>, int> edges;
    std::set<int> vertexes;
    // returns the neighborhoods and weight of vertex i
    std::map<int, std::vector<std::tuple<int, int, int>>> adjList;
public:
    TwoGraph(Graph graph);
    ~TwoGraph();
    std::set<int> getVertexes();
    std::map<std::tuple<int, int, int>, int> getEdges();
    std::map<int, std::vector<std::tuple<int, int, int>>> getAdjList();
};

#endif