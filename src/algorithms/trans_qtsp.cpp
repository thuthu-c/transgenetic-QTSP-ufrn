#include "../../include/algorithms/trans_qtsp.h"
#include <numeric>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <string>
#include <vector>
#include "../../include/helpers/random.h"

const int gir_size = 100;

std::vector<int> TransQTSP::run(Graph& graphInput){
    this->numEvaluation = 0;
    graph = &graphInput;
    int vertex_q = graph->getNumVertex();

    // eu preciso pegar esse grafo e gerar os ciclos, caminhos,
    // árvores e cromossosmos para o GIR
    generate_gir(graph);

    population = generate_population(graph);

    std::vector<int> best_solution = *population.begin();

    while(num_evaluations > numEvaluation){
        // gerar um agente transgenético
        // plasmídeo e transposon

        //ter um número de probailidade para o plasmideo
        for(auto p: population){
            if(rand() % 2){
                Plasmid plasmid;
                plasmid = generate_plasmid(gir);
                std::vector<int> new_solution = m1(plasmid, p.size(), p); 
                if(cost(p) < cost(new_solution)){
                    p = new_solution;
                    gir.emplace_back(p);
                }
            }else{
                std::vector<int> transposon;
                transposon =transposon_4OPT(p);
                if(cost(transposon) < cost(p)){
                    p = transposon;
                    gir.emplace_back(p);
                }
            }
            
            if(cost(p) < cost(best_solution)) best_solution = p;

        }
    }

    return best_solution;

}


// qual o tamanho de um repositório????
void TransQTSP::generate_gir(Graph *graph){
    int n = graph->getNumVertex();

    // pensar em formas de gerar os ciclos, caminhos, árvores e cromossomos.
    // perguntar a silvia quais opções de elementos podem compor o gir
    std::vector<int> solution;
    std::vector<int> vertices = getVertex();


    // geando os caminhos hamiltonianos aleatoriamente
    for(auto i{0}; i < gir_size; ++i){
        std::shuffle(vertices.begin(), vertices.end(), g);

        for (auto v : vertices)
        {
            solution.push_back(v);
        }

        gir.push_back(solution);
    }    
}




void TransQTSP::generate_population_random(Graph *graph){
    int n = graph->getNumVertex();
    std::vector<int> vertices = getVertex();

    // geando os caminhos hamiltonianos aleatoriamente
    for(auto i{0}; i < populationSize; ++i){
        std::vector<int> solution;
        std::shuffle(vertices.begin(), vertices.end(), g);
        solution.reserve(n);
        solution = vertices; 
        population.push_back(solution);
    }
}


int TransQTSP::eval(std::vector<int> &tour, Graph &graph)
{
    int tourVal = 0;
    int n = graph.getMaxM();

    tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
    tourVal += graph.custo[tour[n - 1]][tour[0]][tour[1]];

    for (int j = 0; j < (n - 2); j++)
    {
        tourVal += graph.custo[tour[j]][tour[j + 1]][tour[j + 2]];
    }

    this->numEvaluation++;

    return tourVal;
}

std::vector<int> TransQTSP::getNearestNeighbors(int cityIndex, Graph &graph, int neighborNumber)
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

std::vector<int> TransQTSP::greedyPermuting(std::vector<int> &seedIndividual, Graph &graph, int n)
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
        if (individual.empty()) 
        {
            int startCity = restCities.front();
            individual.push_back(startCity);
            restCities.erase(restCities.begin());
            continue; // Go to the next iteration safely
        }

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


std::vector<std::vector<int>> TransQTSP::initializeNearestNeighbors(int populationSize, Graph &graph)
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
    if (population.size() > (unsigned long long)populationSize) {
        population.erase(population.begin() + populationSize, population.end());
    }

    return population;
}

std::vector<std::vector<int>> TransQTSP::initializeRandomPopulation(int populationSize, Graph &graph)
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

// LIU, J.; LI, W. Greedy permuting method for genetic algorithm on traveling salesman
// problem. In: IEEE. 2018 8th International Conference on Electronics Information and
// Emergency Communication (ICEIEC). [S.l.], 2018. p. 47–51.
std::vector<std::vector<int>> TransQTSP::generate_population(Graph *graph){

    std::vector<std::vector<int>> population(populationSize);

    int randomPopulationSize = (int)(0.5 * populationSize);

    std::vector<std::vector<int>> randomPopulation, cheapestPopulation, nearestPopulation;
    randomPopulation = this->initializeRandomPopulation(
        randomPopulationSize,
        *graph);

    int nearestPopulationSize = this->populationSize - randomPopulationSize;
    nearestPopulation = this->initializeNearestNeighbors(
        nearestPopulationSize,
        *graph);

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



std::vector<int> TransQTSP::getVertex()
{
    int n = graph->getNumVertex();
    std::vector<int> vertex;
    vertex.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        vertex.push_back(i);
    }
    return vertex;
}

// como gerar um agente transgenético? 
// plasmideo um pedaço de informação genética e um método de manipulação
// a informação genética vem de uma solução da população?

// gerando o plasmideo usando o crowding_distances()
// The genetic information of simple plasmids comes
// from a solution (source) of the external archive. The solution is chosen at random in
// the least crowded region of the objective space deﬁned by the points correspondent
// to the solutions of the archive.

TransQTSP::Plasmid TransQTSP::generate_plasmid(std::vector<std::vector<int>>& gir) {
    // std::cout<< "oi to gerando um plasmideo" << std::endl;
    TransQTSP::Plasmid plasmid;
    int gir_size = gir.size();
    
    if (gir_size == 0) return plasmid;
    
    int source_index = 0;

    if (gir_size < 3) {
        std::uniform_int_distribution<int> dist_gir(0, gir_size - 1);
        source_index = dist_gir(engine);
    } else {

        std::vector<std::pair<int, long long int>> custos_gir;
        custos_gir.reserve(gir_size);
        for (int i = 0; i < gir_size; ++i) {
            custos_gir.push_back({i, cost(gir[i])});
        }
        std::sort(custos_gir.begin(), custos_gir.end(), 
            [](const std::pair<int, long long int>& a, const std::pair<int, long long int>& b) {
                return a.second < b.second;
            });

        std::vector<std::pair<int, long long int>> crowding_distances; 
        
        for (int i = 1; i < gir_size - 1; ++i) {
            long long int distance = custos_gir[i + 1].second - custos_gir[i - 1].second;
            crowding_distances.push_back({custos_gir[i].first, distance});
        }

        std::sort(crowding_distances.begin(), crowding_distances.end(),
            [](const std::pair<int, long long int>& a, const std::pair<int, long long int>& b) {
                return a.second > b.second; 
            });

        int top_k = std::max(1, static_cast<int>(0.1 * crowding_distances.size()));
        std::uniform_int_distribution<int> dist_top(0, top_k - 1);
        
        source_index = crowding_distances[dist_top(engine)].first;
    }

    const std::vector<int>& endossimbionte = gir[source_index];
    int n_vertices = endossimbionte.size();
    
    if (n_vertices == 0) return plasmid;

    int pl = std::max(1, static_cast<int>(plasmidSize * n_vertices));

    std::uniform_int_distribution<int> dist_start(0, n_vertices - 1);
    int start_idx = dist_start(engine);

    plasmid.genes.reserve(pl);
    for (int i = 0; i < pl; ++i) {
        plasmid.genes.push_back(endossimbionte[(start_idx + i) % n_vertices]);
    }

    plasmid.fitness_gain = 0;

    // std::cout<< "o tamanho do plasmideo eh pra ser: " << plasmidSize << " mas na realidade eh " << plasmid.genes.size() << std:: endl;
    return plasmid;
}

std::vector<int> TransQTSP::m1(const Plasmid &p, int tamanho_solucao, const std::vector<int>& solution) {

    // std::cout<< "solucao antes da transcricao: " << std::endl;
    // for(auto s : solution) std::cout << s << " ";
    // std::cout<< std::endl;
    std::vector<int> novo_ciclo;
    novo_ciclo.reserve(tamanho_solucao);

    if (p.genes.empty()) return solution; 

    int primeiro_gene = p.genes[0];
    // std::cout << "o primeiro gene do plasmideo eh " << primeiro_gene << std::endl; 
    
    std::unordered_set<int> plasmid_genes(p.genes.begin(), p.genes.end());

    for (int v : solution) {
        // std::cout<<"o vertice atual eh " << v << std::endl;
        if (v == primeiro_gene) {
            novo_ciclo.insert(novo_ciclo.end(), p.genes.begin(), p.genes.end());
        } 
        else if (plasmid_genes.find(v) == plasmid_genes.end()) {
            novo_ciclo.push_back(v);
        }
    }

    // std::cout<< "solucao depois da transcricao: " << std::endl;
    // for(auto n : novo_ciclo) std::cout << n << " ";
    // std::cout<< std::endl;  

    return novo_ciclo;
}


long long int TransQTSP::cost(const std::vector<int>& tour)
{
    long long int totalCost{0};
    int n = tour.size();

    for (int i{0}; i < n - 2; ++i)
    {
        totalCost += graph->custo[tour[i]][tour[i + 1]][tour[i + 2]];
    }

    totalCost += graph->custo[tour[n - 2]][tour[n - 1]][tour[0]];
    totalCost += graph->custo[tour[n - 1]][tour[0]][tour[1]];

    numEvaluation++;
    return totalCost;
}


// transposon 

std::vector<int> TransQTSP::transposon_4OPT(const std::vector<int>& individual){
    return best4opt(individual);
}

inline int get_safe(const std::vector<int> &tour, int i)
{
    int n = tour.size();
    return tour[((i % n) + n) % n];
}

std::vector<int> TransQTSP::best4opt(const std::vector<int>& solution)
{
    int n = solution.size();
    if (n < 8)
        return std::vector<int>(solution);

    // Tabelas da DP
    // F[i2][j1] armazena o melhor custo parcial
    // Parent[i2][j1] armazena qual i1 gerou esse custo (para reconstrução)
    std::vector<std::vector<double>> F(n, std::vector<double>(n, 1e18));
    std::vector<std::vector<int>> parent_i1(n, std::vector<int>(n, -1));

    // Inicialização (Algoritmo 2 linha 3) - Corrigido índice 7 para 2 (base 0) que equivale ao 3 do artigo
    for (int j1 = 4; j1 <= n - 4; ++j1)
    {
        F[2][j1] = D2O(0, j1, solution); // Assume i1 = 0
        parent_i1[2][j1] = 0;
    }

    // Recursão (Algoritmo 2 linhas 4-6)
    for (int i2 = 3; i2 <= n - 5; ++i2)
    {
        for (int j1 = i2 + 2; j1 <= n - 3; ++j1)
        {
            double val_stay = F[i2 - 1][j1];
            double val_new = D2O(i2 - 1, j1, solution); // Novo corte

            // Simplificação da recorrência: escolhe o melhor entre estender ou novo corte
            // Nota: A lógica exata da Eq 13 pode variar, mas a ideia é minimizar custo
            if (val_new < val_stay)
            {
                F[i2][j1] = val_new;
                parent_i1[i2][j1] = i2 - 1;
            }
            else
            {
                F[i2][j1] = val_stay;
                parent_i1[i2][j1] = parent_i1[i2 - 1][j1];
            }
        }
    }

    // Parte 2: Encontrar o melhor Delta* (combinação dos cortes)
    double best_improvement = -0.000001; // Só aceita se melhorar (negativo)
    int best_i1 = -1, best_i2 = -1, best_j1 = -1, best_j2 = -1;

    for (int i2 = 3; i2 <= n - 5; ++i2)
    {
        for (int j2 = i2 + 5; j2 <= n - 1; ++j2)
        {
            // Varredura simplificada para achar o melhor j1 compatível
            for (int j1 = i2 + 2; j1 <= j2 - 2; ++j1)
            {
                double current_delta = D2O(i2, j2, solution) + F[i2][j1];
                numEvaluation++;

                if (current_delta < best_improvement)
                {
                    best_improvement = current_delta;
                    best_i2 = i2;
                    best_j2 = j2;
                    best_j1 = j1;
                    best_i1 = parent_i1[i2][j1];
                }
            }
        }
    }

    // Se encontrou melhora, APLICA O MOVIMENTO
    if (best_i2 != -1)
    {
        // std::cout << "Aplicando Double Bridge! Melhora: " << best_improvement << std::endl;
        return fourOptMove(best_i1, best_i2, best_j1, best_j2, solution);
    }

    return solution; // Sem melhora
}

long long int TransQTSP::D2O(int i, int j, const std::vector<int>& tour)
{
    int vi_minus_1 = get_safe(tour, i - 1);
    int vi = get_safe(tour, i);
    int vi_plus_1 = get_safe(tour, i + 1);
    int vi_plus_2 = get_safe(tour, i + 2);

    int vj_minus_1 = get_safe(tour, j - 1);
    int vj = get_safe(tour, j);
    int vj_plus_1 = get_safe(tour, j + 1);
    int vj_plus_2 = get_safe(tour, j + 2);

    long long int removed =
        graph->custo[vi_minus_1][vi][vi_plus_1] + graph->custo[vi][vi_plus_1][vi_plus_2] + 
        graph->custo[vj_minus_1][vj][vj_plus_1] + graph->custo[vj][vj_plus_1][vj_plus_2];

    long long int added =
        graph->custo[vi_minus_1][vi][vj_plus_1] + graph->custo[vi][vj_plus_1][vj_plus_2] + 
        graph->custo[vj][vi_plus_1][vi_plus_2] + graph->custo[vj_minus_1][vj][vi_plus_1];

    return added - removed;
}



std::vector<int> TransQTSP::fourOptMove(int i1, int i2, int j1, int j2, const std::vector<int>& tour)
{
    // dividir a tour em quatro partes
    std::vector<int> parte_um;
    std::vector<int> parte_dois;
    std::vector<int> parte_tres;
    std::vector<int> parte_quatro;

    int n = static_cast<int>(tour.size());
    if (n == 0)
        return {};

    auto append_range = [&](int start, int end, std::vector<int> &dest)
    {
        int idx = start;
        while (true)
        {
            dest.push_back(tour[idx]);
            if (idx == end)
                break;
            idx = (idx + 1) % n;
        }
    };

    // part1: from j2+1 to i1 (wrapping)
    int s1 = (j2 + 1) % n;
    int e1 = i1 % n;
    append_range(s1, e1, parte_um);

    // part2: from i1+1 to i2
    int s2 = (i1 + 1) % n;
    int e2 = i2 % n;
    append_range(s2, e2, parte_dois);

    // part3: from i2+1 to j1
    int s3 = (i2 + 1) % n;
    int e3 = j1 % n;
    append_range(s3, e3, parte_tres);

    // part4: from j1+1 to j2
    int s4 = (j1 + 1) % n;
    int e4 = j2 % n;
    append_range(s4, e4, parte_quatro);

    // montando a nova tour (example reconnection: 1 + 4 + 3 + 2)
    std::vector<int> solution;
    solution = parte_um;
    solution.insert(solution.end(), parte_quatro.begin(), parte_quatro.end());
    solution.insert(solution.end(), parte_tres.begin(), parte_tres.end());
    solution.insert(solution.end(), parte_dois.begin(), parte_dois.end());

    return solution;
}


