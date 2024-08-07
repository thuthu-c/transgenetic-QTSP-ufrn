#ifndef _GRAPH_GENERATOR_H
#define _GRAPH_GENERATOR_H

#include "../data_structures/graph.h"
/*
It generates a complete graph
*/
class GraphGenerator
{
private:
    int nVertex;
    int maxWeight;
public:
    GraphGenerator(int nVertex, int maxWeight = 5000);
    ~GraphGenerator();
    Graph generate();
    int getMaxWeight();
};

#endif