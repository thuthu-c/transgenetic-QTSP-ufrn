#include "../../include/algorithms/another_genetic.h"
#include <numeric>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>

using namespace std;

AnotherGenetic::AnotherGenetic(int popSize, int maxGen, double mutRate, double crossRate)
    : populationSize(popSize), maxGenerations(maxGen), mutationRate(mutRate) , crossoverRate(crossRate){}

AnotherGenetic::~AnotherGenetic() {}

void AnotherGenetic::initializePopulation() {
    population.clear();
    for (int i = 0; i < populationSize/2; ++i) {
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

        // Random number generator
        std::random_device rd;
        std::mt19937 gen(rd());

        // Shuffle triples to introduce randomness
        std::shuffle(triples.begin(), triples.end(), gen);

        // Start with a random triple
        auto startTriple = triples.back();
        triples.pop_back();
        solution.push_back(std::get<0>(startTriple));
        solution.push_back(std::get<1>(startTriple));
        solution.push_back(std::get<2>(startTriple));
        visited[std::get<0>(startTriple)] = true;
        visited[std::get<1>(startTriple)] = true;
        visited[std::get<2>(startTriple)] = true;

        // Continue constructing the solution
        while (solution.size() < n) {
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
            std::shuffle(restrictedCandidateList.begin(), restrictedCandidateList.end(), gen);

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
                std::shuffle(triples.begin(), triples.end(), gen);
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

        // Make sure the solution is a valid circular tour
        if (solution.size() > n) {
            solution.resize(n);
        }
        //performing local search
        std::vector<int> currentTour = solution;
        bool improvement = true;

        while (improvement) {
            improvement = false;
            int bestCost = calculateTour(currentTour);

            for (size_t i = 0; i < currentTour.size() - 1; ++i) {
                for (size_t j = i + 1; j < currentTour.size(); ++j) {
                    std::vector<int> newTour = currentTour;
                    std::swap(newTour[i], newTour[j]);
                    int newCost = calculateTour(newTour);

                    if (newCost < bestCost) {
                        currentTour = newTour;
                        bestCost = newCost;
                        improvement = true;
                    }
                }
            }
        }

        individual.tour = currentTour;





        individual.cost = calculateTour(individual.tour);
        population.push_back(individual);
    }
    for (int i = 0; i < populationSize/2; ++i) {
        Individual individual;
        int n = graph->getMaxM();
        individual.tour.resize(n);
        std::iota(individual.tour.begin(), individual.tour.end(), 0);

        // Shuffle to create a random permutation
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(individual.tour.begin(), individual.tour.end(), gen);

        individual.cost = calculateTour(individual.tour);
        population.push_back(individual);
    }

}

void AnotherGenetic::transgenesis(Individual& individual) {
    
}

std::pair<AnotherGenetic::Individual, AnotherGenetic::Individual> AnotherGenetic::tournamentSelection(const std::vector<AnotherGenetic::Individual>& population) {
    std::random_device rd;
    std::mt19937 g(rd());
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

AnotherGenetic::Individual AnotherGenetic::crossover(const Individual& parent1, const Individual& parent2) {
    std::random_device rd;
    std::mt19937 g(rd());
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


void AnotherGenetic::mutate(Individual& individual) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> idx_dis(0, individual.tour.size() - 1);

    if (dis(g) < mutationRate) {
        int start = idx_dis(g);
        int end = idx_dis(g);
        if (start > end) std::swap(start, end);
        std::reverse(individual.tour.begin() + start, individual.tour.begin() + end + 1);
    }
}

int AnotherGenetic::calculateTour(const std::vector<int>& tour) {
    int n = tour.size();
    int totalCost = 0;
    
    // Traverse each triplet in the tour
    for (int i = 0; i < n - 2; ++i) {
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

    return totalCost;
}

std::vector<int> AnotherGenetic::run(Graph& graphInput) {
    graph = &graphInput;
    initializePopulation();

    for (int generation = 0; generation < maxGenerations; ++generation) {
        std::vector<Individual> newPopulation;

        for (int i = 0; i < populationSize; ++i) {
            std::pair<Individual, Individual> parents = tournamentSelection(population);
            

            Individual child1 = crossover(parents.first, parents.second);
            Individual child2 = crossover(parents.second, parents.first);
            mutate(child1);
            mutate(child2);
            child1.cost = calculateTour(child1.tour);
            child2.cost = calculateTour(child2.tour);
            
            population.push_back(child1);
            population.push_back(child2);
        }
    }

    // Return the best individual from the final population
    return std::min_element(population.begin(), population.end(),
                           [](const Individual& a, const Individual& b) {
                               return a.cost < b.cost;
                           })->tour;
}
