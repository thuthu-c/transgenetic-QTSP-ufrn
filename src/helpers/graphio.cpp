#include <iostream>
#include <fstream>
#include "../../include/helpers/graphio.h"
#include "../../include/helpers/split.h"

GraphIO::GraphIO()
{
    
}

void GraphIO::setGraph(Graph graph)
{
    this->graph = graph;
}

GraphIO::GraphIO(Graph graph)
{
    this->graph = graph;
}

Graph GraphIO::getGraph()
{
    return this->graph;
}

void GraphIO::read(std::string filename)
{
    std::ifstream input(filename);

    std::string line;
    getline(input, line);
    Graph graph(stoi(line));

    while(getline(input, line))
    {
        std::vector<std::string> graphLine;
        
        split(line, ' ', graphLine);

        graph.addEdge(
            stoi(graphLine[0]),
            stoi(graphLine[1]),
            stoi(graphLine[2]),
            stoi(graphLine[3])
        );
    }

    this->graph = graph;
}

void GraphIO::write(std::string filename)
{
    int n = this->graph.getMaxM();
    int ***matrix = this->graph.custo;

    std::ofstream file;
    file.open(filename);

    // write the graph size
    file << this->graph.getMaxM() << "\n";

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; ++j) {
            for(int k = 0; k < n; k++) {
                file << i << " " << j << " " << k << " " << matrix[i][j][k] << "\n";
            }
        }
    }

    file.close();
}
