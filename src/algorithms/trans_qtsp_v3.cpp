#include "../../include/algorithms/trans_qtsp_v3.h"
#include "../../include/algorithms/cheapest_insertion.h"
#include "../../include/algorithms/hga.h"
#include <numeric>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <string>
#include <vector>
#include <utility>
#include "../../include/helpers/random.h"


RemTransp::RemTransp(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize) 
    : TransQTSPV2(maxEvaluations, populationSize, probT, stepProb, plasmidSize) 
{
    this->num_evaluations = maxEvaluations;
    this->populationSize = populationSize; 
    this->probT = probT;
    this->stepProb = stepProb;
    this->plasmidSize = plasmidSize;
    this->numEvaluation = 0;
}


// versão 3: definição de novo transposon:
// remTransp: escolhe trecho para destruir e reconstruir com cheapest insertion,
// quando o transposon for escolhido, na v3, aplica remTransp seguido de 4optTransp

// ruin and recreate
std::vector<int> RemTransp::transposon_4OPT(const std::vector<int>& tour){

    HGA::Individual indi;
    indi.tour = tour;
    indi.cost = cost(tour);

    HGA hga;   

    //  std::cout<<"A solucao antes de arruinar eh: " << std::endl;
    // for(auto r : indi.tour ) std::cout<< r << " ";
    // std::cout<<std::endl; 


    //vertices que vao ser removidos da solucao original
    std::vector<int> vertexToBeRemoved = hga.ruin(indi);
    // std::cout<<"O custo da solucao arruinada eh: " << cost(indi.tour)<< std::endl; 
    // std::cout<<"A solucao arruinada eh: " << std::endl;
    // for(auto r : indi.tour ) std::cout<< r << " ";
    // std::cout<<std::endl; 
    // recebe a solucao
    // escolhe qual trecho vai ser destruido (o mais custoso) 
    // reconstroi o trecho com cheapeast insertion
    std::vector<int> solution_recreated;

    solution_recreated = cheapest_insertion(indi.tour, vertexToBeRemoved, *graph);

    //  std::cout<<"O custo da solucao recreada eh: " << cost(solution_recreated)<< std::endl; 
    // std::cout<<"A solucao recreada eh: " << std::endl;
    // for(auto r : solution_recreated ) std::cout<< r << " ";
    // std::cout<<std::endl;
    


    // manda a solucao para o transposon4opt

    return best4opt(solution_recreated);

}
int RemTransp::eval(std::vector<int> &tour, Graph &graph)
{
    int tourVal = 0;
    int n = tour.size();

    tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
    tourVal += graph.custo[tour[n - 1]][tour[0]][tour[1]];

    for (int j = 0; j < (n - 2); j++)
    {
        tourVal += graph.custo[tour[j]][tour[j + 1]][tour[j + 2]];
    }
    numEvaluation++;

    return tourVal;
}


std::vector<int> RemTransp::cheapest_insertion(std::vector<int> &tour, std::vector<int> vertexToBeInserted, Graph &graph){

    while(!vertexToBeInserted.empty()){
        int bestInsertionCost = INT_MAX;
        int bestInsertionVertex = -1;
        int bestInsertionIndex = -1;

        for (int k : vertexToBeInserted) {
            for (auto i{0u}; i < tour.size(); i++) {
                std::vector<int> tmpTour = tour;
                tmpTour.insert(tmpTour.begin() + i, k);

                int localEval = eval(tmpTour, graph);

                if (localEval < bestInsertionCost) {
                    bestInsertionCost = localEval;
                    bestInsertionVertex = k;
                    bestInsertionIndex = i;
                }
            }
        }

        tour.insert(tour.begin() + bestInsertionIndex, bestInsertionVertex);

        vertexToBeInserted.erase(std::remove(vertexToBeInserted.begin(), vertexToBeInserted.end(), bestInsertionVertex), vertexToBeInserted.end());
    }

    return tour;

}
