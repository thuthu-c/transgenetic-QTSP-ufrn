#include "../../include/algorithms/trans_qtsp_v2.h"
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

const double uElite = 5.0; 



int TransQTSPV2::numberOfSuccesivesPairs(const std::vector<int>& Pi, const std::vector<int>& Pj) {
    int n = Pi.size();
    if (n == 0) return 0;
    
    int max_v = 0;
    for (int v : Pj) { if (v > max_v) max_v = v; }
    
    std::vector<int> pos(max_v + 1, -1);
    for(int i = 0; i < n; i++) { pos[Pj[i]] = i; }
    
    int missing_pairs = 0;
    for(int i = 0; i < n; i++) {
        int u = Pi[i];
        int v = Pi[(i + 1) % n];
        
        if (u > max_v || pos[u] == -1) { 
            missing_pairs++; 
            continue; 
        }
        
        int p_u = pos[u];
        int v2_next = Pj[(p_u + 1) % n];
        
        if(v != v2_next) {
            missing_pairs++;
        }
    }
    return missing_pairs;
}

std::vector<std::pair<int, int>> TransQTSPV2::generateAllNodesPairs(Individual individual)
{
    std::cout<<"gerando os par tudo " << std::endl;
    std::vector<std::pair<int, int>> pairs;

    for (auto i{0}; i < individual.tour.size() - 1; ++i)
    {
        pairs.push_back(std::make_pair(individual.tour[i], individual.tour[i + 1]));
    }

    return pairs;
}

long int TransQTSPV2::numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj(Individual Pi, Individual Pj)
{
    std::cout<<"n sucessives pairs ??? " << std::endl;
    long int numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj{0};
    std::vector<std::pair<int, int>> pairsPi = generateAllNodesPairs(Pi);
    std::vector<std::pair<int, int>> pairsPj = generateAllNodesPairs(Pj);

    for (auto i : pairsPi)
    {
        const auto search = std::find(pairsPj.begin(), pairsPj.end(), i);
        const auto unsucceeded = pairsPj.end();
        if (search != unsucceeded)
            numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj++;
    }
    return numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj;
}

double TransQTSPV2::normalizeBronkenPairsDistance(Individual Pi, Individual Pj)
{
    std::cout<<"nbpd??? " << std::endl;
    int n = graph->getMaxM();
    return (1.0 / n) * numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj(Pi, Pj);
}

// A sua função normalizada
double TransQTSPV2::dc(Individual& Pi) {
     std::cout<<"sou o dc??? " << std::endl;
     int n_close = 2;

    double resultSum{0};
    for (auto j{1}; j <= n_close; ++j)
    {
        Individual Pj = population[j];
        resultSum += normalizeBronkenPairsDistance(Pi, Pj);
    }
    return (1.0 / n_close) * resultSum;
}


void TransQTSPV2::individualDiversityRank() {
    std::cout<<"ta entrando aqui??? " << std::endl;

    for (auto& ind : population) {
        ind.diversityContribution = dc(ind);
         std::cout<<"diversidade do indi " << ind.diversityContribution << std::endl;
    }

    std::vector<Individual*> copyPopulation;
    for (auto& i : population) copyPopulation.push_back(&i); // Endereços reais e seguros!

    std::sort(copyPopulation.begin(), copyPopulation.end(),
              [](const Individual* a, const Individual* b) {
                  return a->diversityContribution > b->diversityContribution; // Maior Div = Rank 1
              });

    int cont = 0;
    for (auto i : copyPopulation) {
        i->diversityRank = ++cont;
    }
}

void individualCostRank(std::vector<Individual>& population) {
    std::vector<Individual*> copyPopulation;
    for (auto& i : population) copyPopulation.push_back(&i);

    std::sort(copyPopulation.begin(), copyPopulation.end(),
              [](const Individual* a, const Individual* b) {
                  return a->cost < b->cost; 
              });

    int cont = 0;
    for (auto i : copyPopulation) {
        i->costRank = ++cont;
    }
}

void evaluatePopulation(std::vector<Individual>& population) {
    int popSize = population.size();
    for (auto& i : population) {
        i.biasedFitness = i.costRank + (1.0 - (uElite / popSize)) * i.diversityRank;
    }
}

TransQTSPV2::TransQTSPV2(int maxEvaluations, int populationSize, double probT, double stepProb, double plasmidSize) 
    : TransQTSPProbT(maxEvaluations, populationSize, probT, stepProb, plasmidSize) 
{
    this->num_evaluations = maxEvaluations;
    this->populationSize = populationSize; 
    this->probT = probT;
    this->stepProb = stepProb;
    this->plasmidSize = plasmidSize;
    this->numEvaluation = 0;
}

std::vector<Individual> TransQTSPV2::generate_population(Graph *graph){

    std::vector<Individual> population(populationSize);
    int randomPopulationSize = (int)(0.5 * populationSize);

    std::vector<std::vector<int>> randomPopulation, nearestPopulation;
    randomPopulation = this->initializeRandomPopulation(randomPopulationSize, *graph);

    int nearestPopulationSize = this->populationSize - randomPopulationSize;
    nearestPopulation = this->initializeNearestNeighbors(nearestPopulationSize, *graph);

    for (int i = 0; i < randomPopulationSize; i++) {
        population[i].tour = randomPopulation[i];
        population[i].cost = cost(randomPopulation[i]);
    }

    for (int i = 0; i < nearestPopulationSize; i++) {
        population[i + randomPopulationSize].tour = nearestPopulation[i];
        population[i + randomPopulationSize].cost = cost(nearestPopulation[i]);
    }

    return population;
}

std::vector<int> TransQTSPV2::run(Graph& graphInput){
    std::cout<<"entrei no run do v2" << std::endl;
    this->numEvaluation = 0;
    graph = &graphInput;

  
    population = generate_population(graph);

    // Matriz pura que guardará APENAS AS ROTAS do hospedeiro
    std::vector<std::vector<int>> girConverted;

    auto atualizarGirBiased = [&]() {

        individualCostRank(population);
        individualDiversityRank();
        
        evaluatePopulation(population);

        std::vector<Individual*> elites;
        for (auto& i : population) elites.push_back(&i);
        
        std::sort(elites.begin(), elites.end(), [](const Individual* a, const Individual* b){
            return a->biasedFitness < b->biasedFitness;
        });

        girConverted.clear();
        int tam_gir = std::min((int)uElite, (int)elites.size());
        for(int i = 0; i < tam_gir; i++) {
            girConverted.push_back(elites[i]->tour);
        }
    };

    // Chama na Geração 0
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
    int step = 5000; 

    auto ajustarProbsEAtualizarGIR = [&]() {
        probT = std::max(0.2, probT - stepProb);
        probP = 1.0 - probT;
        atualizarGirBiased();
    }; 

    std::uniform_real_distribution<double> dist_prob(0.0, 1.0);

    while(num_evaluations > numEvaluation){

        for(auto& p: population){
            
            if (numEvaluation >= num_evaluations) break;

            if (numEvaluation >= step) {
                std::cout<<"vou mudar o step" << std::endl;
                ajustarProbsEAtualizarGIR();
                step += 5000;
            }

            double chance = dist_prob(engine);
            long long int current_cost = p.cost;

            if(chance <= probP){
                // O Plasmid agora lê perfeitamente a matriz de rotas seguras!
                Plasmid plasmid = generate_plasmid(girConverted);
                 std::cout<<"o plasmideo é: " << std::endl;
                for(auto t : plasmid.genes){
                    std::cout<< t << std::endl;
                }
                std::cout<<"o custo da solucao atual é: " << current_cost<< std::endl;
                std::vector<int> new_solution = m1(plasmid, p.tour.size(), p.tour); 

                long long int new_cost = cost(new_solution); 
                std::cout<<"o custo da nova solucao  é: " << new_cost<< std::endl;
                if(new_cost < current_cost){
                    p.tour = new_solution;
                    p.cost = new_cost;
                    current_cost = new_cost; 
                }
            }else{
                std::vector<int> transposon = transposon_4OPT(p.tour);
                std::cout<<"o transposon é: " << std::endl;
                for(auto t : transposon){
                    std::cout<< t << std::endl;
                }
                long long int transposon_cost = cost(transposon);
                std::cout<<"o custo do transposon é: " << transposon_cost<< std::endl;
                
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

    return best_solution;
}