#include "../../include/algorithms/tabu_memetic.h"
#include <numeric>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

TabuMemetic::TabuMemetic(int popSize, int maxEval, double mutRate, double crossRate)
    : populationSize(popSize), maxEvaluations(maxEval), mutationRate(mutRate) , crossoverRate(crossRate){
        std::random_device rd;
        std::mt19937 k(rd());
        g = k;
    }

TabuMemetic::~TabuMemetic() {}

void TabuMemetic::initializePopulation() {
    population.clear();
    b_ini_c = INT_MAX;
    //cout<<"beginning pop of "<<populationSize<<"\n";
    for (int i = 0; i < populationSize; ++i) {
        //cout<<i<<"th individual\n";
        Individual individual;
        int n = graph->getMaxM();
        std::vector<int> solution;
        std::vector<std::tuple<int, int, int>> triples;
        std::vector<bool> visited(n, false);

        // Collect all valid triples from the graph
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < n; ++k) {
                    if (graph->custo[i][j][k] > 0) {
                        triples.push_back(std::make_tuple(i, j, k));
                    }
                    else graph->custo[i][j][k] = INT_MAX;
                }
            }
        }

        // //cout<<"got all triples\n";

        // Random number generator
        // std::random_device rd;
        // std::mt19937 gen(rd());

        // Shuffle triples to introduce randomness
        std::shuffle(triples.begin(), triples.end(), g);

        // Start with a random triple
        auto startTriple = triples.back();
        triples.pop_back();
        solution.push_back(std::get<0>(startTriple));
        solution.push_back(std::get<1>(startTriple));
        solution.push_back(std::get<2>(startTriple));
        visited[std::get<0>(startTriple)] = true;
        visited[std::get<1>(startTriple)] = true;
        visited[std::get<2>(startTriple)] = true;
        // //cout<<"beginning solution construction\n";
        // Continue constructing the solution
        while (solution.size() < (unsigned long long int)n) {
            std::vector<std::tuple<int, int, int>> restrictedCandidateList;

            // Create the Restricted Candidate List (RCL) with triples that connect to the current tour
            for (auto triple : triples) {
                int a = std::get<0>(triple);
                int b = std::get<1>(triple);
                int c = std::get<2>(triple);

                if (visited[a] + visited[b] + visited[c] == 2) {
                    restrictedCandidateList.push_back(triple);
                }
            }

            // Shuffle the RCL to introduce randomness
            std::shuffle(restrictedCandidateList.begin(), restrictedCandidateList.end(), g);

            // Select a random triple from the RCL
            if (!restrictedCandidateList.empty()) {
                auto selectedTriple = restrictedCandidateList.back();
                restrictedCandidateList.pop_back();

                // Add the unvisited city in the selected triple to the solution
                if (!visited[std::get<0>(selectedTriple)]) {
                    solution.push_back(std::get<0>(selectedTriple));
                    visited[std::get<0>(selectedTriple)] = true;
                } else if (!visited[std::get<1>(selectedTriple)]) {
                    solution.push_back(std::get<1>(selectedTriple));
                    visited[std::get<1>(selectedTriple)] = true;
                } else if (!visited[std::get<2>(selectedTriple)]) {
                    solution.push_back(std::get<2>(selectedTriple));
                    visited[std::get<2>(selectedTriple)] = true;
                }

                // Remove the selected triple from the triples list
                triples.erase(std::remove(triples.begin(), triples.end(), selectedTriple), triples.end());
            } else {
                // If RCL is empty (no connected unvisited triples), restart with a new random triple
                std::shuffle(triples.begin(), triples.end(), g);
                auto newTriple = triples.back();
                triples.pop_back();
                if (!visited[std::get<0>(newTriple)]) {
                    solution.push_back(std::get<0>(newTriple));
                    visited[std::get<0>(newTriple)] = true;
                }
                if (!visited[std::get<1>(newTriple)]) {
                    solution.push_back(std::get<1>(newTriple));
                    visited[std::get<1>(newTriple)] = true;
                }
                if (!visited[std::get<2>(newTriple)]) {
                    solution.push_back(std::get<2>(newTriple));
                    visited[std::get<2>(newTriple)] = true;
                }
            }
        }
        // //cout<<"got em\n";

        // Make sure the solution is a valid circular tour
        if (solution.size() > (unsigned long long int)n) {
            solution.resize(n);
        }
        //performing local search
        Individual currentTour{solution, 0};

           
        bool improvement = true;
        int bestCost = calculateTour(0ll, 0ll, currentTour);
        // std:://cout<<"seeking improvement: "<<bestCost;
        while (improvement) {
            improvement = false;
            long long int sz = currentTour.tour.size();

            for (auto i = 0ll; i < sz - 1; ++i) {
                for (auto j = i + 1; j < sz; ++j) {
                    // std:://cout<<"Checking ("<<i<<','<<j<<")\n";
                    Individual newTour{currentTour};
                    std::swap(newTour.tour[i], newTour.tour[j]);
                    int newCost = calculateTour(i, j, newTour);
                    // std:://cout<<"Current new cost: "<<newCost<<std::endl;

                    if (newCost < bestCost) {
                        // std:://cout<<"Improved!\n"<<bestCost<<" to "<<newCost<<std::endl;
                        currentTour = newTour;
                        bestCost = newCost;
                        improvement = true;
                    }
                }
            }
        // std:://cout<<"Best so far: "<<bestCost<<std::endl;
        }
        // //cout<<"improved\n";
        
        
        individual = currentTour;
        // individual.cost = calculateTour(0, 0, individual);

        if(individual.cost < b_ini_c){
            b_ini_c = individual.cost;
            b_ini_p = individual.tour;
        }
        population.push_back(individual);
        // std:://cout<<"We have "<<population.size()<<" individuals of "<<populationSize<<"\n";
    }
    ini_population = population;
    //cout<<"Population all set!\n";
}

void TabuMemetic::transgenesis(Individual& individual) {
    if(individual.cost) return;
}

std::pair<TabuMemetic::Individual, TabuMemetic::Individual> TabuMemetic::tournamentSelection(const std::vector<TabuMemetic::Individual>& population) {
    // std::random_device rd;
    // std::mt19937 g(rd());
    std::vector<Individual> tournament;

    // Ensure tournament size is positive and not larger than population size
    int tournamentSize = std::min(8, static_cast<int>(population.size()));
    
    // Randomly sample individuals for the tournament
    std::sample(population.begin(), population.end(), std::back_inserter(tournament),
                tournamentSize, g);


    // Evaluate tournament candidates and select the best one
    Individual best = tournament[0], sec = tournament[0];
    for (const auto& ind : tournament) {
        if (ind.cost < best.cost) {
            best = ind;
        }else if(ind.cost < sec.cost){
            sec = best;
        }
    }

    return std::make_pair(best, sec);
}

TabuMemetic::Individual TabuMemetic::crossover(const Individual& parent1, const Individual& parent2) {
    // std::random_device rd;
    // std::mt19937 g(rd());
    std::uniform_real_distribution<> disr(0.0, 1.0);
    
    int n = parent1.tour.size();
    Individual offspring;
    offspring.tour.resize(n);
    
    if(disr(g)<crossoverRate){
        
        // Random number generator
        std::uniform_int_distribution<> dis(0, n - 1);

        // Choose two random cut points
        int start = dis(g);
        int end = dis(g);
        if (start > end) std::swap(start, end);

        // Copy segment from the first parent
        for (int i = start; i <= end; ++i) {
            offspring.tour[i] = parent1.tour[i];
        }

        // Fill remaining positions from the second parent
        int current = (end + 1) % n;
        for (int i = 0; i < n; ++i) {
            int value = parent2.tour[(end + 1 + i) % n];
            // Place the value if not already in the offspring
            if (std::find(offspring.tour.begin(), offspring.tour.end(), value) == offspring.tour.end()) {
                offspring.tour[current] = value;
                current = (current + 1) % n;
            }
        }
    }
    return offspring;
}


void TabuMemetic::mutate(Individual& individual) {
    // std::random_device rd;
    // std::mt19937 g(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> idx_dis(0, individual.tour.size() - 1);

    if (dis(g) < mutationRate) {
        int start = idx_dis(g);
        int end = idx_dis(g);
        if (start > end) std::swap(start, end);
        std::reverse(individual.tour.begin() + start, individual.tour.begin() + end + 1);
    }
}

int TabuMemetic::calculateTour(const long long int& x, const long long int& y, const Individual& ind) {
    int totalCost = ind.cost;

    if(x || y){
         long long int sz = ind.tour.size();
         long long int yl2{max(y-2, 0ll)}, yl1{max(y-1, 0ll)}, yp1{min(y+1, sz)}, yp2{min(y+2, sz)};
         long long int xl2{max(x-2, 0ll)}, xl1{max(x-1, 0ll)}, xp1{min(x+1, sz)}, xp2{min(x+2, sz)};
        // //cout<<"Swappin' "<<x<<"n"<<y<<endl;

        totalCost += (yl2 ? (graph->custo[ind.tour[yl2]][ind.tour[yl1]][ind.tour[x]] - 
                             graph->custo[ind.tour[yl2]][ind.tour[yl1]][ind.tour[y]]) : 0);

        totalCost += (yl1 && (yp1<sz) ? (graph->custo[ind.tour[yl1]][ind.tour[x]][ind.tour[yp1]] - 
                                         graph->custo[ind.tour[yl1]][ind.tour[y]][ind.tour[yp1]]) : 0);

        totalCost += ((yp2<sz) ? (graph->custo[ind.tour[x]][ind.tour[yp1]][ind.tour[yp2]] - 
                                  graph->custo[ind.tour[y]][ind.tour[yp1]][ind.tour[yp2]]) : 0);

        totalCost += (xl2 ? (graph->custo[ind.tour[xl2]][ind.tour[xl1]][ind.tour[y]] - 
                             graph->custo[ind.tour[xl2]][ind.tour[xl1]][ind.tour[x]]) : 0);

        totalCost += (xl1 && (xp1<sz) ? (graph->custo[ind.tour[xl1]][ind.tour[y]][ind.tour[xp1]] - 
                                        graph->custo[ind.tour[xl1]][ind.tour[x]][ind.tour[xp1]]) : 0);

        totalCost += ((xp2<sz) ? (graph->custo[ind.tour[y]][ind.tour[xp1]][ind.tour[xp2]] - 
                                    graph->custo[ind.tour[x]][ind.tour[xp1]][ind.tour[xp2]]) : 0);
        // totalCost += (graph->custo[ind.tour[yl2]][ind.tour[yl1]][ind.tour[x]]
                    //  +graph->custo[ind.tour[yl1]][ind.tour[x]][ind.tour[y+1]]
                    //  +graph->custo[ind.tour[x]][ind.tour[y+1]][ind.tour[y+2]]) +  //adicionando yl2 / yl1 / x / y+1 / y+2
                    
                    //  (graph->custo[ind.tour[x-2]][ind.tour[x-1]][ind.tour[y]]
                    //  +graph->custo[ind.tour[x-1]][ind.tour[y]][ind.tour[x+1]]
                    //  +graph->custo[ind.tour[y]][ind.tour[x+1]][ind.tour[x+2]]) -  //adicionando x-2 / x-1 / y / x+1 / x+2
                    
                    //  (graph->custo[ind.tour[x-2]][ind.tour[x-1]][ind.tour[x]]
                    //  +graph->custo[ind.tour[x-1]][ind.tour[x]][ind.tour[x+1]]
                    //  +graph->custo[ind.tour[x]][ind.tour[x+1]][ind.tour[x+2]]) - //removendo x-2 / x-1 / x / x+1 / x+2
                    
                    //  (graph->custo[ind.tour[yl2]][ind.tour[yl1]][ind.tour[y]]
                    //  +graph->custo[ind.tour[yl1]][ind.tour[y]][ind.tour[y+1]]
                    //  +graph->custo[ind.tour[y]][ind.tour[y+1]][ind.tour[y+2]]);  //removendo y-2 / yl1 / y / y+1 / y+2
    } else {

        auto n = ind.tour.size();
        auto tour{ind.tour};
        
        // Traverse each triplet in the tour
        for (long unsigned int i = 0; i < n - 2; ++i) {
            int a = tour[i];
            int b = tour[i + 1];
            int c = tour[i + 2];
            if(graph->custo[a][b][c] < INT_MAX) totalCost += graph->custo[a][b][c];
            else return INT_MAX;
        }
        
        // Handle the wrap-around cases for the end of the tour
        int a = tour[n - 2];
        int b = tour[n - 1];
        int c = tour[0];
        if(graph->custo[a][b][c] < INT_MAX) totalCost += graph->custo[a][b][c];
        else return INT_MAX;
        
        // Add the cost for the last segment
        a = tour[n - 1];
        b = tour[0];
        c = tour[1];
        if(graph->custo[a][b][c] < INT_MAX) totalCost += graph->custo[a][b][c];
        else return INT_MAX;

    }



    ++currEvaluations;

    // //cout<<currEvaluations<<endl;

    
    return totalCost;
}

std::vector<int> TabuMemetic::run(Graph& graphInput) {
    graph = &graphInput;
    // //cout<<"initializing!\n";
    initializePopulation();

    while (currEvaluations < maxEvaluations) {
        // std:://cout<<"Gen "<<generation<<std::endl;
        // //cout<<"Max sz: "<<populationSize<<endl;
        for (int i = 0; i < populationSize; ++i) {
            //cout<<"Pop sz: "<<i<<endl;
            std::pair<Individual, Individual> parents = tournamentSelection(population);
            

            Individual child1 = crossover(parents.first, parents.second);
            Individual child2 = crossover(parents.second, parents.first);
            mutate(child1);
            mutate(child2);
            child1.cost = calculateTour(0ll, 0ll, child1);
            child2.cost = calculateTour(0ll, 0ll, child2);

            apply_tabu_search(child1);
            apply_tabu_search(child2);
            
            population.push_back(child1);
            population.push_back(child2);
        }
        //cout<<currEvaluations<<" de "<<maxEvaluations<<endl;
    }

    // Return the best individual from the final population
    return std::min_element(population.begin(), population.end(),
                           [](const Individual& a, const Individual& b) {
                               return a.cost < b.cost;
                           })->tour;
}

// Tabu Search Implementation

void TabuMemetic::apply_tabu_search(Individual& ind) {
    int best_cost = ind.cost;
    std::vector<int> best_tour = ind.tour;
    std::set<std::vector<int>> tabu_list;
    long long int sz = ind.tour.size();

    for (int i = 0; i < 10/*tabu_tenure*/; ++i) {
        for (auto j = 0ll; j < sz; ++j) {
            for (auto k = j + 1; k < sz; ++k) {
                std::vector<int> new_tour = ind.tour;
                std::swap(new_tour[j], new_tour[k]);
                int new_cost = calculateTour(j, k, ind);
            

                if (new_cost < best_cost && tabu_list.find(new_tour) == tabu_list.end()) {
                    best_cost = new_cost;
                    best_tour = new_tour;
                }
            }
        }
        // tabu_list.insert(std::to_string(best_tour));
        if (tabu_list.size() > static_cast<size_t>(10/*tabu_tenure*/)) {
            tabu_list.erase(tabu_list.begin());
        }
    }
    ind.tour = best_tour;
    ind.cost = best_cost;
}


