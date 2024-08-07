#include <iostream>
#include <chrono>
#include <sstream>
#include <fstream>
#include "../../include/data_structures/graph.h"
#include "../../include/benchmark/graph_generator.h"
#include "../../include/algorithms/tsp_solver.h"
#include "../../include/algorithms/nearest_neighborhood.h"
#include "../../include/algorithms/memetic.h"
#include "../../include/algorithms/cheapest_insertion.h"
#include "../../include/algorithms/brute_force.h"
#include "../../include/algorithms/branch_and_bound.h"
#include "../../include/algorithms/tabu.h"
#include "../../include/helpers/graphio.h"
#include "../../include/benchmark/benchmark.h"
#include "../../include/algorithms/genetic_improved.h"
#include "../../include/algorithms/another_genetic.h"

Benchmark::Benchmark(
    int maxEvaluations,
    int populationSize,
    float crossoverRate,
    float mutationRate
)
{
    this->maxEvaluations = maxEvaluations;
    this->populationSize = populationSize;
    this->crossoverRate = crossoverRate;
    this->mutationRate = mutationRate;
}

Benchmark::Benchmark(
    int tabuTime,
    int tabuAspirationTime,
    int tabuMaxIter
)
{
    this->tabuTime = tabuTime;
    this->tabuAspirationTime = tabuAspirationTime;
    this->tabuMaxIter = tabuMaxIter;
}


Benchmark::~Benchmark(){}

// Generates graph instances from 5 vertexes to 24 vertexes
// each instance contains 5 variations.
// It saves the graphs on disk, returning its paths
std::vector<std::string> generateGraphs(int start, int end)
{
    std::vector<std::string> paths;

    for (int j = start; j <= end; j += 1)   
    {
        for (int i = 0; i < 5; i++)
        {
            GraphGenerator graphGenerator(j);
            auto graph = graphGenerator.generate();

            // writing graph on disk if the filename doesn't exists
            GraphIO graphio;
            graphio.setGraph(graph);

            std::ostringstream graphFilename;
            graphFilename << "data/FIS" << j << "-" << i << ".txt";
            std::ifstream infile(graphFilename.str());

            if (!infile.good())
            {
                graphio.write(graphFilename.str());
            }

            paths.push_back(graphFilename.str());
        }
    }

    return paths;
}

int tourLength(std::vector<int> tour, Graph graph)
{
    int tourVal = 0;
    int n = graph.getMaxM();

    tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
    tourVal += graph.custo[tour[n - 1]][tour[0]][tour[1]];

    for (int j = 0; j < (n - 2); j++)
    {
        tourVal += graph.custo[tour[j]][tour[j + 1]][tour[j + 2]];
    }

    return tourVal;
}

void writeResult(
    std::ofstream &file,
    std::string algorithm,
    int execution,
    std::string filename,
    int numVertex,
    long double time,
    std::vector<int> &path,
    int cost)
{
    file << algorithm << ";";
    file << execution << ";";
    file << filename << ";";
    file << numVertex << ";";

    for (int i = 0; i < (int)path.size(); i++)
    {
        file << path[i];
        if (i != (((int)path.size()) - 1))
        {
            file << ",";
        }
    }

    file << ";";
    file << time << ";";
    file << cost << "\n";
}

std::string getAlgorithmName(TspSolver *solver)
{
    if (dynamic_cast<BruteForce *>(solver))
    {
        return "BruteForce";
    }
    else if (dynamic_cast<CheapestInsertion *>(solver))
    {
        return "CheapestInsertion";
    }  else if (dynamic_cast<Memetic*>(solver))
    {
        return "Memetic";
    }
    else if (dynamic_cast<Tabu *>(solver))
    {
        return "Tabu";
    }
    else if(dynamic_cast<BranchAndBound*>(solver)){
        return "Branch and bound";
    }
    else if(dynamic_cast<GeneticImproved*>(solver)){
        return "GeneticImproved";
    }

    return "AnotherGenetic";
}

void run(TspSolver *solver, std::string graphFilename, std::ofstream &file)
{
    std::string solverName = getAlgorithmName(solver);

    GraphIO graphio;
    graphio.read(graphFilename);
    Graph graph = graphio.getGraph();

    for (int i = 1; i <= 30; i++)
    {
        std::cout << "Start running algorithm " << solverName << " for " << graph.getMaxM() << "in execution " << i << std::endl;

        // https://en.cppreference.com/w/cpp/chrono/duration/duration_cast
        auto start = std::chrono::high_resolution_clock::now();
        auto minPath = solver->run(graph); // RUN
        auto end = std::chrono::high_resolution_clock::now();

        int cost = tourLength(minPath, graph);

        long double miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        writeResult(
            file,
            getAlgorithmName(solver),
            i,
            graphFilename,
            graph.getMaxM(),
            miliseconds,
            minPath,
            cost
        );

        std::cout << "End " << solverName << " execution. It took " << miliseconds << " miliseconds" << std::endl;
    }    
}

int Benchmark::evaluate()
{
    std::vector<TspSolver *> algorithms;
    BruteForce *bf = new BruteForce();
    Tabu* tabu = new Tabu(this->tabuTime,this->tabuAspirationTime,this->tabuMaxIter);
    NearestNeighborhood *nb = new NearestNeighborhood();
    CheapestInsertion *ci = new CheapestInsertion();
    BranchAndBound *bnb = new BranchAndBound();

    GeneticImproved *gi = new GeneticImproved(
        this->maxEvaluations,
        this->populationSize,
        this->crossoverRate,
        this->mutationRate
    );

    Memetic *mm = new Memetic(
        this->maxEvaluations,
        this->populationSize,
        this->crossoverRate,
        this->mutationRate
    );

    AnotherGenetic *ag = new AnotherGenetic(
        this->populationSize,
        this->maxEvaluations,
        this->mutationRate,
        this->crossoverRate
    );
    
    // algorithms.push_back(ci);
    // algorithms.push_back(mm);
    // algorithms.push_back(gi);
    // algorithms.push_back(nb);
    // algorithms.push_back(ci);
    algorithms.push_back(ag);
    //algorithms.push_back(tabu);
    //algorithms.push_back(bnb);
    //algorithms.push_back(bf);*/

    std::vector<std::string> graphsPath = generateGraphs(5, 14);

    // add header to csv
    std::ofstream outputFile;
    outputFile.open("result.csv", std::ios::app);
    outputFile << "algorithm;execution;filename;num_vertex;min_path;milisec;cost\n";
    outputFile.close();

    // run algorithms for every graph instances
    for (auto algorithm : algorithms)
    {
        for (std::string graphPath : graphsPath)
        {
            outputFile.open("result.csv", std::ios::app);
            run(algorithm, graphPath, outputFile);
            outputFile.close();
        }
    }

    // running big graphs for big instances (ignoring brute force)
    graphsPath = generateGraphs(50, 50);

    for (auto generated : generateGraphs(75, 75))
    {
        graphsPath.push_back(generated);
    }

    for (auto generated : generateGraphs(100, 100))
    {
        graphsPath.push_back(generated);
    }

    for (auto algorithm : algorithms)
    {
        // ignores brute force
        if (dynamic_cast<BruteForce *>(algorithm))
        {
            continue;
        }

        for (auto g : graphsPath)
        {
            outputFile.open("result.csv", std::ios::app);
            run(algorithm, g, outputFile);
            outputFile.close();
        }
    }

    return -1;
}

int Benchmark::evaluate(std::string instance, std::string algorithmName)
{
    TspSolver* algorithm;

    if(algorithmName.compare("memetic") == 0) {
        std::cout << "memetic" << std::endl;
        algorithm = new Memetic(
            this->maxEvaluations,
            this->populationSize,
            this->crossoverRate,
            this->mutationRate
        );
    } else if (algorithmName.compare("genetic") == 0){
        std::cout << "genetic" << std::endl;
        algorithm = new GeneticImproved(
            this->maxEvaluations,
            this->populationSize,
            this->crossoverRate,
            this->mutationRate
        );
    } else {
        algorithm = new Tabu(this->tabuTime,this->tabuAspirationTime,this->tabuMaxIter);
    }

    GraphIO graphio;
    graphio.read(instance);
    Graph graph = graphio.getGraph();

    auto cost = tourLength(algorithm->run(graph), graph);
    // irace `target-runner` expects this output
    std::cout << cost << std::endl;

    return cost; // RUN
}