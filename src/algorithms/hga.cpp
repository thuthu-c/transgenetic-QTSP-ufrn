#include "../../include/algorithms/hga.h"
#include "../../include/algorithms/cheapest_insertion.h"
#include <numeric>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <string>
#include <vector>

const long long int uElite = 5;
std::vector<long int> costsTriples;
const int gama = 20;
extern std::mt19937 engine;

typedef std::tuple<int, int, int> triple;

void print_tour(std::vector<int> tour)
{
    for (auto t : tour)
    {
        std::cout << t << std::endl;
    }
}

HGA::HGA(int populationSize, int itMax) : populationSize(populationSize), graph(nullptr), itMax(itMax)
{
    std::random_device rd;
    g = std::mt19937(rd());
}
void HGA::setGraph(Graph &graph)
{
    this->graph = &graph;
}

std::vector<int> HGA::run(Graph& graphInput){
    graph = &graphInput;
    population.clear(); 
    
    std::uniform_real_distribution<double> distribution(0, 1);
    double p_mut = 0.9;

    // 1. Inicializa
    initializePopulation();

    // 2. Variáveis para guardar o Melhor Global (Seguras, sem ponteiros)
    double bestGlobalCost = std::numeric_limits<double>::max();
    std::vector<int> bestGlobalTour;

    // Inicializa o best global com o melhor da população inicial
    // Precisamos de uma avaliação inicial rápida para setar o bestGlobal
    std::vector<std::pair<int, Individual*>> tempEval = evaluatePopulation(population);
    if(!tempEval.empty()){
        bestGlobalCost = tempEval[0].first;
        bestGlobalTour = tempEval[0].second->tour;
    }

    // --- LOOP PRINCIPAL ---
    for (int generation = 0; generation < itMax; ++generation) {
    
        // [CORREÇÃO 1] Recalcula ponteiros AQUI, no início de cada geração.
        // Isso garante que selectParent use ponteiros válidos após o erase/push_back da geração anterior.
        std::vector<std::pair<int, Individual*>> evaluatedPopulation = evaluatePopulation(population);

        // 3. Seleção
        std::pair<int, Individual*> parent1Ptr = selectParent(evaluatedPopulation);
        std::pair<int, Individual*> parent2Ptr = selectParent(evaluatedPopulation);
        
        // Garante pais diferentes (com limite para não travar)
        int tentativas = 0;
        while(parent1Ptr.second == parent2Ptr.second && tentativas < 10) {
            parent2Ptr = selectParent(evaluatedPopulation);
            tentativas++;
        }

        // 4. Crossover
        std::pair<int, Individual*> offspringPair = crossover(parent1Ptr, parent2Ptr);
        
        // Copia para objeto local seguro
        Individual offspringIndi = *(offspringPair.second); 
        delete offspringPair.second; // Limpa heap

        // 5. Mutação
        if(distribution(engine) < p_mut){
            std::cout<<"o que estou mandando pro ruin and recreate" << std::endl;
            print_tour(offspringIndi.tour);
            ruinAndRecreate(offspringIndi);
        }

        // 6. Busca Local
        offspringIndi.tour = lsProcedure(offspringIndi.tour);
        
        // Recalcula custo
        offspringIndi.cost = cost(offspringIndi.tour);
        offspringIndi.costRank = -1; 
        offspringIndi.diversityRank = -1;

        // [CORREÇÃO 2] Atualização do Melhor Global de forma segura
        if (offspringIndi.cost < bestGlobalCost) {
            std::cout << "Geracao " << generation << " | Novo melhor: " << offspringIndi.cost << std::endl;
            bestGlobalCost = offspringIndi.cost;
            bestGlobalTour = offspringIndi.tour; // Copia o vetor (seguro)
        }

        // 7. Atualização da População
        // Estratégia simples e segura: Adiciona o filho e remove o pior da lista atual.
        
        population.push_back(offspringIndi);

        // Como adicionamos um, precisamos remover um para manter o tamanho.
        // O evaluatedPopulation (calculado no inicio) tem a ordem dos piores antigos.
        // O pior é o último da lista evaluatedPopulation.
        
        Individual* worstPtr = evaluatedPopulation.back().second;

   
        individualCostRank();
        individualDiversityRank();
        
        // Reavalia para achar o pior ATUAL (incluindo o filho, se ele for muito ruim)

        if(populationSize > 40){
            auto currentEval = evaluatePopulation(population);
        
            // Remove o último (o pior rankeado)
            Individual* targetToRemove = currentEval.back().second;
        
            for(auto it = population.begin(); it != population.end(); ++it) {
                if ( &(*it) == targetToRemove ) {
                 population.erase(it);
                    break;
                }
            }   
        }
        
    } 
    // Fim do Loop

    std::cout << "Fim da execucao. Melhor custo: " << bestGlobalCost << std::endl;
    
    if (!bestGlobalTour.empty()) {
        print_tour(bestGlobalTour); 
        return bestGlobalTour;
    } else {
        std::cout << "Retornando solucao vazia (erro)." << std::endl;
        return std::vector<int>();
    }
}

HGA::~HGA() {}
// criar um individuo da população
HGA::Individual HGA::createIndividuals()
{
    // grafo da run do TspSlver
    // numero de nós do grafo que estamos recebendo
    int n = graph->getMaxM();

    std::cout << "O n é " << n << std::endl;

    // criando um individuo
    Individual individual;

    // solução: uma tour qtsp formada por triplas válidas
    std::vector<int> solution;

    std::vector<int> vertices = getVertex();

    shuffle_vertex(vertices);

    // antes da inserção dos vértices na solução é necessário diversificar com o operador de mutação e intensificar com a busca local
    // aqui só estamos inserindo os vértices aleatoriamente na solução

    for (auto v : vertices)
    {
        solution.push_back(v);
        std::cout << "VÉRTICE: " << v << std::endl;
    }

    individual.tour = solution;

    std::cout << "Solução antes da diversificação" << " " << std::endl;

    for (auto s : solution)
    {
        std::cout << s << " " << std::endl;
    }
    // diversificar

    std::cout << "Vou arruinar e recriar" << " " << std::endl;
    ruinAndRecreate(individual);

    auto solution2 = individual.tour;

    std::cout << "Solução depois da diversificação" << " " << std::endl;
    for (auto s : solution2)
    {
        std::cout << s << " " << std::endl;
    }

    // intensificar

    individual.tour = lsProcedure(solution2);

    std::cout << "Solução depois da intensificação" << " " << std::endl;
    for (auto s : individual.tour)
    {
        std::cout << s << " " << std::endl;
    }
    setIndividualCost(individual);

    std::cout << "custo do individuo: " << individual.cost << std::endl;

    if (individual.tour.size() != n)
    {
        std::cerr << "[ERRO FATAL] Tentando retornar individuo invalido em createIndividuals!" << std::endl;
        std::cerr << "Tamanho esperado: " << n << " Tamanho real: " << individual.tour.size() << std::endl;
        exit(1);
    }

    return individual;
}

std::vector<int> HGA::getVertex()
{
    int n = graph->getNumVertex();
    std::vector<int> vertex(n);
    int cont{0};
    for (auto &v : vertex)
    {
        v = cont++;
    }

    return vertex;
}

void HGA::shuffle_vertex(std::vector<int> &vertex)
{
    std::shuffle(vertex.begin(), vertex.end(), g);
}

void HGA::createPopulation()
{
    for (auto i{0}; i < populationSize; ++i)
    {
        population.push_back(createIndividuals());
    }

    for (auto p : population)
    {
        setIndividualDiversityContribution(p);
        std::cout << "diversidade do individuo: " << p.diversityContribution << std::endl;
    }
}

long long int HGA::cost(std::vector<int> tour)
{

    long long int totalCost{0};

    std::vector<int> custo;

    int n = tour.size();

    for (auto i{0}; i < n - 2; ++i)
    {
        // std::cout<< "totalCost+= graph->custo[tour[i]][tour[i+1]][tour[i+2]] = " << graph->custo[tour[i]][tour[i+1]][tour[i+2]] << std::endl;
        totalCost += graph->custo[tour[i]][tour[i + 1]][tour[i + 2]];

        // std::cout << "tour[i] : " << tour[i] << " tour[i+1] : " << tour[i+1] << " tour[i+2] : " << tour[i+2] << std::endl;
    }

    // std::cout<< "totalCost+= graph->custo[tour[n-2]][tour[n-1]][tour[0]] = " << graph->custo[tour[n-2]][tour[n-1]][tour[0]] << std::endl;
    totalCost += graph->custo[tour[n - 2]][tour[n - 1]][tour[0]];
    // std::cout<< "totalCost+= graph->custo[tour[n-1]][tour[0]][tour[1]] =  " << graph->custo[tour[n-1]][tour[0]][tour[1]] << std::endl;
    totalCost += graph->custo[tour[n - 1]][tour[0]][tour[1]];
    return totalCost;
}

void HGA::setIndividualCost(HGA::Individual &individual)
{
    individual.cost = cost(individual.tour);
}

void HGA::setIndividualDiversityContribution(HGA::Individual &individual)
{
    individual.diversityContribution = dc(individual);
}

void HGA::setIndividualCostRank(HGA::Individual &individual)
{
    individual.costRank = cost(individual.tour);
}

// criar uma lista com a referencia dos individuos da população
std::vector<HGA::Individual *> HGA::generateCopyPopulation()
{

    std::vector<Individual *> copyPopulation;

    for (auto &i : population)
    {
        copyPopulation.push_back(&i);
    }

    return copyPopulation;
}

void HGA::individualCostRank()
{

    std::vector<Individual *> copyPopulation = generateCopyPopulation();

    // seta o custo de cada individuo na populção original
    for (auto individual : copyPopulation)
    {
        setIndividualCost(*individual);
    }

    std::cout << "O problema é aqui claramente" << std::endl;

    std::sort(copyPopulation.begin(), copyPopulation.end(),
              [](const Individual *a, const Individual *b)
              {
                  return a->cost < b->cost;
              });

    int cont{0};
    for (auto i : copyPopulation)
    {
        // seta o rank de cada individuo na populção original
        i->costRank = cont++;
    }

    std::cout << "terminei de setar o rank do custo" << std::endl;
}

std::vector<HGA::Individual> &HGA::getPopulation()
{
    return this->population;
}

void HGA::individualDiversityRank()
{

    std::vector<Individual *> copyPopulation = generateCopyPopulation();

    // seta o custo de cada individuo na populção original
    for (auto individual : copyPopulation)
    {
        setIndividualDiversityContribution(*individual);
    }

    std::sort(copyPopulation.begin(), copyPopulation.end(),
              [](const Individual *a, const Individual *b)
              {
                  return a->diversityContribution < b->diversityContribution;
              });

    int cont{0};
    for (auto i : copyPopulation)
    {
        // seta o rank de cada individuo na populção original
        i->diversityRank = ++cont;
    }
}

int HGA::fc(HGA::Individual individual)
{
    return individual.costRank;
}

double HGA::fd(HGA::Individual individual)
{
    return individual.diversityRank;
}

// acesso a tour e lido com os vértices de cada tour
std::vector<std::pair<int, int>> HGA::generateAllNodesPairs(HGA::Individual individual)
{
    std::vector<std::pair<int, int>> pairs;

    for (auto i{0}; i < individual.tour.size() - 1; ++i)
    {
        pairs.push_back(std::make_pair(individual.tour[i], individual.tour[i + 1]));
    }

    return pairs;
}

double HGA::dc(HGA::Individual Pi)
{
    // a variável é setada assim no artigo, ver uma maneira de receber melhor a variável (colocar como atributo da classe)
    int n_close = 2;

    double resultSum{0};
    for (auto j{1}; j <= n_close; ++j)
    {
        Individual Pj = population[j];
        resultSum += normalizeBronkenPairsDistance(Pi, Pj);
    }
    return (1.0 / n_close) * resultSum;
}

double HGA::normalizeBronkenPairsDistance(HGA::Individual Pi, HGA::Individual Pj)
{
    int n = graph->getMaxM();
    return (1.0 / n) * numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj(Pi, Pj);
}

long int HGA::numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj(HGA::Individual Pi, HGA::Individual Pj)
{
    long int numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj{0};
    std::vector<std::pair<int, int>> pairsPi = generateAllNodesPairs(Pi);
    std::vector<std::pair<int, int>> pairsPj = generateAllNodesPairs(Pj);

    for (auto i : pairsPi)
    {
        const auto search = std::find(pairsPj.begin(), pairsPj.end(), i);
        const auto unsucceeded = pairsPj.end();
        if (search != unsucceeded)
            numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj++;
    }
    return numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj;
}

int HGA::biasedFitness(HGA::Individual individual)
{
    // Lower values of biasedFitness indicate better fitness.
    // If negative values are returned, check if this is expected for your problem instance.

    if (fc(individual) == -1 || fd(individual) == -1)
        return INT_MAX;

    return fc(individual) + (1 - ((double)uElite / populationSize)) * fd(individual);
}

std::vector<std::pair<int, HGA::Individual *>> HGA::evaluatePopulation(std::vector<HGA::Individual> &population)
{

    std::vector<std::pair<int, HGA::Individual *>> evaluatedPopulation;
    int cost{0};

    for (auto &i : population)
    {
        cost = 0;
        cost = biasedFitness(i);
        evaluatedPopulation.push_back(std::make_pair(cost, &i));
    }

    std::sort(evaluatedPopulation.begin(), evaluatedPopulation.end(),
              [](const std::pair<int, Individual *> &a, const std::pair<int, Individual *> &b)
              {
                  return a.first < b.first;
              });

    return evaluatedPopulation;
}

void HGA::initializePopulation()
{
    createPopulation();
    individualDiversityRank();
    individualCostRank();
}

int HGA::generateNumberOfVertexToBeRemove(std::vector<int> tour)
{
    int numero_de_nos = tour.size();
    std::cout << "qual o número de nós? " << numero_de_nos << " " << std::endl;
    int p_min = 1;
    int p_max = std::min(40, numero_de_nos / 3);

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(p_min, p_max);

    int retorno = distrib(gen);

    std::cout << "o retorno é? " << retorno << " " << std::endl;

    return retorno;
}

std::vector<int> HGA::worstRemovalHeuristic(HGA::Individual indi)
{
    int custo_total = cost(indi.tour);
    std::vector<std::pair<int, int>> node_costs;

    // 1. Calcular o impacto real de remover cada nó
    for (size_t i = 0; i < indi.tour.size(); ++i)
    {
        std::vector<int> temp_tour = indi.tour;
        int v = temp_tour[i];
        temp_tour.erase(temp_tour.begin() + i); // REMOVE de verdade para testar
        int custo_sem_v = cost(temp_tour);

        // O "pior" nó é aquele cuja remoção mais reduz o custo (maior diferença)
        node_costs.push_back({v, custo_total - custo_sem_v});
    }

    int n_to_remove = generateNumberOfVertexToBeRemove(indi.tour);
    std::vector<int> removed_nodes;

    // 2. Usar pesos para selecionar (Garantir que pesos sejam >= 0)
    std::vector<double> weights;
    for (auto &p : node_costs)
    {
        // Garantimos um peso mínimo para evitar erros na distribuição
        weights.push_back(std::max(0.1, (double)p.second));
    }

    for (int i = 0; i < n_to_remove; ++i)
    {
        std::discrete_distribution<> dist(weights.begin(), weights.end());
        int selected = dist(g);

        // Se o peso for 0, algo deu errado na lógica ou todos são iguais
        if (weights[selected] < 0.0001 && i < n_to_remove)
        {
            // Fallback: seleciona qualquer um que ainda tenha peso > 0
            for (size_t j = 0; j < weights.size(); j++)
                if (weights[j] > 0)
                {
                    selected = j;
                    break;
                }
        }

        removed_nodes.push_back(node_costs[selected].first);

        // Em vez de while, simplesmente zeramos o peso para a próxima iteração
        weights[selected] = 0.0;
    }

    return removed_nodes;
}

std::vector<int> HGA::blockRemovalHeuristic(Individual indi)
{
    int numero_de_vertices_para_remover = generateNumberOfVertexToBeRemove(indi.tour);
    std::vector<int> removed_nodes;

    std::uniform_int_distribution<> distrib(0, indi.tour.size() - 1);
    int tourSize = indi.tour.size();

    int choosedNode = distrib(g);

    for (auto i{0}; i < numero_de_vertices_para_remover; ++i)
    {

        int currentIndex = (choosedNode + i) % tourSize;
        removed_nodes.push_back(indi.tour[currentIndex]);
    }

    return removed_nodes;
}

HGA::HEURISTICS HGA::chooseRemovalHeuristic()
{
    std::uniform_int_distribution<> distrib(0, HEURISTICS_COUNT - 1);
    HEURISTICS escolha = static_cast<HEURISTICS>(distrib(g));

    return escolha;
}

std::vector<int> HGA::ruin(Individual &indi)
{
    std::vector<int> vertexToBeRemoved;

    if (chooseRemovalHeuristic() == HEURISTICS::BLOCK)
    {
        std::cout << "O problema sou eu???" << " " << std::endl;
        vertexToBeRemoved = blockRemovalHeuristic(indi);
        std::cout << "A heurística rodou legal!!!" << " " << std::endl;

        for (auto v : vertexToBeRemoved)
        {
            std::cout << "o vértice que eu vou remover é o " << v << " " << std::endl;
            indi.tour.erase(std::remove(indi.tour.begin(), indi.tour.end(), v), indi.tour.end());
        }
    }
    else
    {

        std::cout << "Não, o problema sou eu" << " " << std::endl;
        vertexToBeRemoved = worstRemovalHeuristic(indi);
        std::cout << "A heurística rodou legal!!!" << " " << std::endl;
        for (auto v : vertexToBeRemoved)
        {
            std::cout << "o vértice que eu vou remover é o " << v << " " << std::endl;
            indi.tour.erase(std::remove(indi.tour.begin(), indi.tour.end(), v), indi.tour.end());
        }

        std::cout << "tour com os vertices removidos" << std::endl;

        for (auto t : indi.tour)
        {
            std::cout << t << " " << std::endl;
        }

        std::cout << "finalizamos o passei" << std::endl;
    }

    return vertexToBeRemoved;
}

long long int HGA::calculateInsertionCost(const std::vector<int> &tour, int pos, int vertex)
{
    int n = tour.size();

    int i_idx = pos;
    int j_idx = (pos + 1) % n;

    int h_idx = (i_idx - 1 + n) % n;
    int k_idx = (j_idx + 1) % n;

    int h = tour[h_idx];
    int i = tour[i_idx];
    int j = tour[j_idx];
    int k = tour[k_idx];

    long long int removedCost = graph->custo[h][i][j] +
                                graph->custo[i][j][k];

    long long int addedCost = graph->custo[h][i][vertex] +
                              graph->custo[i][vertex][j] +
                              graph->custo[vertex][j][k];

    return addedCost - removedCost;
}

void HGA::recreate(Individual &indi, std::vector<int> vertexToBeInserted)
{
    std::cout << "Qual é a tour que eu to recebendo??" << std::endl;
    for (auto s : indi.tour)
    {
        std::cout << s << std::endl;
    }

    shuffle_vertex(vertexToBeInserted);
    // indi.tour.clear();
    for (int vertex : vertexToBeInserted)
    {
        std::cout << "Foi aqui que travou o recreate" << std::endl;
        // Check for duplicates before insertion
        if (std::find(indi.tour.begin(), indi.tour.end(), vertex) != indi.tour.end())
        {
            continue; // Skip if vertex already exists in the tour
        }

        long long int bestDelta = LLONG_MAX;
        int bestPos = -1;

        if (indi.tour.empty())
        {
            indi.tour.push_back(vertex);
            continue;
        }

        for (int i = 0; i < indi.tour.size(); ++i)
        {
            long long int currentDelta = calculateInsertionCost(indi.tour, i, vertex);

            if (currentDelta < bestDelta)
            {
                bestDelta = currentDelta;
                bestPos = i;
            }
        }

        indi.tour.insert(indi.tour.begin() + bestPos + 1, vertex);
    }

    std::cout << "A solução depois que eu recriei" << std::endl;

    for (auto t : indi.tour)
        std::cout << t << std::endl;
}

void HGA::ruinAndRecreate(Individual &indi)
{
    std::cout << "ARRUINANDO" << " " << std::endl;
    std::vector<int> vertex = ruin(indi);

    std::cout << "RECREANDO" << " " << std::endl;
    recreate(indi, vertex);
}

std::vector<int> HGA::lsProcedure(std::vector<int> current_solution)
{
    return LocalSearch(current_solution);
}

std::vector<int> HGA::LocalSearch(std::vector<int> current_solution)
{
    // linha 1
    std::vector<int> solution = current_solution;

    std::vector<int> V = solution;

    std::random_device rd;
    std::mt19937 gen(rd());

    // linha 2
    double p4Opt = 0.1;
    std::uniform_real_distribution<> dis(0.0, 1.0);
    bool use4Opt = (dis(gen) < p4Opt);

    bool imp = true;

    while (imp)
    {
        imp = false;
        std::vector<int> solu_aux = solution;
        int pos{0};
        for (auto u : V)
        {
            std::vector<int> Li = L(pos, u, V);

            for (auto v : Li)
            {
                for (auto i{1}; i <= 7; ++i)
                {

                    if (i == 1)
                    {

                        auto it_p = std::find(solu_aux.begin(), solu_aux.end(), v);
                        solu_aux.erase(it_p);
                        auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                        solu_aux.insert(it_pos + 1, v);
                    }
                    if (i == 2)
                    {

                        auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                        auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                        if (it_v == it_pos + 1)
                        {
                            continue;
                        }
                        else
                        {
                            std::vector<int> arc(it_v, it_v + 1);
                            solu_aux.erase(it_v, it_v + 1);
                            solu_aux.insert(solu_aux.begin() + pos + 1, arc.begin(), arc.end());
                        }
                        if (i == 3)
                        {
                            auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                            auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                            if (it_v == it_pos + 1)
                            {
                                continue;
                            }
                            else
                            {
                                std::swap(solu_aux[pos + 1], v);
                            }
                        }
                        if (i == 4)
                        {
                            auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                            auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                            if (it_v == it_pos + 1)
                            {
                                continue;
                            }
                            else
                            {
                                std::swap(solu_aux[pos + 1], v);
                                solu_aux.insert(solu_aux.begin() + pos + 2, Li[pos + 1]);
                                Li.erase(Li.begin() + pos + 1);
                            }
                        }
                        if (i == 5)
                        {
                            auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                            auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                            if (it_pos + 1 == it_v || it_pos + 2 == it_v)
                            {
                                continue;
                            }
                            else
                            {
                                std::swap(solu_aux[pos + 1], v);
                                solu_aux.erase(solu_aux.begin() + pos + 2);
                            }
                        }
                        if (i == 6)
                        {
                            auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                            auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                            if (it_v == it_pos + 1)
                            {
                                continue;
                            }
                            else
                            {
                                std::swap(solu_aux[pos + 1], v);
                                std::swap(solu_aux[pos + 2], Li[pos + 1]);
                            }
                        }
                        if (i == 7)
                        {
                            auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                            auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                            if (it_v == it_pos + 1)
                            {
                                continue;
                            }
                            else
                            {
                                std::vector<int> sequence;

                                for (auto s{pos + 1}; s <= solu_aux.size(); ++s)
                                {
                                    if (solu_aux[s] == v)
                                    {
                                        sequence.push_back(solu_aux[s]);
                                        solu_aux.erase(solu_aux.begin() + pos + 1, solu_aux.begin() + s);
                                        break;
                                    }
                                    sequence.push_back(solu_aux[s]);
                                }
                                std::reverse(sequence.begin(), sequence.end());
                                solu_aux.insert(solu_aux.begin() + pos, sequence.begin(), sequence.end());
                            }
                        }
                    }

                    if (cost(solu_aux) < cost(solution))
                    {
                        solution = solu_aux;
                        imp = true;
                        break;
                    }
                }
            }

            ++pos;
        }

        if (use4Opt && !imp)
        {
            solu_aux = best4opt(solution);
            if (cost(solu_aux) < cost(solution))
            {
                solution = solu_aux;
                imp = true;
            }
        }
    }

    return solution;
}

std::vector<int> HGA::L(int pos_u, int u, std::vector<int> V)
{
    int n = V.size();
    std::vector<std::pair<int, int>> neighbors;

    for (auto v : V)
    {
        if (v != u)
        {
            neighbors.push_back(std::make_pair(v, pi(pos_u, u, v, V)));
        }
    }

    std::sort(neighbors.begin(), neighbors.end(),
              [](const std::pair<int, int> &a, const std::pair<int, int> &b)
              {
                  return a.second < b.second;
              });

    std::vector<int> result;

    size_t limit = std::min((size_t)gama, neighbors.size());

    for (size_t i = 0; i < limit; ++i)
    {
        result.push_back(neighbors[i].first);
    }

    return result;
}

inline int get_safe(const std::vector<int> &tour, int i)
{
    int n = tour.size();
    return tour[((i % n) + n) % n];
}

// Substitua sua função pred por esta:
int HGA::pred(int pos_u, std::vector<int> tour)
{
    // Usa a função segura para pegar o anterior
    return get_safe(tour, pos_u - 1);
}

long long int HGA::pi(int pos_u, int u, int v, std::vector<int> tour)
{
    return graph->custo[pred(pos_u, tour)][u][v];
}

std::vector<int> HGA::best4opt(std::vector<int> solution)
{
    int n = solution.size();
    if (n < 8)
        return solution;

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

long long int HGA::D2O(int i, int j, std::vector<int> tour)
{
    // Note o uso de get_safe para TODOS os índices que podem ser i-1 ou j+2 (que estoura o tamanho)

    int vi_minus_1 = get_safe(tour, i - 1);
    int vi = get_safe(tour, i);
    int vi_plus_1 = get_safe(tour, i + 1);
    int vi_plus_2 = get_safe(tour, i + 2); // Necessário para a aresta (i, i+1, i+2)

    int vj_minus_1 = get_safe(tour, j - 1);
    int vj = get_safe(tour, j);
    int vj_plus_1 = get_safe(tour, j + 1);
    int vj_plus_2 = get_safe(tour, j + 2);

    // Agora usamos os VÉRTICES seguros para acessar a matriz de custo
    long long int removed =
        graph->custo[vi_minus_1][vi][vi_plus_1] + graph->custo[vi][vi_plus_1][vi_plus_2] + graph->custo[vj_minus_1][vj][vj_plus_1] + graph->custo[vj][vj_plus_1][vj_plus_2];

    long long int added =
        graph->custo[vi_minus_1][vi][vj_plus_1] + graph->custo[vi][vj_plus_1][vj_plus_2] + graph->custo[vj][vi_plus_1][vi_plus_2] + graph->custo[vj_minus_1][vj][vi_plus_1];

    return added - removed;
}

// long long int HGA::delta(int i1, int i2, int j1, int j2){
//     return D2O(i1, j1) + D2O(i2, j2);
// }

bool HGA::Cond(int i1, int i2, int j1, int j2)
{
    return ((i1 + 1 == i2) ? ((i2 + 1 == j1) ? true : ((j1 + 1 == j2) ? true : false)) : false);
}

std::vector<int> HGA::fourOptMove(int i1, int i2, int j1, int j2, std::vector<int> tour)
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

std::pair<int, HGA::Individual *> HGA::selectParent(
    std::vector<std::pair<int, Individual *>> &populationEvaluated)
{

    std::cout << "chego aqui viu" << std::endl;
    // 1. PROTEÇÃO DE POPULAÇÃO VAZIA (Causa comum de crash em rand() % size)
    if (populationEvaluated.empty())
    {
        std::cerr << "ERRO CRÍTICO: A população está vazia em selectParent!" << std::endl;
        // Retorna um par dummy ou encerra para não dar SegFault
        exit(EXIT_FAILURE);
    }

    int tournamentSize = 2;
    // Ajusta o tamanho do torneio se a população for muito pequena
    if (tournamentSize > populationEvaluated.size())
    {
        tournamentSize = populationEvaluated.size();
    }

    std::vector<std::pair<int, Individual *>> tournament;
    tournament.reserve(tournamentSize);

    for (int i = 0; i < tournamentSize; i++)
    {
        int randIndex = std::rand() % populationEvaluated.size();

        // 2. PROTEÇÃO DE PONTEIRO NULO
        if (populationEvaluated[randIndex].second == nullptr)
        {
            std::cerr << "ERRO: Ponteiro nulo encontrado na população no índice " << randIndex << std::endl;
            continue;
        }

        tournament.push_back(populationEvaluated[randIndex]);
    }

    // Se não conseguiu selecionar ninguém (muito improvável), pega o primeiro da população
    if (tournament.empty())
    {
        return populationEvaluated[0];
    }

    std::pair<int, Individual *> best = tournament[0];

    for (auto &competitor : tournament)
    {

        if (competitor.first < best.first)
        {
            best = competitor;
        }
    }

    std::cout << "indo embora" << std::endl;

    return best;
}

HGA::Individual *HGA::generateOffspring(std::pair<int, Individual *> &mother, std::pair<int, Individual *> &father)
{

    std::cout<<"entrei no offspring"<<std::endl;
    int n = mother.second->tour.size();
    std::vector<int> offspring(n, -1);
    std::vector<bool> isInSlice(n + 1000, false); // Ajuste para o valor max das cidades

    std::uniform_int_distribution<int> distribution(0, n - 1);
    int r1 = distribution(engine);
    int r2 = distribution(engine);
    if (r1 > r2) std::swap(r1, r2);

    // 1. Copia o segmento da mãe
    for (int i = r1; i < r2; i++) {
        int city = mother.second->tour[i];
        offspring[i] = city;
        isInSlice[city] = true; 
    }

    // 2. Preenche o resto com o pai
    int currentOffspringPos = r2 % n;
    
    // Percorremos o pai a partir de r2 para manter a ordem (Order Crossover)
    for (int count = 0; count < n; count++) {
        int circularIdx = (r2 + count) % n;
        int fatherCity = father.second->tour[circularIdx];

        if (!isInSlice[fatherCity]) {
            // Se o índice já estiver ocupado pela fatia da mãe, pula
            while (offspring[currentOffspringPos] != -1) {
                currentOffspringPos = (currentOffspringPos + 1) % n;
            }
            
            offspring[currentOffspringPos] = fatherCity;
            currentOffspringPos = (currentOffspringPos + 1) % n;
        }
    }

    Individual *individual = new Individual();
    individual->tour = offspring;

    std::cout<<"estou indo embora do offspring, beijos"<<std::endl;
    return individual;
}

std::pair<int, HGA::Individual *> HGA::crossover(
    std::pair<int, Individual *> &mother,
    std::pair<int, Individual *> &father)
{

    std::cout << "bora pro crossover" << std::endl;

    std::cout<<"offspring 1"<<std::endl;
    auto offspring1 = generateOffspring(mother, father);
    std::cout<<"offspring 2"<<std::endl;
    auto offspring2 = generateOffspring(father, mother);

    std::cout << "gerei o offspring" << std::endl;

    int offspring1Val = biasedFitness(*offspring1);
    int offspring2Val = biasedFitness(*offspring2);

    if (offspring1Val < offspring2Val)
    {
        return std::make_pair(offspring1Val, offspring1);
    }

    std::cout << "indo embora do crossover" << std::endl;

    return std::make_pair(offspring2Val, offspring2);
}
