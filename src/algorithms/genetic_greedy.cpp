#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <queue>
#include <bits/stdc++.h>
#include "../../include/algorithms/genetic_greedy.h"

int GeneticGreedy::eval(std::vector<int> &tour, Graph &graph)
{
    int tourVal = 0;
    int n = tour.size();

    tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
    tourVal += graph.custo[tour[n - 1]][tour[0]][tour[1]];

    for (int j = 0; j < (n - 2); j++)
    {
        tourVal += graph.custo[tour[j]][tour[j + 1]][tour[j + 2]];
    }

    return tourVal;
}

std::vector<int> GeneticGreedy::getNearestNeighbors(int cityIndex, Graph &graph, int neighborNumber)
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
                if(v3 != cityIndex) {
                    nearestNeighbor = std::min(nearestNeighbor, std::make_pair(graph.custo[v1][v2][v3], v3));
                    v1 = v2;
                    v2 = v3;
                    if(nearestNeighbor.second != cityIndex) {
                        tour.push_back(nearestNeighbor.second);
                    }
                }
            }

            if ((int) bestResults.size() < neighborNumber)
            {
                if(cityIndex != tour[1]) {
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
                    if(k != cityIndex) {
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
                    if(result[worstLocalResultIndex] != cityIndex) {
                        result[worstLocalResultIndex] = j;
                        bestResults[worstLocalResultIndex] = tour;
                    }
                }
            }
        }
    }

    return result;
}

std::vector<int> GeneticGreedy::greedyPermuting(std::vector<int> &cities)
{
    auto result = cities; // copy vector

    std::next_permutation(result.begin(), result.end());

    return result;
}

std::vector<std::vector<int>> GeneticGreedy::initializePopulation(Graph &graph)
{
    int cityIndex = 0;
    std::vector<std::vector<int>> population;

    std::vector<int> vertexes;
    for (int i = 0; i < graph.getMaxM(); i++)
    {
        vertexes.push_back(i);
    }

    // neighborNumber it's fixed to five according to the paper:
    // Greedy Permuting Method for Genetic Algorithm on TSP
    const int neighborNumber = 5;

    while (cityIndex < graph.getMaxM())
    {
        std::vector<int> neighbors = getNearestNeighbors(
            cityIndex,
            graph,
            neighborNumber);

        for (auto neighbor: neighbors)
        {
            std::vector<int> individual;
            individual.push_back(cityIndex);
            individual.push_back(neighbor);

            std::vector<int> restCities;

            // filter remaining cities
            std::copy_if(
                vertexes.begin(),
                vertexes.end(),
                std::back_inserter(restCities),
                [&](int needle)
                {
                    return std::find(individual.begin(), individual.end(), needle) == individual.end();
                });

            restCities = greedyPermuting(restCities);

            for (auto restCity : restCities)
            {
                individual.push_back(restCity);
            }

            population.push_back(individual);
        }

        cityIndex += 1;
    }

    // load remaining population randomly
    std::uniform_int_distribution<int> distribution(0, (int)graph.getMaxM());
    std::random_device rd;
    std::mt19937 engine(rd());

    while ((int) population.size() < this->populationSize)
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

std::vector<int> GeneticGreedy::run(Graph &graph)
{
    std::uniform_real_distribution<double> distribution(0, 1);
    std::random_device rd;
    std::mt19937 engine(rd());

    int generation = 0;

    std::vector<std::vector<int>> population = this->initializePopulation(graph);

    this->evaluateFitness(population, graph);

    std::vector<int> offspring;
    while (generation < this->maxGenerations)
    {
        std::vector<std::vector<int>> newPopulation;

        for (int i = 0; i < (int)population.size(); i++)
        {
            bool isNotEltims = (!this->isElitism(i));
            // TODO::Move this logic to the crossover function
            if (isNotEltims && distribution(engine) >= this->crossoverRate)
            {
                std::vector<int> parent2 = this->selectParent(population, graph);

                offspring = this->crossover(population[i], parent2);
            }
            else
            {
                offspring = population[i];
            }

            newPopulation.push_back(offspring);
        }

        population = newPopulation;
        this->evaluateFitness(population, graph);
        std::vector<std::vector<int>> nnewPopulation;

        for (int i = 0; i < (int)population.size(); i++)
        {
            // TODO::move this logic to the mutation function
            bool isNotEltims = (!this->isElitism(i));
            if (isNotEltims && distribution(engine) >= this->mutationRate)
            {
                offspring = this->mutate(population[i]);
            }
            else
            {
                offspring = population[i];
            }

            nnewPopulation.push_back(offspring);
        }
        population = nnewPopulation;
        this->evaluateFitness(population, graph);
        generation += 1;
    }

    return population[0];
}
