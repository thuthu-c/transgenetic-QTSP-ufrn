#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>
#include "../../include/algorithms/genetic_algorithm.h"

GeneticAlgorithm::GeneticAlgorithm(
    int maxGenerations,
    int populationSize,
    float crossoverRate,
    float mutationRate,
    float elitimsNumber)
{
    this->maxGenerations = maxGenerations;
    this->populationSize = populationSize,
    this->setCrossverRate(crossoverRate);
    this->setMutationRate(mutationRate);
    this->elitimsNumber = elitimsNumber;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
}

std::vector<std::vector<int>> GeneticAlgorithm::initializePopulation(Graph &graph)
{
    std::vector<std::vector<int>> population;

    std::uniform_int_distribution<int> distribution(0, (int)graph.getMaxM());
    std::random_device rd;
    std::mt19937 engine(rd());

    for (int i = 0; i < this->populationSize; i++)
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
                    vertexesToBeRandomInserted[randIndex]
                ), 
                vertexesToBeRandomInserted.end()
            );
        }

        population.push_back(currentPopulation);
    }

    return population;
}

// it uses proportional roulette wheel selection method
std::vector<int> GeneticAlgorithm::selectParent(std::vector<std::vector<int>> &population, Graph &graph)
{
    int totalSum = 0;
    for(auto individual: population) {
        totalSum += this->eval(individual, graph);
    }

    std::vector<std::pair<int, double>> proportionalPopulation;
    double maxPopulationFitness = -1;
    for(int i = 0; i < (int) population.size(); i++) {
        double individualValue = (double) this->eval(population[i], graph)/ (double)totalSum;

        if(individualValue > maxPopulationFitness) {
            maxPopulationFitness = individualValue;
        }

        proportionalPopulation.push_back(std::make_pair(i, individualValue));
    }

    std::sort(
        proportionalPopulation.begin(), 
        proportionalPopulation.end()
    );

    std::uniform_real_distribution<double> distribution(0.0, maxPopulationFitness);
    std::random_device rd;
    std::mt19937 engine(rd());
    double randValue = distribution(engine);

    int i = -1;
    for(auto individual: proportionalPopulation) {
        if(individual.second >= randValue) {
            i = individual.first;
            break;
        }
    }

    return population[i];
}

std::vector<int> GeneticAlgorithm::crossover(std::vector<int> &mother, std::vector<int> &father)
{
    std::uniform_int_distribution<int> distribution(0, (int)mother.size() - 1);
    std::random_device rd;
    std::mt19937 engine(rd());

    std::vector<int> crossed(mother.size(), -1);

    int random1 = -1;
    int random2 = -1;

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
        crossed[i] = mother[i];
    }

    // executes a cyclic traverse in father and fill the offspring's blanks
    int fatherIndex = 0;
    int i = random2;

    while (fatherIndex < (int)father.size())
    {
        bool crossedHasNotFatherElem = std::find(
                                           motherSlice.begin(),
                                           motherSlice.end(),
                                           father[fatherIndex]) == motherSlice.end();

        if (crossedHasNotFatherElem)
        {
            crossed[i % (mother.size())] = father[fatherIndex];
            i += 1;
        }

        fatherIndex += 1;
    }

    return crossed;
}

// since the population is sorted the elitism can be checked by individual index
bool GeneticAlgorithm::isElitism(int individualIndex)
{
    int elitismIndex = (int) this->populationSize * this->elitimsNumber;

    // checking if population was on top elitsm
    return individualIndex <= elitismIndex;
}

int GeneticAlgorithm::eval(std::vector<int> &tour, Graph &graph)
{
    int tourVal = 0;
    int n = graph.getMaxM();

    tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
    tourVal += graph.custo[tour[n-1]][tour[0]][tour[1]];

    for(int j = 0; j < (n-2); j++)
    {
        tourVal += graph.custo[tour[j]][tour[j+1]][tour[j+2]];
    }

    return tourVal;
}

std::vector<int> GeneticAlgorithm::mutate(std::vector<int> &individual)
{
    std::vector<int> mutated = individual;

    std::uniform_int_distribution<int> distribution(0, (int)individual.size()-1);
    std::random_device rd;
    std::mt19937 engine(rd());
    int random1 = -1;
    int random2 = -1;

    while (true)
    {
        random1 = distribution(engine);
        random2 = distribution(engine);
        if (random1 != random2)
        {
            break;
        }
    }

    std::swap(mutated[random1], mutated[random2]);

    return mutated;
}

void GeneticAlgorithm::evaluateFitness(std::vector<std::vector<int>> &population, Graph &graph)
{
    return std::sort(
        population.begin(),
        population.end(), 
        [&](std::vector<int> &a, std::vector<int> &b) {
            return this->eval(a, graph) < this->eval(b, graph);
        }
    );
}

std::vector<int> GeneticAlgorithm::run(Graph &graph)
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

        for (int i = 0; i < (int) population.size(); i++)
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
            // condicionar a melhoria da solução
            newPopulation.push_back(offspring);
        }

        population = newPopulation;
        this->evaluateFitness(population, graph);
        std::vector<std::vector<int>> nnewPopulation;

        for (int i = 0; i < (int) population.size(); i++)
        {
            // TODO::move this logic to the mutation function
            bool isNotEltims = (!this->isElitism(i));
            if (isNotEltims && distribution(engine) >= this->mutationRate) // aumentar taxa de mutação
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

void GeneticAlgorithm::setCrossverRate(float crossoverRate)
{
    if (crossoverRate < 0 || crossoverRate > 1)
    {
        throw std::invalid_argument(
            "Crossover rate must be a value bigger than zero and smaller than one");
    }

    this->crossoverRate = crossoverRate;
}

void GeneticAlgorithm::setMutationRate(float mutationRate)
{
    if (mutationRate < 0 || mutationRate > 1)
    {
        throw std::invalid_argument(
            "Crossover rate must be a value bigger than zero and smaller than one");
    }

    this->mutationRate = mutationRate;
}