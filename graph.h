#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <map>
#include <set>

class Graph
{
private:
    int maxm;
public:
    Graph();
    Graph(int maxm);
    ~Graph();
    void addEdge(int v1, int v2, int v3, int weight);
    int * getNeighboor(int v1) const;
    int getNumEdges() const;
    int getMaxM() const;
    int ***custo;
};

#endif
