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
public: 
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
    ~Benchmark();
    int evaluate();
    int evaluate(std::string instance, std::string algorithm);
};

#endif