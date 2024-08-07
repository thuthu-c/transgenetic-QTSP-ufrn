#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../../include/helpers/graphio.h"

/*
The code running on Digital Ocean doesn't have the costs generated on its csv
this code calculates the tour cost based on 
*/

void ssplit(std::string &str, char delim, std::vector<std::string> &out)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}

void calculateCostFromCsv()
{
    std::ifstream input("results/digital-ocean-results.csv");

    std::string line;
    getline(input, line); // ignore header

    while(getline(input, line))
    {
        std::vector<std::string> graphLine;

        ssplit(line, ';', graphLine);

        GraphIO graphio;
        graphio.read(graphLine[1]);
        Graph graph = graphio.getGraph();

        std::vector<std::string> tourStr;
        ssplit(graphLine[graphLine.size()-1], ',', tourStr);
        std::vector<int> tour;
        for(auto tstr: tourStr) {
            tour.push_back(stoi(tstr));
        }

        std::cout << tourLength(tour, graph) << std::endl; //todo?
    }
}

int tourLength(std::vector<int> tour, Graph graph){//todo???? id where it should be tho
    return 0;
}