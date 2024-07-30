#include <iostream>
#include <climits>
#include "graph.h"
#include "ag.h" // Ensure the header file is included correctly

int main() {
    // Example graph initialization
    Graph graph(3); // Assuming 5 nodes for this example

    // Add edges with costs (example edges)
    graph.addEdge(0, 1, 2, 10);
    graph.addEdge(1, 2, 0, 20);
    graph.addEdge(2, 0, 1, 30);
    graph.addEdge(0, 2, 1, 40);
    graph.addEdge(1, 0, 0, 50);
    graph.addEdge(2, 1, 1, 60);
    // graph.addEdge(2, 0, 3, 70);
    // graph.addEdge(3, 1, 1, 80);

    // Genetic algorithm parameters
    int populationSize = 100;
    int numGenerations = 100;
    double mutationRate = 0.1;
    double crossoverRate = 0.2;

    // Initialize genetic algorithm
    AnotherGenetic geneticAlgorithm(populationSize, numGenerations, mutationRate, crossoverRate);

    // Run the genetic algorithm
    std::vector<int> bestTour = geneticAlgorithm.run(graph);
    std::cout<<"Tour size: "<<bestTour.size()<<std::endl;

    // Calculate and print the cost of the best tour
    int bestCost = geneticAlgorithm.calculateTour(bestTour);
    if(bestCost < INT_MAX)std::cout << "Best tour cost: " << bestCost << std::endl;
    else std::cout<<"There is no valid tour for this graph :("<<std::endl;

    // Print the best tour
    std::cout << "Best tour: ";
    for (int city : bestTour) {
        std::cout << city << " ";
    }
    std::cout << std::endl;

    return 0;
}
