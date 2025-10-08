#include "../../include/algorithms/hga.h"
#include <numeric>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <string>
#include <vector>

const long long int  uElite = 1000000; 
std::vector<long int> costsTriples;


typedef std::tuple<int, int, int> triple;

enum HEURISTICS {
    WORST,
    BLOCK,
    HEURISTICS_COUNT
};



HGA::HGA(int populationSize) : populationSize(populationSize), graph(nullptr){
    std::random_device rd;
    g = std::mt19937(rd());
}
void HGA::setGraph(Graph &graph){
    this->graph = &graph;
}

std::vector<int> HGA::run(Graph& graph){}

HGA::~HGA() {}
// criar um individuo da população
HGA::Individual HGA::createIndividuals(){
    // grafo da run do TspSlver
    //numero de nós do grafo que estamos recebendo
    int n = graph->getMaxM();

    std::cout << "O n é " << n << std::endl;

    // criando um individuo
    Individual individual;

    // lista de indivíduos
    std::vector<Individual> population;

    // solução: uma tour qtsp formada por triplas válidas
    std::vector<int> solution;

    std::vector<int> vertices = getVertex();

    std::cout << "os vértices são: " << vertices.size() << std::endl;
    
    std::cout << "os vértices antes do shuffle são: "  << std::endl;
    for(auto v : vertices) {
        std::cout << v   << std::endl;
    }

    std::cout << "acabou os vértices antes do shuffle"  << std::endl;

    shuffle_vertex(vertices); 

    // antes da inserção dos vértices na solução é necessário diversificar com o operador de mutação e intensificar com a busca local
    // aqui só estamos inserindo os vértices aleatoriamente na solução

    for(auto v : vertices){
        solution.push_back(v);
        std::cout << "VÉRTICE: " << v << std::endl; 
    }

    individual.tour = solution;

    setIndividualCost(individual);
    // setIndividualDiversityContribution(individual);

    return individual;
}

std::vector<int> HGA::getVertex(){
    int n = graph->getNumVertex();
    std::vector<int> vertex(n);
    int cont{0};
    for(auto &v: vertex){
        v = cont++; 
    }

    return vertex;

}

void HGA::shuffle_vertex(std::vector<int> &vertex){
    std::shuffle(vertex.begin(), vertex.end(), g); 
}


void HGA::createPopulation(){
    for(auto i{0}; i < populationSize; ++i){
        population.push_back(createIndividuals());
    }
}



long long int HGA::cost (const HGA::Individual& individual){

    long long int totalCost{0}; 

    std::vector<int> custo;
    
    int n = individual.tour.size(); 


    for(auto i{0}; i < n-2; ++i){
        std::cout<< "o custo do grafo é: " << graph->custo[individual.tour[i]][individual.tour[i+1]][individual.tour[i+2]] << std::endl;
        
        totalCost+= graph->custo[individual.tour[i]][individual.tour[i+1]][individual.tour[i+2]];
        std::cout<< "vértices do custo: " << individual.tour[i] << " " << individual.tour[i+1] << " " << individual.tour[i+2] << std::endl;
        std::cout<< "a variável totalcost é: " << totalCost << std::endl;
        
    }
    std::cout<< "vértices do custo  N-2: " << individual.tour[n-2] << " " << individual.tour[n-1] << " " << individual.tour[0] << std::endl;
    std::cout<< "o custo do grafo n -2 é: " << graph->custo[individual.tour[n-2]][individual.tour[n-1]][individual.tour[0]] << std::endl;
    totalCost+= graph->custo[individual.tour[n-2]][individual.tour[n-1]][individual.tour[0]];
    std::cout<< "a variável totalcost é: " << totalCost << std::endl;
    std::cout<< "vértices do custo N-1: " << individual.tour[n-1] << " " <<individual.tour[0] << " " <<individual.tour[1] << std::endl;
    std::cout<< "o custo do grafo n -1 é: " <<  graph->custo[individual.tour[n-1]][individual.tour[0]][individual.tour[1]]<< std::endl;
    totalCost+= graph->custo[individual.tour[n-1]][individual.tour[0]][individual.tour[1]];
    std::cout<< "a variável totalcost é: " << totalCost << std::endl;
    std::cout<< "o custo total do grafo é: " << totalCost << std::endl;
    return totalCost; 
}

void HGA::setIndividualCost ( HGA::Individual& individual ){
    individual.cost = cost(individual);
    std::cout<< "o custo do set é: " << individual.cost << std::endl;
} 

void HGA::setIndividualDiversityContribution ( HGA::Individual& individual ){
    individual.diversityContribution = dc(individual);
} 



void HGA::setIndividualCostRank ( HGA::Individual& individual ){
    individual.costRank = cost(individual);
}


// criar uma lista com a referencia dos individuos da população
std::vector<HGA::Individual*> HGA::generateCopyPopulation (){

    std::vector<Individual*> copyPopulation; 

    for(auto &i : population ){
        copyPopulation.push_back(&i);
    }

    return copyPopulation; 
}


void HGA::individualCostRank(){

    std::vector<Individual*> copyPopulation = generateCopyPopulation(); 


    //seta o custo de cada individuo na populção original
    for(auto individual : copyPopulation){
        setIndividualCost(*individual);
    }

    std::sort(copyPopulation.begin(), copyPopulation.end(), 
    [](const Individual *a, const Individual *b)
        {
            return a->cost < b->cost;
        });

    
        int cont{0}; 
        for(auto i : copyPopulation){
            //seta o rank de cada individuo na populção original
            i->costRank = ++cont; 
        }

}

std::vector<HGA::Individual>& HGA::getPopulation(){
    return this->population; 
}

void HGA::individualDiversityRank ( ){

    std::vector<Individual*> copyPopulation = generateCopyPopulation(); 


    //seta o custo de cada individuo na populção original
    for(auto individual : copyPopulation){
        setIndividualDiversityContribution(*individual);
    }
 // lista de triplas (dividindo o grafo que eu estou recebendo em todas as triplas possíveis para montar todos os ciclos hamiltonianos possíveis)
    std::vector<triple> triples;
    std::sort(copyPopulation.begin(), copyPopulation.end(), 
    [](const Individual *a, const Individual *b)
        {
            return a->diversityContribution < b->diversityContribution;
        });

    
        int cont{0}; 
        for(auto i : copyPopulation){
            //seta o rank de cada individuo na populção original
            i->diversityRank = ++cont; 
        }

}

int HGA::fc(HGA::Individual individual) {
    return individual.costRank;
}

double HGA::fd(HGA::Individual individual) {
    return individual.diversityRank;
}

std::vector<std::pair<int,int>> HGA::generateAllNodesPairs(HGA::Individual individual){
    std::vector<std::pair<int,int>> pairs;

    for(auto i{0}; i < individual.tour.size(); ++i){
        pairs.push_back(std::make_pair(individual.tour[i],individual.tour[i+1]));
    }

    return pairs;
}

 double HGA::dc (HGA::Individual Pi){
    // a variável é setada assim no artigo, ver uma maneira de receber melhor a variável (colocar como atributo da classe)
    int n_close = 2;

    double resultSum{0};
    for(auto j{1}; j <= n_close; ++j){
        Individual Pj = population[j];
        resultSum+=normalizeBronkenPairsDistance(Pi, Pj);
    }
    return (1.0/n_close)*resultSum;

 }


double HGA::normalizeBronkenPairsDistance(HGA::Individual Pi, HGA::Individual Pj){
    int n = graph->getMaxM();
    return (1.0/n) * numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj(Pi, Pj);
}

long int HGA::numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj(HGA::Individual Pi, HGA::Individual Pj){
    long int numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj{0};
    std::vector<std::pair<int,int>> pairsPi = generateAllNodesPairs(Pi);
    std::vector<std::pair<int,int>> pairsPj = generateAllNodesPairs(Pj);

    for(auto i : pairsPi){
        const auto search = std::find(pairsPj.begin(),pairsPj.end(), i);
        const auto unsucceeded = pairsPj.end();
        if( search != unsucceeded) numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj++;
    }
    return numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj; 
}

double HGA::biasedFitness (HGA::Individual individual){
    return (fc(individual) + (1 - (uElite/populationSize)) * fd(individual));
}

std::vector<std::pair<int, HGA::Individual*>> HGA::evaluatePopulation(std::vector<HGA::Individual> &population, Graph &graph){
    std::vector<std::pair<int, HGA::Individual*>> evaluatedPopulation;
    int  cost{0};

    for(auto i : population){
        cost = biasedFitness(i);
        evaluatedPopulation.push_back(std::make_pair(cost, &i));
    }

    std::sort(evaluatedPopulation.begin(), evaluatedPopulation.end(),
    [] (const std::pair<int, Individual*> &a, const std::pair<int, Individual*> &b)
    {
        return a.first < b.first;
    });

    return evaluatedPopulation; 

}

std::vector<std::pair<int, HGA::Individual*>> HGA::initializePopulation(Graph &graph){
    createPopulation();
    individualDiversityRank();
    // individualCostRank();

    return evaluatePopulation(getPopulation(), graph);

}


int HGA::generateNumberOfVertexToBeRemove(std::vector<int> tour){
    int numero_de_nos = tour.size();
    int p_min = 5;
    int p_max = std::min(40, numero_de_nos/3);

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(p_min, p_max);

    return distrib(gen);
}


std::vector<int> HGA::worstRemovalHeuristic(HGA::Individual indi){
    auto custo_tour = cost(indi);
    std::vector<std::pair<int,int>> costs;
    int numero_de_vertices_para_remover = generateNumberOfVertexToBeRemove(indi.tour);
    std::vector<int> removed_nodes;

    for(auto v : indi.tour){
        Individual aux;
        aux.tour = indi.tour;
        aux.tour.erase(std::remove(aux.tour.begin(), aux.tour.end(), v), aux.tour.end());
        int custo_sem_v = cost(aux); 
        costs.emplace_back(std::make_pair(v, custo_tour - custo_sem_v));
    }

    std::vector<int> weights;
    for (const auto& pair : costs) {
        weights.push_back(pair.second);
    }
    std::discrete_distribution<> distribution(weights.begin(), weights.end());


    std::vector<bool> already_removed(indi.tour.size(), false);
    for (int i = 0; i < numero_de_vertices_para_remover; ++i) {

        int selected_tour_index = distribution(g);


        while (already_removed[selected_tour_index]) {
            selected_tour_index = distribution(g);
        }

        removed_nodes.push_back(costs[selected_tour_index].first);
        already_removed[selected_tour_index] = true;

        weights[selected_tour_index] = 0.0;
        distribution.param(std::discrete_distribution<>::param_type(weights.begin(), weights.end()));
    }

    return removed_nodes;
}

std::vector<int> HGA::blockRemovalHeuristic(Individual indi){
    int numero_de_vertices_para_remover = generateNumberOfVertexToBeRemove(indi.tour);
    std::vector<int> removed_nodes;

    std::uniform_int_distribution<> distrib(0, indi.tour.size() - 1);
    
    int choosedNode = distrib(g);

    for(auto i{choosedNode}; i <= choosedNode+numero_de_vertices_para_remover; ++i) removed_nodes.push_back(indi.tour[i]);

    return removed_nodes;
}

int HGA::chooseRemovalHeuristic(){
    std::uniform_int_distribution<> distrib(0, HEURISTICS_COUNT -1); 
    HEURISTICS escolha = static_cast<HEURISTICS>(distrib(g));

    return escolha;
}
