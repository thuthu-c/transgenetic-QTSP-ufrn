#ifndef _GRAPHIO_H_
#define _GRAPHIO_H_

#include <string>
#include "../data_structures/graph.h"

class GraphIO
{
private:
    Graph graph;
public:
    GraphIO();
    GraphIO(Graph graph);
    Graph getGraph();
    void setGraph(Graph graph);
    void read(std::string filename);
    void write(std::string filename);
};

#endif
