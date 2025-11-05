#include <iostream>
#include "../include/benchmark/benchmark.h"
#include<fstream>
using namespace std;

#include "../include/algorithms/tabu_memetic.h"
#include "../include/algorithms/hga.h"
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
bool isMemetic = false;//gambiarra por enquanto depois refatoramos melhor

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

        benchmark.evaluate();
        return ;
    }
}

int main(int argc, char *argv[])
{
    // if(isMemetic) runMemetic(argc,argv);
    // else runTabu(argc,argv);
    Graph graph(5); // Assuming 5 nodes for this example

graph.addEdge(0, 0, 0, 0);
graph.addEdge(0, 0, 1, 0);
graph.addEdge(0, 0, 2, 0);
graph.addEdge(0, 0, 3, 0);
graph.addEdge(0, 0, 4, 0);
graph.addEdge(0, 1, 0, 0);
graph.addEdge(0, 1, 1, 0);
graph.addEdge(0, 1, 2, 4779);
graph.addEdge(0, 1, 3, 6711);
graph.addEdge(0, 1, 4, 5388);
graph.addEdge(0, 2, 0, 0);
graph.addEdge(0, 2, 1, 4798);
graph.addEdge(0, 2, 2, 0);
graph.addEdge(0, 2, 3, 1091);
graph.addEdge(0, 2, 4, 5409);
graph.addEdge(0, 3, 0, 0);
graph.addEdge(0, 3, 1, 5726);
graph.addEdge(0, 3, 2, 5876);
graph.addEdge(0, 3, 3, 0);
graph.addEdge(0, 3, 4, 4805);
graph.addEdge(0, 4, 0, 0);
graph.addEdge(0, 4, 1, 6327);
graph.addEdge(0, 4, 2, 4961);
graph.addEdge(0, 4, 3, 6410);
graph.addEdge(0, 4, 4, 0);
graph.addEdge(1, 0, 0, 0);
graph.addEdge(1, 0, 1, 0);
graph.addEdge(1, 0, 2, 5962);
graph.addEdge(1, 0, 3, 9527);
graph.addEdge(1, 0, 4, 7263);
graph.addEdge(1, 1, 0, 0);
graph.addEdge(1, 1, 1, 0);
graph.addEdge(1, 1, 2, 0);
graph.addEdge(1, 1, 3, 0);
graph.addEdge(1, 1, 4, 0);
graph.addEdge(1, 2, 0, 4798);
graph.addEdge(1, 2, 1, 0);
graph.addEdge(1, 2, 2, 0);
graph.addEdge(1, 2, 3, 2097);
graph.addEdge(1, 2, 4, 6415);
graph.addEdge(1, 3, 0, 5726);
graph.addEdge(1, 3, 1, 0);
graph.addEdge(1, 3, 2, 5249);
graph.addEdge(1, 3, 3, 0);
graph.addEdge(1, 3, 4, 4178);
graph.addEdge(1, 4, 0, 6327);
graph.addEdge(1, 4, 1, 0);
graph.addEdge(1, 4, 2, 5275);
graph.addEdge(1, 4, 3, 6724);
graph.addEdge(1, 4, 4, 0);
graph.addEdge(2, 0, 0, 0);
graph.addEdge(2, 0, 1, 5962);
graph.addEdge(2, 0, 2, 0);
graph.addEdge(2, 0, 3, 9369);
graph.addEdge(2, 0, 4, 7105);
graph.addEdge(2, 1, 0, 4779);
graph.addEdge(2, 1, 1, 0);
graph.addEdge(2, 1, 2, 0);
graph.addEdge(2, 1, 3, 7736);
graph.addEdge(2, 1, 4, 6413);
graph.addEdge(2, 2, 0, 0);
graph.addEdge(2, 2, 1, 0);
graph.addEdge(2, 2, 2, 0);
graph.addEdge(2, 2, 3, 0);
graph.addEdge(2, 2, 4, 0);
graph.addEdge(2, 3, 0, 5876);
graph.addEdge(2, 3, 1, 5249);
graph.addEdge(2, 3, 2, 0);
graph.addEdge(2, 3, 3, 0);
graph.addEdge(2, 3, 4, 253);
graph.addEdge(2, 4, 0, 4961);
graph.addEdge(2, 4, 1, 5275);
graph.addEdge(2, 4, 2, 0);
graph.addEdge(2, 4, 3, 8440);
graph.addEdge(2, 4, 4, 0);
graph.addEdge(3, 0, 0, 0);
graph.addEdge(3, 0, 1, 9527);
graph.addEdge(3, 0, 2, 9369);
graph.addEdge(3, 0, 3, 0);
graph.addEdge(3, 0, 4, 2523);
graph.addEdge(3, 1, 0, 6711);
graph.addEdge(3, 1, 1, 0);
graph.addEdge(3, 1, 2, 7736);
graph.addEdge(3, 1, 3, 0);
graph.addEdge(3, 1, 4, 3776);
graph.addEdge(3, 2, 0, 1091);
graph.addEdge(3, 2, 1, 2097);
graph.addEdge(3, 2, 2, 0);
graph.addEdge(3, 2, 3, 0);
graph.addEdge(3, 2, 4, 5642);
graph.addEdge(3, 3, 0, 0);
graph.addEdge(3, 3, 1, 0);
graph.addEdge(3, 3, 2, 0);
graph.addEdge(3, 3, 3, 0);
graph.addEdge(3, 3, 4, 0);
graph.addEdge(3, 4, 0, 6410);
graph.addEdge(3, 4, 1, 6724);
graph.addEdge(3, 4, 2, 8440);
graph.addEdge(3, 4, 3, 0);
graph.addEdge(3, 4, 4, 0);
graph.addEdge(4, 0, 0, 0);
graph.addEdge(4, 0, 1, 7263);
graph.addEdge(4, 0, 2, 7105);
graph.addEdge(4, 0, 3, 2523);
graph.addEdge(4, 0, 4, 0);
graph.addEdge(4, 1, 0, 5388);
graph.addEdge(4, 1, 1, 0);
graph.addEdge(4, 1, 2, 6413);
graph.addEdge(4, 1, 3, 3776);
graph.addEdge(4, 1, 4, 0);
graph.addEdge(4, 2, 0, 5409);
graph.addEdge(4, 2, 1, 6415);
graph.addEdge(4, 2, 2, 0);
graph.addEdge(4, 2, 3, 5642);
graph.addEdge(4, 2, 4, 0);
graph.addEdge(4, 3, 0, 4805);
graph.addEdge(4, 3, 1, 4178);
graph.addEdge(4, 3, 2, 253);
graph.addEdge(4, 3, 3, 0);
graph.addEdge(4, 3, 4, 0);
graph.addEdge(4, 4, 0, 0);
graph.addEdge(4, 4, 1, 0);
graph.addEdge(4, 4, 2, 0);
graph.addEdge(4, 4, 3, 0);
graph.addEdge(4, 4, 4, 0);

    HGA hgaAlgo(10);
    HGA::Individual individuo_teste;
    HGA::Individual individuo_teste_dois;
    hgaAlgo.setGraph(graph);

    int heuristic = hgaAlgo.chooseRemovalHeuristic();
    std::cout<<"A heurística escolhida foi: " << heuristic << std::endl;
    
    individuo_teste = hgaAlgo.createIndividuals();
    individuo_teste_dois = hgaAlgo.createIndividuals();
    std::vector<HGA::Individual> population;
    hgaAlgo.initializePopulation(graph);

    for(auto p : hgaAlgo.getPopulation()){
        std::cout << "O custo do indivíduo é: " <<p.cost << std::endl;
        std::cout << "O custo do indivíduo calculado é: " <<hgaAlgo.cost(p.tour) << std::endl;
        std::cout << "A diversidade é: " << hgaAlgo.fd(p) << std::endl; 
        
        for(auto v : p.tour){
            std::cout << "vertice populacao" << v << std::endl; 
        }
        
        std::cout << "acabou a tour da populacao " << std::endl; 
    }
    

    // 

    return 0;
}
