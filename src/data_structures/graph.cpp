#include <map>
#include <algorithm>
#include <iostream>
#include "../../include/data_structures/graph.h"

Graph::Graph(int maxm) {
    this->maxm = maxm;

    // initialize cost matrix
    int*** custo;

    custo = new int**[maxm];
    for(int i = 0; i < maxm; i++) {
        custo[i] = new int*[maxm];
        for(int j = 0; j < maxm; ++j) {
            custo[i][j] = new int[maxm];
            for(int k = 0; k < maxm; k++) {
                custo[i][j][k] = 0;
            }
        }
    }

    this->custo = custo;
}

Graph::Graph()
{
    
}

Graph::~Graph()
{
}

void Graph::addEdge(int v1, int v2, int v3, int weight)
{
    this->custo[v1][v2][v3] = weight;
}

int Graph::getNumEdges()
{
    int numEdges = 0;

    for(int i = 0; i < maxm; i++) {
        for(int j = 0; j < maxm; ++j) {
            for(int k = 0; k < maxm; k++) {
                if(custo[i][j][k] > 0) {
                    numEdges++;
                }
            }
        }
    }

    return numEdges;
}

int Graph::getMaxM()
{
    return this->maxm;
}

int Graph::getNumVertex(){ 
    return this->maxm; 
}