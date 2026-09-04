#include "../../include/algorithms/trans_qtsp_v4.h"
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


TransQTSPV4::TransQTSPV4(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize, int plasmidBank, double plasmidMin, double plasmidMax) 
    : RemTransp(maxEvaluations, populationSize, probT, stepProb, plasmidSize, plasmidBank)
{
    this->num_evaluations = maxEvaluations;
    this->populationSize = populationSize; 
    this->probT = probT;
    this->stepProb = stepProb;
    this->plasmidSize = plasmidSize;
    this->numEvaluation = 0;
    this->plasmidBank=10;
    this->plasmidMin = plasmidMin;
    this->plasmidMax = plasmidMin + plasmidMax; 
}

 TransQTSPV4::TransQTSPV4(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize, int plasmidBank) 
    : RemTransp(maxEvaluations, populationSize, probT, stepProb, plasmidSize, plasmidBank)
 {
    this->num_evaluations = maxEvaluations;
    this->populationSize = populationSize; 
    this->probT = probT;
    this->stepProb = stepProb;
    this->plasmidSize = plasmidSize;
    this->numEvaluation = 0;
    this->plasmidBank=plasmidBank;
 }

std::vector<int> TransQTSPV4::run(Graph& graphInput){
    // std::cout<<"entrei no run do v2" << std::endl;
    this->numEvaluation = 0;
    graph = &graphInput;

  
    population = generate_population(graph);
    std::vector<std::vector<int>> girConverted;

    // isso aqui pode ser um parametro depois...
    std::vector<Plasmid> plasmidBank; 
    auto contador{0};
    int step = 10000; 
    int bankSize = 10;

    auto atualizarGirBiased = [&]() {

        individualCostRank(population);
        individualDiversityRank();
        
        evaluatePopulation(population);
        

        // a escolha dos elites eh baseada no biased fitness 
        std::vector<Individual*> elites;
        for (auto& i : population) elites.push_back(&i);
        
        // a ordenacao do gir, eh baseada no custo (funcao objetivo)
        std::sort(elites.begin(), elites.end(), [](const Individual* a, const Individual* b){
            return a->cost < b->cost;
        });

        girConverted.clear();
        int tam_gir = std::min((int)uElite, (int)elites.size());
        for(int i = 0; i < tam_gir; i++) {
            girConverted.push_back(elites[i]->tour);
        }

        plasmidBank.clear();
        contador++;
        // std::cout<< "atualizando o banco de plasmideos "<< contador << "vezes" << std::endl;
        std::uniform_real_distribution<double> distribution_plasmid(this->plasmidMin, this->plasmidMax);
        
       
        if(!girConverted.empty()){
            for(auto i{0}; i < bankSize; ++i){
                plasmidSize =  distribution_plasmid(engine);
                plasmidBank.push_back(generate_plasmid(girConverted, plasmidSize));
            }

            // std::cout<< "o banco de plasmideo eh: " << std::endl;

            // for(auto p : plasmidBank){
            //     for(auto v :p.genes){
            //         std::cout << v << " ";
            //     }
            //     std::cout << std::endl;
            // }
        }
    };


    atualizarGirBiased();


    std::vector<int> best_solution = population.begin()->tour;
    long long int best_cost = population.begin()->cost;

    for(const auto& p : population) {
        if(p.cost < best_cost) {
            best_cost = p.cost;
            best_solution = p.tour;
        }
    }

    double probP = 1.0 - probT;
    
    auto ajustarProbsEAtualizarGIR = [&]() {
        probT = std::max(0.2, probT - stepProb);
        probP = 1.0 - probT;
    }; 

    std::uniform_real_distribution<double> dist_prob(0.0, 1.0);
    

    while(num_evaluations > numEvaluation){

        for(auto& p: population){
            
            if (numEvaluation >= num_evaluations) break;

            if (numEvaluation >= step) {
           
                ajustarProbsEAtualizarGIR();
                step += 1000;
            }

            // verificar q se deixar maior ou igual vao ser feitas varias atualizacoes
            //atualizo o step?
            if(numEvaluation >= (step/2)){
                atualizarGirBiased();
            } 

            double chance = dist_prob(engine);
            long long int current_cost = p.cost;
            
            if(chance <= probP){
           

                //plasmideo foi escolhido!
              

                long long int best_candidate_cost = LLONG_MAX;
                std::vector<int> best_candidate_solution;
                // std::cout<< "a solucao antes do plasmideo: " << std::endl;
                    // for(auto b : p.tour) std::cout<< b << " ";
                    // std::cout<<std::endl;
                    // std::cout<< "o custo eh " << current_cost << std::endl; 

                std::uniform_int_distribution<int> distribution(0, bankSize);
                
                std::vector<Plasmid> choosedPlasmids;

           

                 for(auto i{0}; i < 3; ++i){

                     int randIndex = distribution(engine) % bankSize;
           
                    choosedPlasmids.emplace_back(plasmidBank[randIndex]);
                 }


                for(const auto& plasmid : choosedPlasmids){
                 

                    // eh gerada todas as solucoes com o banco de plasmideos
                    std::vector<int> candidate_solution = m1(plasmid, p.tour.size(), p.tour);

                    //pegou a solucao da populacao e fez a transcricao do plasmideo
                    long long int candidate_cost = cost(candidate_solution);
                    // std::cout<< "o plasmideo candidato custa: " << candidate_cost << std::endl;
                    // std::cout<< "o melhor plasmideo custa: " << best_candidate_cost << std::endl;

                    if(candidate_cost < best_candidate_cost) {
                        best_candidate_cost = candidate_cost;
                        best_candidate_solution = candidate_solution;
                    }

                    // o best_candidate_solution eh a solucao com a melhor transcricao de plasmideo
                    // o best_candidate_cost tem o custo da solucao ja com a transcricao do plasmideo

                }
                

                //current_cost tem o custo da solucao inicial da populacao sem a transcricao do plasmideo
                if(best_candidate_cost < current_cost){
                    p.tour = best_candidate_solution;
                    p.cost = best_candidate_cost;
                    current_cost = best_candidate_cost; 

                    // std::cout<< "a solucao depois do plasmideo: " << std::endl;
                    // for(auto b : best_candidate_solution) std::cout<< b << " ";
                    // std::cout<<std::endl;
                    // std::cout<< "o custo eh " << best_candidate_cost << std::endl;
                }

                if(current_cost < best_cost) {
                best_solution = p.tour;
                best_cost = current_cost;
            }

            // std::cout<< "a melhor solucao pos plasmideo eh: " << std::endl;
            // std::cout << "com custo " << best_cost << std::endl; 

            // for(auto b : best_solution) std::cout<< b << " ";
            // std::cout<< std::endl;

            }else{
               
                std::vector<int> transposon = transposon_4OPT(p.tour);
                // std::cout<<"o transposon é: " << std::endl;
                // for(auto t : transposon){
                //     std::cout<< t << std::endl;
                // }
                long long int transposon_cost = cost(transposon);
                // std::cout<<"o custo do transposon é: " << transposon_cost<< std::endl;
                
                if(transposon_cost < current_cost){
                    p.tour = transposon;
                    p.cost = transposon_cost;
                    current_cost = transposon_cost; 
                }
            }
            
            if(current_cost < best_cost) {
                best_solution = p.tour;
                best_cost = current_cost;
            }
        }
    }

    int gir_best_cost = cost(girConverted[0]); 

    if(gir_best_cost < best_cost){
        best_solution = girConverted[0];
        best_cost = gir_best_cost; 
        taNoGir = true;
    }else taNoPop = true;

    // std::cout << "ta no gir? " << taNoGir << " ou ta no pop? " << taNoPop << std::endl;


    return best_solution;

}