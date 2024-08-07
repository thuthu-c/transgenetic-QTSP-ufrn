#include <chrono>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <bits/stdc++.h>
#include "../../include/algorithms/memetic.h"

extern std::random_device rd;
extern std::mt19937 engine;

Memetic::Memetic(
    int maxEvaluations,
    int populationSize,
    float crossoverRate,
    float mutationRate)
{
    this->maxEvaluations = maxEvaluations;
    this->populationSize = populationSize,
    this->setCrossverRate(crossoverRate);
    this->setMutationRate(mutationRate);
    this->numEvaluation = 0;
    this->numBestIndividuals = 50;
}

Memetic::~Memetic()
{
}

std::vector<int> Memetic::getNearestNeighbors(int cityIndex, Graph &graph, int neighborNumber)
{
    int n = graph.getMaxM();
    std::vector<int> result;
    std::vector<std::vector<int>> bestResults;

    std::vector<int> vertexes;
    for (int i = 0; i < n; i++)
    {
        vertexes.push_back(i);
    }

    for (int j = 0; j < n; j++)
    {
        if (cityIndex != j)
        {
            std::vector<int> tour;
            int v1 = cityIndex;
            int v2 = j;
            tour.push_back(cityIndex);
            tour.push_back(j);

            // remaining vertexes
            std::vector<int> vertexToCheck;

            std::copy_if(
                vertexes.begin(),
                vertexes.end(),
                std::back_inserter(vertexToCheck),
                [&](int needle)
                {
                    return std::find(tour.begin(), tour.end(), needle) == tour.end();
                });

            std::pair<int, int> nearestNeighbor = std::make_pair(INT_MAX, -1);
            for (auto v3 : vertexToCheck)
            {
                if (v3 != cityIndex)
                {
                    nearestNeighbor = std::min(nearestNeighbor, std::make_pair(graph.custo[v1][v2][v3], v3));
                    v1 = v2;
                    v2 = v3;
                    if (nearestNeighbor.second != cityIndex)
                    {
                        tour.push_back(nearestNeighbor.second);
                    }
                }
            }

            if ((int)bestResults.size() < neighborNumber)
            {
                if (cityIndex != tour[1])
                {
                    bestResults.push_back(tour);
                    result.push_back(tour[1]);
                }
            }
            else
            {
                // calcular onde esse melhor resultado vai ser inserido
                int worstLocalResultIndex = -1;
                int worstLocalResultValue = -1;
                std::vector<int> worstLocalResult;
                for (int k = 0; k < neighborNumber; k++)
                {
                    if (k != cityIndex)
                    {
                        if (worstLocalResult.size() == 0)
                        {
                            worstLocalResultIndex = k;
                            worstLocalResult = bestResults[k];
                        }
                        else
                        {
                            int val = this->eval(worstLocalResult, graph);
                            if (this->eval(bestResults[k], graph) < val)
                            {
                                worstLocalResult = bestResults[k];
                                worstLocalResultIndex = k;
                                worstLocalResultValue = val;
                            }
                        }
                    }
                }

                if (this->eval(tour, graph) < worstLocalResultValue)
                {
                    if (result[worstLocalResultIndex] != cityIndex)
                    {
                        result[worstLocalResultIndex] = j;
                        bestResults[worstLocalResultIndex] = tour;
                    }
                }
            }
        }
    }

    return result;
}

std::vector<std::vector<int>> Memetic::initializeNearestNeighbors(int populationSize, Graph &graph)
{
    int cityIndex = 0;
    int cityNumber = graph.getMaxM();
    std::vector<std::vector<int>> population;

    std::vector<int> vertexes;
    for (int i = 0; i < graph.getMaxM(); i++)
    {
        vertexes.push_back(i);
    }

    // neighborNumber it's fixed to five according to the paper:
    // Greedy Permuting Method for Genetic Algorithm on TSP
    const int neighborNumber = 5;

    while (cityIndex < cityNumber)
    {
        std::vector<int> neighbors = this->getNearestNeighbors(
            cityIndex,
            graph,
            neighborNumber);

        for (int neighbor : neighbors)
        {
            std::vector<int> individual = {cityIndex, neighbor};
            std::vector<int> permutedCities = this->greedyPermuting(individual, graph, cityNumber);
            individual.insert(individual.end(), permutedCities.begin(), permutedCities.end());
            population.push_back(individual);
        }

        cityIndex += 1;
    }

    // load remaining population randomly
    std::uniform_int_distribution<int> distribution(0, (int)graph.getMaxM());

    while ((int)population.size() < populationSize)
    {
        std::vector<int> currentPopulation;

        std::vector<int> vertexesToBeRandomInserted;
        for (int j = 0; j < (int)graph.getMaxM(); j++)
        {
            vertexesToBeRandomInserted.push_back(j);
        }

        for (int k = 0; k < (int)graph.getMaxM(); k++)
        {
            int randIndex = distribution(engine) % vertexesToBeRandomInserted.size();
            currentPopulation.push_back(vertexesToBeRandomInserted[randIndex]);

            // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
            vertexesToBeRandomInserted.erase(
                std::remove(
                    vertexesToBeRandomInserted.begin(),
                    vertexesToBeRandomInserted.end(),
                    vertexesToBeRandomInserted[randIndex]),
                vertexesToBeRandomInserted.end());
        }

        population.push_back(currentPopulation);
    }

    // Trim the population to the desired size
    if (population.size() > populationSize) {
        population.erase(population.begin() + populationSize, population.end());
    }

    return population;
}

std::vector<int> Memetic::greedyPermuting(std::vector<int> &seedIndividual, Graph &graph, int n)
{
    std::vector<int> individual = seedIndividual;
    std::vector<int> restCities;
    for (int i = 0; i < n; i++)
    {
        if (std::find(individual.begin(), individual.end(), i) == individual.end()) // valor não existe no vetor)
        {
            restCities.push_back(i);
        }
    }

    while (!restCities.empty())
    {
        int nextCity = restCities[0];
        int minCost = INT_MAX;
        for (int i = 0; i < n; i++)
        {
            if (graph.custo[individual.back()][nextCity][i] < minCost)
            {
                minCost = graph.custo[individual.back()][nextCity][i];
            }
        }
        for (int city : restCities)
        {
            int cost = INT_MAX;
            for (int i = 0; i < n; i++)
            {
                if (graph.custo[individual.back()][city][i] < cost)
                {
                    cost = graph.custo[individual.back()][city][i];
                }
            }
            if (cost < minCost)
            {
                nextCity = city;
                minCost = cost;
            }
        }
        individual.push_back(nextCity);
        restCities.erase(std::remove(restCities.begin(), restCities.end(), nextCity), restCities.end());
    }

    // Remove the initial seed cities from the result to return only the new cities
    for(int city : seedIndividual)
    {
        individual.erase(std::remove(individual.begin(), individual.end(), city), individual.end());
    }

    return individual;
}

std::vector<std::vector<int>> Memetic::initializeRandomPopulation(int populationSize, Graph &graph)
{
    std::vector<std::vector<int>> population;

    std::uniform_int_distribution<int> distribution(0, (int)graph.getMaxM());

    for (int i = 0; i < populationSize; i++)
    {
        std::vector<int> currentPopulation;

        std::vector<int> vertexesToBeRandomInserted;
        for (int j = 0; j < (int)graph.getMaxM(); j++)
        {
            vertexesToBeRandomInserted.push_back(j);
        }

        for (int k = 0; k < (int)graph.getMaxM(); k++)
        {
            int randIndex = distribution(engine) % vertexesToBeRandomInserted.size();
            currentPopulation.push_back(vertexesToBeRandomInserted[randIndex]);

            // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
            vertexesToBeRandomInserted.erase(
                std::remove(
                    vertexesToBeRandomInserted.begin(),
                    vertexesToBeRandomInserted.end(),
                    vertexesToBeRandomInserted[randIndex]),
                vertexesToBeRandomInserted.end());
        }

        population.push_back(currentPopulation);
    }

    return population;
}

std::vector<std::vector<int>> Memetic::initializePopulation(Graph &graph)
{
    std::vector<std::vector<int>> population(this->populationSize);

    int randomPopulationSize = (int)(0.5 * this->populationSize);

    std::vector<std::vector<int>> randomPopulation, cheapestPopulation, nearestPopulation;
    randomPopulation = this->initializeRandomPopulation(
        randomPopulationSize,
        graph);

    int nearestPopulationSize = this->populationSize - randomPopulationSize;
    nearestPopulation = this->initializeNearestNeighbors(
        nearestPopulationSize,
        graph);

    for (int i = 0; i < randomPopulationSize; i++)
    {
        population[i] = randomPopulation[i];
    }

    for (int i = 0; i < nearestPopulationSize; i++)
    {
        population[i + randomPopulationSize] = nearestPopulation[i];
    }

    return population;
}

// it uses tournament
std::pair<int, std::vector<int>> Memetic::selectParent(
    std::vector<std::pair<int, std::vector<int>>> &populationEvaluated)
{
   int tournamentSize = 5;
    std::vector<std::pair<int, std::vector<int>>> tournament(tournamentSize);
    
    for(int i = 0; i < tournamentSize; i++) {
        int randIndex = std::rand() % populationEvaluated.size();
        tournament[i] = populationEvaluated[randIndex];
    }

    std::pair<int, std::vector<int>> best = tournament[0];

    for(auto &competitor : tournament) {
        if(competitor.first < best.first) {
            best = competitor;
        }
    }

    return best;
}

std::vector<int> Memetic::generateOffspring(std::vector<int> &mother, std::vector<int> &father)
{
    int n = mother.size();
    int random1 = -1;
    int random2 = -1;

    std::uniform_int_distribution<int> distribution(0, n - 1);

    std::vector<int> offspring(n, -1);

    // generate two random positions
    while (true)
    {
        random1 = distribution(engine);
        random2 = distribution(engine);
        if (random1 != random2)
        {
            break;
        }
    }

    if (random1 > random2)
    {
        std::swap(random1, random2);
    }

    // copy the segment of mother to the offspring
    std::vector<int> motherSlice(mother.begin() + random1, mother.begin() + random2);

    for (int i = random1; i < random2; i++)
    {
        offspring[i] = mother[i];
    }

    // executes a cyclic traverse in father and fill the offspring's blanks
    int i = random2;
    int offspringIndex = i % n;
    for (int _ = 0; _ < n; _++)
    {
        int circularIndex = i % n;

        bool offspringHasNotFatherElem = std::find(
                                             motherSlice.begin(),
                                             motherSlice.end(),
                                             father[circularIndex]) == motherSlice.end();

        if (offspringHasNotFatherElem)
        {
            offspring[offspringIndex] = father[circularIndex];
            offspringIndex = (offspringIndex + 1) % n;
        }

        i += 1;
    }

    return offspring;
}

std::pair<int, std::vector<int>> Memetic::crossover(
    std::vector<int> &mother,
    std::vector<int> &father,
    Graph &graph)
{
    auto offspring1 = this->generateOffspring(mother, father);
    auto offspring2 = this->generateOffspring(father, mother);

    int offspring1Val = this->eval(offspring1, graph);
    int offspring2Val = this->eval(offspring2, graph);

    if (offspring1Val < offspring2Val)
    {
        return std::make_pair(offspring1Val, offspring1);
    }

    return std::make_pair(offspring2Val, offspring2);
}

int Memetic::eval(std::vector<int> &tour, Graph &graph)
{
    int tourVal = 0;
    int n = graph.getMaxM();

    tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
    tourVal += graph.custo[tour[n - 1]][tour[0]][tour[1]];

    for (int j = 0; j < (n - 2); j++)
    {
        tourVal += graph.custo[tour[j]][tour[j + 1]][tour[j + 2]];
    }

    this->numEvaluation += 1;

    return tourVal;
}

std::pair<int, std::vector<int>> swap(std::pair<int, std::vector<int>>& tour, int i, int j) {
    std::pair<int, std::vector<int>> newTour;
    newTour.second = tour.second;

    // std::reverse usa o intervalo [first, last),
    // então é necessário utilizar j + 1 para incluir o elemento j na reversão.
    std::reverse(newTour.second.begin() + i, newTour.second.begin() + j + 1);

    return newTour;
}

std::pair<int, std::vector<int>> Memetic::twoOpt(Graph &graph, std::pair<int, std::vector<int>>& tour) {
    bool improved = true;

    while(improved && this->numEvaluation < this->maxEvaluations) {
        improved = false;
        for(int i = 1; i < tour.second.size() - 1; ++i) {
            bool localImprovement = false;

            for(int j = i + 1; j < tour.second.size(); ++j) {
                std::pair<int, std::vector<int>> newTour = swap(tour, i, j);
                newTour.first = this->eval(newTour.second, graph);
                if(newTour.first < tour.first) {
                    tour = newTour;
                    improved = true;
                    // alterar o break para ter uma parada do for em i também. 
                    // A modificação do tour deve gerar uma nova iteração completa
                    // PS: Lembrar de modificar o texto da monografia
                    break; 
                }
            }

            if(localImprovement) {
                break;
            }
        }
    }

    return tour;
}

std::vector<int> Memetic::mutate(std::vector<int> &individual)
{
    std::vector<int> mutated = individual;
    std::uniform_int_distribution<int> distribution(0, static_cast<int>(individual.size()) - 1);

    int random1 = distribution(engine);
    int random2 = distribution(engine);

    while (random2 == random1)
    {
        random2 = distribution(engine);
    }

    std::swap(mutated[random1], mutated[random2]);

    return mutated;
}

std::vector<std::pair<int, std::vector<int>>> Memetic::evaluatePopulation(std::vector<std::vector<int>> &population, Graph &graph)
{
    std::vector<std::pair<int, std::vector<int>>> evaluatedPopulation;

    for (int i = 0; i < (int)population.size(); i++)
    {
        int cost = this->eval(population[i], graph);
        evaluatedPopulation.push_back(std::make_pair(cost, population[i]));
    }

    std::sort(
        evaluatedPopulation.begin(),
        evaluatedPopulation.end(),
        [](const std::pair<int, std::vector<int>> &a, const std::pair<int, std::vector<int>> &b)
        {
            return a.first < b.first;
        });

    return evaluatedPopulation;
}

std::vector<int> Memetic::run(Graph &graph)
{
    this->numEvaluation = 0;

    std::uniform_real_distribution<double> distribution(0, 1);
    
    std::vector<std::vector<int>> population = this->initializePopulation(graph);

    std::vector<std::pair<int, std::vector<int>>> populationEvaluated = this->evaluatePopulation(population, graph);

    std::pair<int, std::vector<int>> bestIndividual = populationEvaluated[0];
    std::pair<int, std::vector<int>> worstIndividual = populationEvaluated[0];
    int worstIdx = 0;

    while (this->numEvaluation < this->maxEvaluations)
    {
        std::vector<std::pair<int, std::vector<int>>> newPopulationEvaluated;

        for (int i = 0; i < (int) populationEvaluated.size(); i++)
        {
            std::pair<int, std::vector<int>> offspring;
            std::vector<int> individual = populationEvaluated[i].second;

            // Crossover
            if (distribution(engine) < this->crossoverRate)
            {
                std::pair<int, std::vector<int>> parentSelected = this->selectParent(populationEvaluated);
                offspring = this->crossover(individual, parentSelected.second, graph);
            }
            else
            {
                offspring = populationEvaluated[i];
            }

            // Mutation
            if (distribution(engine) < this->mutationRate)
            {
                auto mutated = this->mutate(offspring.second);
                auto mutatedCost = this->eval(mutated, graph);
                offspring = std::make_pair(mutatedCost, mutated);
            }

            // Apply 2-opt local search to the offspring
            offspring = this->twoOpt(graph, offspring);

            if(offspring.first < bestIndividual.first) {
                bestIndividual = offspring;
            }
            if(offspring.first > worstIndividual.first) {
                worstIndividual = offspring;
                worstIdx = i;
            }

            newPopulationEvaluated.push_back(offspring);
        }

        // Replace the worst individual with the best individual
        newPopulationEvaluated[worstIdx] = bestIndividual;

        populationEvaluated = newPopulationEvaluated;
    }

    return bestIndividual.second;
}

void Memetic::setCrossverRate(float crossoverRate)
{
    if (crossoverRate < 0 || crossoverRate > 1)
    {
        throw std::invalid_argument(
            "Crossover rate must be a value bigger than zero and smaller than one");
    }

    this->crossoverRate = crossoverRate;
}

void Memetic::setMutationRate(float mutationRate)
{
    if (mutationRate < 0 || mutationRate > 1)
    {
        throw std::invalid_argument(
            "Crossover rate must be a value bigger than zero and smaller than one");
    }

    this->mutationRate = mutationRate;
}
