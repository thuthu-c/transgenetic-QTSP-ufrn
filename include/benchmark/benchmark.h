#ifndef _BENCHMARK_H
#define _BENCHMARK_H

#include "../data_structures/graph.h"
#include "../algorithms/tsp_solver.h"

class Benchmark
{
private:
    int maxEvaluations;
    int populationSize;
    float crossoverRate;
    float mutationRate;
    

    int tabuTime;
    int tabuAspirationTime;
    int tabuMaxIter;

    double probT;
    double stepProb;
    double plasmidSize;
    int plasmidBank;
    double plasmidMin, plasmidMax;
public: 

    Benchmark(
    int maxEvaluations,
    int populationSize
    );
    
    Benchmark(
    int maxEvaluations,
    int populationSize,
    double plasmidSize
    );

    Benchmark(
        int tabuTime,
        int tabuAspirationTime,
        int tabuMaxIter
    );
    Benchmark(
        int maxEvaluations,
        int populationSize,
        float crossoverRate,
        float mutationRate
    );

       Benchmark(
            int maxEvaluations,
            int populationSize,
            double probT,
            double stepProb,
            double plasmidSize
        );

    Benchmark(
    int maxEvaluations,
    int populationSize,
    float crossoverRate,
    float mutationRate,
    double probT,
    double stepProb,
    double plasmidSize
    );

     Benchmark(
    int maxEvaluations,
    int populationSize,
    float crossoverRate,
    float mutationRate,
    double probT,
    double stepProb,
    double plasmidSize,
    int plasmidBank
    );

     Benchmark(
            int maxEvaluations,
            int populationSize,
            double probT,
            double stepProb,
            double plasmidSize,
            int plasmidBank
        );
    Benchmark(
            int maxEvaluations,
            int populationSize,
            double probT,
            double stepProb,
            double plasmidSize,
            int plasmidBank, 
            double plasmidMin,
            double plasmidMax
        );

        Benchmark(
    int maxEvaluations,
    int populationSize,
    float crossoverRate,
    float mutationRate,
    double probT,
    double stepProb,
    double plasmidSize,
    double plasmidMin,
    double plasmidMax
    );
    
    ~Benchmark();
    int evaluate();
    int evaluate(std::string instance, std::string algorithm);
};

#endif