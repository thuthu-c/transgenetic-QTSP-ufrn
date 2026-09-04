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
#include "../../include/algorithms/tabu_memetic.h"
#include "../../include/algorithms/hga.h"
#include "../../include/algorithms/trans_qtsp.h"
#include "../../include/algorithms/trans_qtsp_v1.h"
#include "../../include/algorithms/trans_qtsp_v2.h"
#include "../../include/algorithms/trans_qtsp_v3.h"
#include "../../include/algorithms/trans_qtsp_v4.h"

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

Benchmark::Benchmark(
    int maxEvaluations,
    int populationSize
)
{
    this->maxEvaluations = maxEvaluations;
    this->populationSize = populationSize;
}

Benchmark::Benchmark(
    int maxEvaluations,
    int populationSize,
    double plasmidSize
)
{
    this->maxEvaluations = maxEvaluations;
    this->populationSize = populationSize;
    this->plasmidSize = plasmidSize;

}

Benchmark::Benchmark(
            int maxEvaluations,
            int populationSize,
            double probT,
            double stepProb,
            double plasmidSize
        ){
            this->maxEvaluations = maxEvaluations;
            this->populationSize = populationSize;
            this->probT = probT;
            this->stepProb = stepProb;
            this->plasmidSize = plasmidSize;
        }

Benchmark::Benchmark(
    int maxEvaluations,
    int populationSize,
    float crossoverRate,
    float mutationRate,
    double probT,
    double stepProb,
    double plasmidSize
) {
    this->maxEvaluations = maxEvaluations;
    this->populationSize = populationSize;
    this->crossoverRate = crossoverRate;
    this->mutationRate = mutationRate;
    this->probT = probT;
    this->stepProb = stepProb;
    this->plasmidSize = plasmidSize;
}

Benchmark::Benchmark(
    int maxEvaluations,
    int populationSize,
    float crossoverRate,
    float mutationRate,
    double probT,
    double stepProb,
    double plasmidSize,
    int plasmidBank
) {
    this->maxEvaluations = maxEvaluations;
    this->populationSize = populationSize;
    this->crossoverRate = crossoverRate;
    this->mutationRate = mutationRate;
    this->probT = probT;
    this->stepProb = stepProb;
    this->plasmidSize = plasmidSize;
    this->plasmidBank = plasmidBank;
}

Benchmark::Benchmark(
            int maxEvaluations,
            int populationSize,
            double probT,
            double stepProb,
            double plasmidSize,
            int plasmidBank
        ){
            this->maxEvaluations = maxEvaluations;
            this->populationSize = populationSize;
            this->probT = probT;
            this->stepProb = stepProb;
            this->plasmidSize = plasmidSize;
            this->plasmidBank = plasmidBank;
        }

        Benchmark::Benchmark(
            int maxEvaluations,
            int populationSize,
            double probT,
            double stepProb,
            double plasmidSize,
            int plasmidBank,
            double plasmidMin, 
            double plasmidMax
        ){
            this->maxEvaluations = maxEvaluations;
            this->populationSize = populationSize;
            this->probT = probT;
            this->stepProb = stepProb;
            this->plasmidSize = plasmidSize;
            this->plasmidBank = plasmidBank;
            this->plasmidMin = plasmidMin;
            this->plasmidMax = plasmidMax;
        }

        Benchmark::Benchmark(
    int maxEvaluations,
    int populationSize,
    float crossoverRate,
    float mutationRate,
    double probT,
    double stepProb,
    double plasmidSize,
    double plasmidMin,
    double plasmidMax
    ){

    this->maxEvaluations = maxEvaluations;
    this->populationSize = populationSize;
    this->crossoverRate = crossoverRate;
    this->mutationRate = mutationRate;
    this->probT = probT;
    this->stepProb = stepProb;
    this->plasmidSize = plasmidSize;
    this->plasmidMin = plasmidMin;
    this->plasmidMax = plasmidMax;
        
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

bool verificaGir (TransQTSPV4 *solver){
        return solver->taNoGir;
}

bool verificaPop (TransQTSPV4 *solver){
        return solver->taNoPop;
}

void writeResult(
    std::ofstream &file,
    std::string algorithm,
    int execution,
    std::string filename,
    int numVertex,
    long double time,
    std::vector<int> &path,
    int cost,
    std::vector<int> &path_ini,
    int cost_ini,
    bool taNoPop,
    bool taNoGir)
{
    file << algorithm << ";";
    file << execution << ";";
    file << filename << ";";
    file << numVertex << ";";

    for (int i = 0; i < (int)path_ini.size(); i++)
    {
        file << path_ini[i];
        if (i != (((int)path_ini.size()) - 1))
        {
            file << ",";
        }
    }
    file << ";";
    file << cost_ini << ";";
    file << time << ";";

    for (int i = 0; i < (int)path.size(); i++)
    {
        file << path[i];
        if (i != (((int)path.size()) - 1))
        {
            file << ",";
        }
    }

    file << ';' << cost
     << ';' << std::boolalpha << taNoPop
     << ';' << taNoGir << '\n';
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
    else if(dynamic_cast<TabuMemetic*>(solver)){
        return "Tabu Search Memetic";

    }
    else if(dynamic_cast<HGA*>(solver)){
        return "HGA";
    }
     else if(dynamic_cast<TransQTSPV4*>(solver)){
        return "TransQTSPV4";
    }
     else if(dynamic_cast<RemTransp*>(solver)){
        return "RemTransp";
    }
     else if(dynamic_cast<TransQTSPV2*>(solver)){
        return "TransQTSPV2";
    }
    else if(dynamic_cast<TransQTSPProbT*>(solver)){
        return "TransQTSPProbT";
    }
    else if(dynamic_cast<TransQTSP*>(solver)){
        return "TransQTSP";
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
        // std::cout << "Start running algorithm " << solverName << " for " << graph.getMaxM() << "in execution " << i << std::endl;

        // https://en.cppreference.com/w/cpp/chrono/duration/duration_cast
        auto start = std::chrono::high_resolution_clock::now();
        auto minPath = solver->run(graph); // RUN

        bool taNoPop = false;
        bool taNoGir = false;

        if (auto* transSolver = dynamic_cast<TransQTSPV4*>(solver)){
            taNoPop = transSolver->taNoPop;
            taNoGir = transSolver->taNoGir;
        }

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
            cost,
            solver->b_ini_p,
            solver->b_ini_c,
            taNoPop,
            taNoGir
        );

        // std::cout << "End " << solverName << " execution. It took " << miliseconds << " miliseconds" << std::endl;
    }    
    std::cout<<"Done "<<solverName<<" for "<<graphFilename<<std::endl;
}

int Benchmark::evaluate()
{
    std::vector<TspSolver *> algorithms;
//     BruteForce *bf = new BruteForce();
    Tabu* tabu = new Tabu(this->tabuTime,this->tabuAspirationTime,this->tabuMaxIter);
//     NearestNeighborhood *nb = new NearestNeighborhood();
//     CheapestInsertion *ci = new CheapestInsertion();
//     BranchAndBound *bnb = new BranchAndBound();
// 
//     GeneticImproved *gi = new GeneticImproved(
//         this->maxEvaluations,
//         this->populationSize,
//         this->crossoverRate,
//         this->mutationRate
//     );
// 
     Memetic *mm = new Memetic(
        this->maxEvaluations,
        this->populationSize,
        this->crossoverRate,
        this->mutationRate
    );

    // AnotherGenetic *agls = new AnotherGenetic(
    //     this->populationSize,
    //     this->maxEvaluations,
    //     this->mutationRate,
    //     this->crossoverRate,
    //     true
    // );
//     AnotherGenetic *ag = new AnotherGenetic(
//         this->populationSize,
//         this->maxEvaluations,
//         this->mutationRate,
//         this->crossoverRate,
//         false
//     );
    // TabuMemetic *tm = new TabuMemetic(
    //     this->populationSize,
    //     this->maxEvaluations,
    //     this->mutationRate,
    //     this->crossoverRate
    // );

    HGA* hgaAlgo = new HGA( 
    this-> maxEvaluations,this->populationSize, this->crossoverRate, this->mutationRate); 

    TransQTSP* trans = new TransQTSP(this-> maxEvaluations, this->populationSize, this->plasmidSize); 
    TransQTSPProbT* transV1 = new TransQTSPProbT(this-> maxEvaluations, this->populationSize, this->probT, this->stepProb, this->plasmidSize);
    TransQTSPV2* transV2 = new TransQTSPV2(this-> maxEvaluations, this->populationSize, this->probT, this->stepProb, this->plasmidSize, this->plasmidBank);
    RemTransp* remTransp = new RemTransp(this-> maxEvaluations, this->populationSize, this->probT, this->stepProb, this->plasmidSize, this->plasmidBank);
    TransQTSPV4* transV4 = new TransQTSPV4(this-> maxEvaluations, this->populationSize, this->probT, this->stepProb, this->plasmidSize, this->plasmidBank, this->plasmidMin, this->plasmidMax);
    // algorithms.push_back(ci);
    // algorithms.push_back(mm);
    // algorithms.push_back(gi);
    // algorithms.push_back(nb);
    // algorithms.push_back(ci);
    // algorithms.push_back(tabu);
    algorithms.push_back(hgaAlgo);
    // algorithms.push_back(bnb);
    // algorithms.push_back(bf);
    // algorithms.push_back(ag);
    // algorithms.push_back(agls);
    // algorithms.push_back(tm);
    // algorithms.push_back(trans);
    // algorithms.push_back(transV1);
    // algorithms.push_back(transV2);
    //algorithms.push_back(remTransp);
    algorithms.push_back(transV4);
    //  std::cout<< "eu sou o transv1  " << transV1->getProbT() << std::endl; 
    // std::vector<std::string> graphsPath = generateGraphs(5, 14);

    // add header to csv
    std::ofstream outputFile;
    outputFile.open("result.csv", std::ios::app);
    outputFile << "algorithm;execution;filename;num_vertex;initial_path;initial_cost;milisec;min_path;cost;taNoPop;taNoGir\n";
    outputFile.close();
// 
//     // run algorithms for every graph instances
    // for (auto algorithm : algorithms)
    // {
    //     for (std::string graphPath : graphsPath)
    //     {
    //         outputFile.open("result.csv", std::ios::app);
    //         run(algorithm, graphPath, outputFile);
    //         outputFile.close();
    //     }
    // }

    // running big graphs for big instances (ignoring brute force)
    std::vector<std::string> graphsPath = generateGraphs(50, 50);


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
        // std::cout<< "o algoritmo é " << algorithm << std::endl; 
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
        // std::cout << "genetic" << std::endl;
        algorithm = new GeneticImproved(
            this->maxEvaluations,
            this->populationSize,
            this->crossoverRate,
            this->mutationRate
        );
    } else if (algorithmName.compare("hga") == 0){
        std::cout << "hga" << std::endl;
        algorithm = new HGA( 
            this-> maxEvaluations,
            this->populationSize,
            this->crossoverRate,
            this->mutationRate
        );
    
    }else if(algorithmName.compare("transV4") == 0){
        // std::cout << "remTransp" << std::endl;
        algorithm = new TransQTSPV4( 
            this-> maxEvaluations,
            this->populationSize,
            this->probT,
            this->stepProb,
            this->plasmidSize,
            this->plasmidBank,
            this->plasmidMin, 
            this->plasmidMax
        );
    }
    else if(algorithmName.compare("remTransp") == 0){
        // std::cout << "remTransp" << std::endl;
        algorithm = new RemTransp( 
            this-> maxEvaluations,
            this->populationSize,
            this->probT,
            this->stepProb,
            this->plasmidSize,
            this->plasmidBank
        );
    }
     else if(algorithmName.compare("transQTSPV2") == 0){
        std::cout << "transgeneticv2" << std::endl;
        algorithm = new TransQTSPV2( 
            this-> maxEvaluations,
            this->populationSize,
            this->probT,
            this->stepProb,
            this->plasmidSize,
            this->plasmidBank
        );
    }
    else if(algorithmName.compare("transQTSPProbT") == 0){
        std::cout << "transgenetic probT" << std::endl;
        algorithm = new TransQTSPProbT( 
            this-> maxEvaluations,
            this->populationSize,
            this->probT,
            this->stepProb,
            this->plasmidSize
        );
    }
     else if(algorithmName.compare("transQTSP") == 0){
        std::cout << "transgenetic" << std::endl;
        algorithm = new TransQTSP( 
            this-> maxEvaluations,
            this->populationSize,
            this->plasmidSize
        );

    }
     else if(algorithmName.compare("all_algorithms") == 0){
        std::cout << "todos os algoritmos" << std::endl;
        algorithm = new RemTransp( 
            this-> maxEvaluations,
            this->populationSize,
            this->plasmidSize,
            this->stepProb,
            this->plasmidSize,
            this->plasmidBank
        );

    }
     else {
        algorithm = new Tabu(this->tabuTime,this->tabuAspirationTime,this->tabuMaxIter);
        std::cout << "Criando um tabu" << std::endl;
    }

    GraphIO graphio;
    // std::cout << "A INSTÂNCIA É: " <<instance<< std::endl;
    graphio.read(instance);
    Graph graph = graphio.getGraph();

    auto cost = tourLength(algorithm->run(graph), graph);
    // irace `target-runner` expects this output
    std::cout << cost << std::endl;
    delete algorithm; 
    return cost; // RUN
}