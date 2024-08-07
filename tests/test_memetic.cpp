#include <iostream>
#include <limits>
#include <algorithm>
#include "../libs/doctest.h"
#include "../include/data_structures/graph.h"
#include "../include/helpers/graphio.h"
#include "../include/benchmark/graph_generator.h"
#include "../include/algorithms/memetic.h"

TEST_CASE("It can sort map")
{
    std::priority_queue<
        std::pair<int, int>,
        std::vector<std::pair<int, int>>,
        std::greater<std::pair<int, int>>
    > populationEvaluated;

    populationEvaluated.push(std::make_pair(4, 4));
    populationEvaluated.push(std::make_pair(1, 1));
    populationEvaluated.push(std::make_pair(3, 3));
    CHECK_EQ(populationEvaluated.top().first, 1);
}

TEST_CASE("It can run memetic")
{
    int numVertex = 5; // Number of nodes
    Graph graph(numVertex);

    int maxGenerations = 600;
    int populationSize = 100;
    float crossoverRate = 0.9;
    float mutationRate = 0.1;

    Memetic mm(
        maxGenerations,
        populationSize,
        crossoverRate,
        mutationRate
    );

    int*** matrix = new int**[5]{
        new int*[5]{
            new int[5]{0, 1, 9, 7, 5},
            new int[5]{1, 2, 10, 8, 6},
            new int[5]{9, 10, 18, 16, 14},
            new int[5]{7, 8, 16, 14, 12},
            new int[5]{5, 6, 14, 12, 10},
        },
        new int*[5]{
            new int[5]{2, 1, 3, 11, 7},
            new int[5]{1, 0, 2, 10, 6},
            new int[5]{3, 2, 4, 12, 8},
            new int[5]{11, 10, 12, 20, 16},
            new int[5]{7, 6, 8, 16, 12},
        },
        new int*[5]{
            new int[5]{18, 11, 9, 12, 17},
            new int[5]{11, 4, 2, 5, 10},
            new int[5]{9, 2, 0, 3, 8},
            new int[5]{12, 5, 3, 6, 11},
            new int[5]{17, 10, 8, 11, 16},
        },
        new int*[5]{
            new int[5]{14, 17, 10, 7, 11},
            new int[5]{17, 20, 13, 10, 14},
            new int[5]{10, 13, 6, 3, 7},
            new int[5]{7, 10, 3, 0, 4},
            new int[5]{11, 14, 7, 4, 8},
        },
        new int*[5]{
            new int[5]{10, 11, 13, 9, 5},
            new int[5]{11, 12, 14, 10, 6},
            new int[5]{13, 14, 16, 12, 8},
            new int[5]{9, 10, 12, 8, 4},
            new int[5]{5, 6, 8, 4, 0},
        }
    };


    graph.custo = matrix;

    auto result = mm.run(graph);

    std::set<int> s;
    for(auto p: result) {
        s.insert(p);
    }

    CHECK_EQ(result.size(), s.size()); // check if vertex are unique
    CHECK_EQ(result.size(), numVertex);

    for(auto r: result) {
        CHECK(r < numVertex);
    }
}

TEST_CASE("It can generate offspring")
{
    std::vector<int> mother = {3, 0, 4, 1, 2};
    std::vector<int> father = {0, 1, 2, 4, 3};

    int maxGenerations = 1;
    int populationSize = 100;
    float crossoverRate = 0.25;
    float mutationRate = 0.25;

    Memetic gg(
        maxGenerations,
        populationSize,
        crossoverRate,
        mutationRate
    );

    auto f1 = gg.generateOffspring(mother, father);
    std::set<int> s1;
    for(auto f: f1) {
        s1.insert(f);
    }

    auto f2 = gg.generateOffspring(father, mother);
    std::set<int> s2;
    for(auto f: f2) {
        s2.insert(f);
    }

    CHECK_EQ(f1.size(), s1.size()); // check if vertex are unique
    CHECK_EQ(f2.size(), s2.size()); // check if vertex are unique
}

TEST_CASE("Testing twoOpt for QTSP") {
    int n = 5; // Number of nodes
    Graph graph(n);

    int maxGenerations = 1;
    int populationSize = 100;
    float crossoverRate = 0.25;
    float mutationRate = 0.25;

    Memetic mm(
        maxGenerations,
        populationSize,
        crossoverRate,
        mutationRate
    );

    int*** matrix = new int**[5]{
        new int*[5]{
            new int[5]{0, 1, 9, 7, 5},
            new int[5]{2, 1, 3, 11, 7},
            new int[5]{18, 11, 9, 12, 17},
            new int[5]{14, 17, 10, 7, 11},
            new int[5]{10, 11, 13, 9, 5},
        },
        new int*[5]{
            new int[5]{1, 2, 10, 8, 6},
            new int[5]{1, 0, 2, 10, 6},
            new int[5]{11, 4, 2, 5, 10},
            new int[5]{17, 20, 13, 10, 14},
            new int[5]{11, 12, 14, 10, 6},
        },
        new int*[5]{
            new int[5]{9, 10, 18, 16, 14},
            new int[5]{3, 2, 4, 12, 8},
            new int[5]{9, 2, 0, 3, 8},
            new int[5]{10, 13, 6, 3, 7},
            new int[5]{13, 14, 16, 12, 8},
        },
        new int*[5]{
            new int[5]{7, 8, 16, 14, 12},
            new int[5]{11, 10, 12, 20, 16},
            new int[5]{12, 5, 3, 6, 11},
            new int[5]{7, 10, 3, 0, 4},
            new int[5]{9, 10, 12, 8, 4},
        },
        new int*[5]{
            new int[5]{5, 6, 14, 12, 10},
            new int[5]{7, 6, 8, 16, 12},
            new int[5]{17, 10, 8, 11, 16},
            new int[5]{11, 14, 7, 4, 8},
            new int[5]{5, 6, 8, 4, 0},
        }
    };

    graph.custo = matrix;

    std::pair<int, std::vector<int>> tour = {60, {0, 2, 1, 3, 4}};
    auto result = mm.twoOpt(graph, tour);
    CHECK(result.first <= tour.first);
}

TEST_CASE("Optimal Swap Check for QTSP") {
    int n = 3;
    Graph graph(n);
    graph.custo = new int**[n] {
        new int*[n] {
            new int[n] {0, 1, 2},
            new int[n] {1, 0, 3},
            new int[n] {2, 3, 0}
        },
        new int*[n] {
            new int[n] {2, 0, 1},
            new int[n] {3, 0, 2},
            new int[n] {1, 2, 0}
        },
        new int*[n] {
            new int[n] {1, 2, 0},
            new int[n] {2, 0, 3},
            new int[n] {3, 1, 0}
        }
    };

    std::pair<int, std::vector<int>> tour = {100, {0, 1, 2} };

    int maxGenerations = 100;
    int populationSize = 100;
    float crossoverRate = 0.9;
    float mutationRate = 0.1;

    Memetic mm(
        maxGenerations,
        populationSize,
        crossoverRate,
        mutationRate
    );

    auto result = mm.twoOpt(graph, tour);

    std::vector<int> expectedTour = {0, 2, 1};

    CHECK(result.second == expectedTour);
}
