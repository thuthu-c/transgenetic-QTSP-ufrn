#include <iostream>
#include "../include/benchmark/benchmark.h"
#include<fstream>
using namespace std;

#include "../include/algorithms/tabu_memetic.h"
#include <climits>
#include <string>
#include <random>

//stdout is not working due to irace, we can debug by printing to a file
void debug(std::string st){
    std::ofstream myfile;
    myfile.open ("debFile.txt",std::ios_base::app);
    myfile << st<<"\n";
    myfile.close();
}


//lembra de mudar o parameterFile do irace_config.xml também
bool isMemetic = true;//gambiarra por enquanto depois refatoramos melhor

void runMemetic(int argc, char *argv[]){
    if(argc == 1){
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
        TabuMemetic taMeAlgorithm(populationSize, numGenerations, mutationRate, crossoverRate);

        // Run the genetic algorithm
        std::vector<int> bestTour = taMeAlgorithm.run(graph);
        std::cout<<"Tour size: "<<bestTour.size()<<std::endl;

        // Calculate and print the cost of the best tour
        TabuMemetic::Individual i{bestTour, 0};
        int bestCost = taMeAlgorithm.calculateTour(0, 0, i);
        if(bestCost < INT_MAX)std::cout << "Best tour cost: " << bestCost << std::endl;
        else std::cout<<"There is no valid tour for this graph :("<<std::endl;

        // Print the best tour
        std::cout << "Best tour: ";
        for (int city : bestTour) {
            std::cout << city << " ";
        }
        std::cout << std::endl;

        return;
    }
    if (argc < 5 || argc > 7)
    {
        std::cout << "The CLI needs the following parameters: " << std::endl;
        std::cout << "1. Max evaluations" << std::endl;
        std::cout << "2. Population size" << std::endl;
        std::cout << "3. Crossover rate" << std::endl;
        std::cout << "4. Mutation rate" << std::endl;
        std::cout << "5. Graph instance (optional)\n" << std::endl;

        std::cout << "Arguments passed (" << argc << ") were: " << std::endl;

        for(int i = 0; i < argc; i++) {
            std::cout << argv[i] << " ";
        }

        std::cout << std::endl;

        return ;
    }
    
    int maxEvaluations = std::atoi(argv[1]);
    int populationSize = std::atoi(argv[2]);
    float crossoverRate = std::atof(argv[3]);
    float mutationRate = std::atof(argv[4]);

    if(argc == 5) {
        // std::cout << "Running full benchmark! This may take a while..." << std::endl;
        // std::cout << std::endl;
        // std::cout << "Max evaluations: " << maxEvaluations << std::endl;
        // std::cout << "Population size: " << populationSize << std::endl;
        // std::cout << "Crossover rate: " << crossoverRate << std::endl;
        // std::cout << "Mutation rate: " << mutationRate << std::endl;

        Benchmark benchmark(
            maxEvaluations,
            populationSize,
            crossoverRate,
            mutationRate
        );

        benchmark.evaluate();
    } else {
        std::string instance = argv[5];
        std::string algorithm = argv[6];
        /*        
        std::cout << "Running irace over the Genetic Improved algorithm\n"
                  << "and instance " << instance << std::endl;
        std::cout << std::endl;
        std::cout << "Max evaluations: " << maxEvaluations << std::endl;
        std::cout << "Population size: " << populationSize << std::endl;
        std::cout << "Crossover rate: " << crossoverRate << std::endl;
        std::cout << "Mutation rate: " << mutationRate << std::endl;
        */
        Benchmark benchmark(
            maxEvaluations,
            populationSize,
            crossoverRate,
            mutationRate
        );

        benchmark.evaluate(instance, algorithm);
    }
}

void runTabu(int argc, char *argv[]){
    if(argc==5){
        int tabuTime = std::atoi(argv[1]);
        int tabuAspirationTime = std::atoi(argv[2]);
        int tabuMaxIter = std::atoi(argv[3]);
        std::string instance = argv[4];
        Benchmark benchmark(
            tabuTime,
            tabuAspirationTime,
            tabuMaxIter
        );

        benchmark.evaluate(instance, "tabu");
        return ;
    }
}

int main(int argc, char *argv[])
{
    if(isMemetic) runMemetic(argc,argv);
    else runTabu(argc,argv);
    
    return 0;
}
