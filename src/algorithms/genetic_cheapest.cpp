#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <queue>
#include <bits/stdc++.h>
#include "../../include/algorithms/genetic_cheapest.h"

// the paper "On the min-cost traveling salesman problem with drone" uses a random k between 2 and 3
int GeneticCheapest::getK()
{
    std::uniform_int_distribution<int> distribution(2, 3);
    std::random_device rd;
    std::mt19937 engine(rd());

    return distribution(engine);
}

std::vector<int> GeneticCheapest::getCheapestNeighbors(Graph &graph)
{
    int n = graph.getMaxM();
    std::vector<int> vertexes;
    for (int i = 0; i < graph.getMaxM(); i++)
    {
        vertexes.push_back(i);
    }

    std::vector<std::vector<int>> bestResults;
    int k = this->getK();
    int bestResultsInitilizationCounter = 0;
    // initializing bestResults to avoid garbage
    for(int i = 0; i < k; i++) {
        std::vector<int> tmp;
        for(int j = 0; j < n; j++) {
            tmp.push_back(-1);
        }
        bestResults.push_back(tmp);
    }


    // preparing randomness to get a value between 0 and min(k, sizeInsertionsAvailable)
    std::uniform_int_distribution<int> distribution(0, k);
    std::random_device rd;
    std::mt19937 engine(rd());

    // will get a random value from between 0 and k best insertion
    int randomInsertionIndex = distribution(engine) % k;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                std::vector<int> tour;
                int v1 = i;
                int v2 = j;
                tour.push_back(v1);
                tour.push_back(v2);

                while ((int)tour.size() < n)
                {
                    // filter vertex yet to be checked
                    std::vector<int> vertexToCheck;

                    std::copy_if(
                        vertexes.begin(),
                        vertexes.end(),
                        std::back_inserter(vertexToCheck),
                        [&](int needle)
                        {
                            return std::find(tour.begin(), tour.end(), needle) == tour.end();
                        });

                    // vertex, index
                    std::pair<int, int> minX1, minX2, localSolution;
                    minX1 = std::make_pair(INT_MAX, -1);
                    minX2 = std::make_pair(INT_MAX, -1);
                    localSolution = std::make_pair(INT_MAX, -1);

                    for (int x : vertexToCheck)
                    {
                        if (graph.custo[x][v1][v2] < minX1.first)
                        {
                            minX1.first = graph.custo[x][v1][v2];
                            minX1.second = x;
                        }

                        if (graph.custo[v1][v2][x] < minX2.first)
                        {
                            minX2.first = graph.custo[v1][v2][x];
                            minX2.second = x;
                        }

                        if (minX1.first + minX2.first < localSolution.first)
                        {
                            localSolution.first = minX1.first + minX2.first;
                            localSolution.second = x;
                        }
                    }

                    auto minX = std::min(minX1, minX2);

                    int index = minX == minX1 ? tour.size() - 2 : tour.size();

                    v1 = minX == minX1 ? v1 : v2;
                    v2 = minX.second;

                    tour.insert(tour.begin() + index, minX.second);
                }

                if (bestResultsInitilizationCounter < k)
                {
                    if(bestResults[bestResultsInitilizationCounter].size() == 0) {
                        for(auto t: tour) {
                            bestResults[bestResultsInitilizationCounter].push_back(t);
                        }
                    } else {
                        for(int t = 0; t < (int)tour.size(); t++) {
                            bestResults[bestResultsInitilizationCounter][t] = tour[t];
                        }
                    }
                    bestResultsInitilizationCounter += 1;
                }
                else
                {
                    // calcular onde esse melhor resultado vai ser inserido
                    int worstLocalResultIndex = -1;
                    int worstLocalResultValue = -1;
                    std::vector<int> worstLocalResult;
                    for (int l = 0; l < k; l++)
                    {
                        if (worstLocalResult.size() == 0)
                        {
                            worstLocalResultIndex = k;
                            worstLocalResult = bestResults[l];
                        }
                        else
                        {
                            int val = this->eval(worstLocalResult, graph);

                            if (this->eval(bestResults[l], graph) < val)
                            {
                                worstLocalResult = bestResults[l];
                                worstLocalResultIndex = l;
                                worstLocalResultValue = val;
                            }
                        }
                    }

                    if (this->eval(tour, graph) < worstLocalResultValue)
                    {
                        bestResults[worstLocalResultIndex] = tour;
                    }
                }
            }
        }
    }

    return bestResults[randomInsertionIndex];
}

std::vector<std::vector<int>> GeneticCheapest::initializePopulation(Graph &graph)
{
    std::vector<std::vector<int>> population;

    while ((int)population.size() < this->populationSize)
    {
        std::vector<int> cheapestNeighbors;
        cheapestNeighbors = this->getCheapestNeighbors(graph);
        population.push_back(cheapestNeighbors);
    }

    this->evaluateFitness(population, graph);

    return population;
}

std::vector<int> GeneticCheapest::run(Graph &graph)
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
