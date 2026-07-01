/**
 * Implementation of TransQTSP algorithm, the algorithm
 * @author Thuanny Carvalho Rolim de Albuquerque
 * @date May 2nd, 2026
 * @file transQTSP.h
 */ 

#ifndef _TRANSQTSP_H_
#define _TRANSQTSP_H_

#include "../algorithms/tsp_solver.h" 
#include "../data_structures/graph.h"
#include <vector>
#include <algorithm>
#include <random>

class TransQTSP : public TspSolver
{ 
public: 
    TransQTSP(
        int maxEvaluations,
        int populationSize,
        double plasmidSize){
        this-> num_evaluations= maxEvaluations;
        this->populationSize = populationSize;
        this->plasmidSize = plasmidSize;
        this->numEvaluation = 0;
        this->numBestIndividuals = 50;
        }   

~TransQTSP()
{
}

struct Plasmid {
    std::vector<int> genes; // uma sequencia de cidades, gerada a partir de algum cromossomo do GIR
    double fitness_gain;    // quanto do custo que se reduz com a transcrição do plasmídeo
    }; 
    
long long int cost(const std::vector<int>& tour);

protected: 
    Graph *graph;
    int numEvaluation;
    int populationSize;
    double plasmidSize;
    int num_evaluations;
    int numBestIndividuals;

    
    

    std::mt19937 g;

    std::vector<int> run(Graph& graphInput);

    std::vector<std::vector<int>> gir; 
    std::vector<std::vector<int>> population;

    void generate_gir(Graph *graph);

    std::vector<int> getNearestNeighbors(int cityIndex, Graph &graph, int neighborNumber);
    std::vector<std::vector<int>> initializeNearestNeighbors(int populationSize, Graph &graph);
    int eval(std::vector<int> &tour, Graph &graph);
    std::vector<int> greedyPermuting(std::vector<int> &seedIndividual, Graph &graph, int n);
    std::vector<std::vector<int>> initializeRandomPopulation(int populationSize, Graph &graph);


    void generate_population_random(Graph *graph);
    std::vector<std::vector<int>> generate_population(Graph *graph);

    std::vector<int> getVertex();


    
    std::vector<int> m1 (const Plasmid &p, int tamanho_solucao, const std::vector<int> &solution);
    Plasmid generate_plasmid(std::vector<std::vector<int>>  &gir);
    std::vector<int> best4opt(const std::vector<int>& cromossome);
    long long int D2O(int i, int j, const std::vector<int>& tour);
    std::vector<int> fourOptMove(int i1, int i2, int j1, int j2, const std::vector<int>& tour);
    std::vector<int> transposon_4OPT(const std::vector<int>& individual);

};
#endif