#include "../../include/algorithms/trans_qtsp_v1.h"
#include <numeric>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <string>
#include <vector>
#include "../../include/helpers/random.h"


const int gir_size = 100;

TransQTSPProbT::TransQTSPProbT(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize) 
    : TransQTSP(maxEvaluations, populationSize, plasmidSize) 
{
    this->num_evaluations = maxEvaluations;
    this->populationSize = populationSize; 
    this->probT = probT;
    this->stepProb = stepProb;
    this->plasmidSize = plasmidSize;
    this->numEvaluation = 0;
    this->numBestIndividuals = 50;
}

std::vector<int> TransQTSPProbT::run(Graph& graphInput){
    this->numEvaluation = 0;

    graph = &graphInput;
    int vertex_q = graph->getNumVertex();

    // eu preciso pegar esse grafo e gerar os ciclos, caminhos,
    // árvores e cromossosmos para o GIR
    generate_gir(graph);

    population = generate_population(graph);

    std::vector<int> best_solution = *population.begin();
    long long int best_cost = cost(best_solution);

    double probP = 1.0 - probT;
    
    int step = 5000; 

    auto ajustarProbs = [&]() {
        probT = std::max(0.2, probT - stepProb);
        probP = 1.0 - probT;
    }; 

    std::uniform_real_distribution<double> dist_prob(0.0, 1.0);

    while(num_evaluations > numEvaluation){

        // Verifica a condição de step no início de cada geração do while
        if (numEvaluation >= step) {
            ajustarProbs();
            step += 5000;
        }


        // gerar um agente transgenético
        // plasmídeo e transposon

        //ter um número de probailidade para o plasmideo
        for(auto& p: population){

            if (numEvaluation >= step) {
                ajustarProbs();
                step += 5000;
            }

            double chance = dist_prob(engine);

            long long int current_cost = cost(p);

            // std::cout<< "a chance é " << chance << "\n";

            if(chance <= probP){
                Plasmid plasmid;
                plasmid = generate_plasmid(gir, plasmidSize);
                std::vector<int> new_solution = m1(plasmid, p.size(), p); 

                long long int new_cost = cost(new_solution); 
                if(new_cost < current_cost){
                    p = new_solution;
                    current_cost = new_cost; 
                    gir.emplace_back(p);
                }
            }else{
                std::vector<int> transposon;
                transposon =transposon_4OPT(p);
                long long int transposon_cost = cost(transposon);
                if(transposon_cost < current_cost){
                    p = transposon;
                    current_cost = transposon_cost; 
                    gir.emplace_back(p);
                }
            }
            
            if(current_cost < best_cost) {
                best_solution = p;
                best_cost = current_cost;
            }

        }
    }

    return best_solution;

}


