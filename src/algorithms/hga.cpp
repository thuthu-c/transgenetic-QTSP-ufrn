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
const int gama = 20;


typedef std::tuple<int, int, int> triple;




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


    shuffle_vertex(vertices); 

    // antes da inserção dos vértices na solução é necessário diversificar com o operador de mutação e intensificar com a busca local
    // aqui só estamos inserindo os vértices aleatoriamente na solução

    for(auto v : vertices){
        solution.push_back(v);
        std::cout << "VÉRTICE: " << v << std::endl; 
    }

    individual.tour = solution;

    setIndividualCost(individual);

    std::cout<<"custo do individuo: " << individual.cost << std::endl; 

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

    for(auto p : population){
        setIndividualDiversityContribution(p);
         std::cout<<"diversidade do individuo: " << p.diversityContribution << std::endl; 
    }
}



long long int HGA::cost (std::vector<int> tour){

    long long int totalCost{0}; 

    std::vector<int> custo;
    
    int n = tour.size(); 


    for(auto i{0}; i < n-2; ++i){
        
        totalCost+= graph->custo[tour[i]][tour[i+1]][tour[i+2]];
    }

    totalCost+= graph->custo[tour[n-2]][tour[n-1]][tour[0]];
    totalCost+= graph->custo[tour[n-1]][tour[0]][tour[1]];
    return totalCost; 
}

void HGA::setIndividualCost ( HGA::Individual& individual ){
    individual.cost = cost(individual.tour);
    std::cout<< "o custo do set é: " << individual.cost << std::endl;
} 

void HGA::setIndividualDiversityContribution ( HGA::Individual& individual ){
    individual.diversityContribution = dc(individual);
} 



void HGA::setIndividualCostRank ( HGA::Individual& individual ){
    individual.costRank = cost(individual.tour);
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

//acesso a tour e lido com os vértices de cada tour
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
    individualCostRank();

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
    auto custo_tour = cost(indi.tour);
    std::vector<std::pair<int,int>> costs;
    int numero_de_vertices_para_remover = generateNumberOfVertexToBeRemove(indi.tour);
    std::vector<int> removed_nodes;

    for(auto v : indi.tour){
        Individual aux;
        aux.tour = indi.tour;
        int custo_sem_v = cost(aux.tour); 
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

HGA::HEURISTICS HGA::chooseRemovalHeuristic(){
    std::uniform_int_distribution<> distrib(0, HEURISTICS_COUNT -1); 
    HEURISTICS escolha = static_cast<HEURISTICS>(distrib(g));

    return escolha;
}

void HGA::ruin(Individual indi){
    std::vector<int> vertexToBeRemoved;

    if(chooseRemovalHeuristic() == HEURISTICS::BLOCK){
        vertexToBeRemoved = blockRemovalHeuristic(indi);

        for(auto v : vertexToBeRemoved){
            indi.tour.erase(std::remove(indi.tour.begin(), indi.tour.end(), v), indi.tour.end());
        }
    } else{
        vertexToBeRemoved = worstRemovalHeuristic(indi);
        for(auto v : vertexToBeRemoved){
            indi.tour.erase(std::remove(indi.tour.begin(), indi.tour.end(), v), indi.tour.end());
        }
    }
}


void HGA::recreate(Individual indi){

    std::vector<int> vertex = indi.tour;
    shuffle_vertex(vertex);
    indi.tour.clear();

    for(auto v : vertex) indi.tour.push_back(v);
}


void HGA::ruinAndRecreate(Individual indi) {
    ruin(indi);

    recreate(indi);
}


void HGA::lsProcedure(){

}

std::vector<int> HGA::LocalSearch (Individual indi){
    // linha 1 
    std::vector<int> solution = indi.tour;

    std::vector<int> V = solution;

    std::random_device rd;
    std::mt19937 gen(rd());

    // linha 2
    double p4Opt = 0.1;
    std::uniform_real_distribution<> dis(0.0, 1.0);
    bool use4Opt = (dis(gen) < p4Opt);

    bool imp = true; 

    int pos{0};

    while(imp){
        imp = false; 
        std::vector<int> solu_aux = solution; 
        for(auto u: V){
            std::vector<int> Li = L(pos,u, V);
            for(auto v: Li){
                for(auto i{1}; i <= 7; ++i){
                    if(i == 1){
                        solu_aux.insert(solu_aux.begin()+pos+1, v);
                    }
                    if(i == 2){
                        int arc[] ={v,Li[pos]};
                        std::vector<int> arcs(arc, arc+ sizeof(arc) / sizeof(int));
                        solu_aux.insert(solu_aux.begin()+pos+1, arcs.begin(), arcs.end());
                    }
                    if(i == 3){
                        std::swap(solu_aux[pos+1], v); 
                    }
                    if(i == 4){
                        std::swap(solu_aux[pos+1], v);
                        solu_aux.insert(solu_aux.begin()+pos+2,Li[pos+1]);
                        Li.erase (Li.begin()+pos+1);
                    }
                    if(i == 5){
                        std::swap(solu_aux[pos+1], v);
                        Li.insert(Li.begin()+pos+1,solu_aux[pos+2]);
                        solu_aux.erase (solu_aux.begin()+pos+2);
                    }
                    if(i == 6){
                        std::swap(solu_aux[pos+1], v);
                        std::swap(solu_aux[pos+2], Li[pos+1]);
                    }
                    if(i ==7){
                        std::vector<int> sequence;

                        for(auto s{pos+1}; s <= solu_aux.size(); ++s){
                            if(solu_aux[s] == v){
                                sequence.push_back(solu_aux[s]);
                                solu_aux.erase(solu_aux.begin()+pos+1,solu_aux.begin()+s);
                                break;
                            }
                            sequence.push_back(solu_aux[s]);
                        }
                        std::reverse(sequence.begin(),sequence.end());
                        solu_aux.insert(solu_aux.begin()+pos, sequence.begin(), sequence.end());
                    }

                    if(cost(solu_aux) < cost(solution)){
                        solution = solu_aux;
                        imp = true;
                        break;
                    }
                }
            }
            
            ++pos;
        }

        if(use4Opt && !imp){
            solu_aux = best4opt(solution);
            if(cost(solu_aux) < cost(solution)){
                solution = solu_aux;
                imp = true;
            }
        }

    }

    
    return solution;
}

std::vector<int> HGA::L (int pos_u, int u, std::vector<int> V){
    int n = V.size();
    std::vector<std::pair<int, int>> neighbors;

    for(auto v : V){
        if(v != u){
            neighbors.push_back(std::make_pair(v, pi(pos_u, u, v, V))); 
        }
    }

    std::sort(neighbors.begin(), neighbors.end(),
    [] (const std::pair<int, int> &a, const std::pair<int, int> &b)
    {
        return a.second < b.second;
    });

    
    std::vector<int> result;
 
    size_t limit = std::min((size_t)gama, neighbors.size()); 

    for (size_t i = 0; i < limit; ++i) {
        result.push_back(neighbors[i].first);
    }
    
    return result;

}


//retornar o vértice predecessor de u
int HGA::pred ( int pos_u,std::vector<int> tour){
    if( pos_u == 0){
        return tour[tour.size()-1];
    }else{

        return tour[pos_u-1];
    }


}

long long int HGA::pi(int pos_u,int u, int v, std::vector<int> tour ){
    return graph->custo[pred(pos_u, tour)][u][v];
}


//guardar o melhor a tour com o melhor movimento 4opt baseado no menor delta delta
std::vector<int> HGA::best4opt(std::vector<int> solution){
    double BEST = std::numeric_limits<double>::infinity(); 
    int n = solution.size();
    // if(Cond(i1, i2, j1, j2)){
    //     BEST = std::min(BEST, static_cast<double>(delta(i1, i2, j1, j2)));
    // }

    std::vector<std::vector<double>> F(n + 1, std::vector<double>(n + 1));
    std::vector<std::vector<double>> DeltaStar(n + 1, std::vector<double>(n + 1));

    for(int j1= 5; j1 <= n-3; ++j1){
        F[7][j1] = D2O(1, j1);

        for(int i2 = 4; i2 <= j1 -2; ++i2){
            F[i2][j1] =  std::min(F[i2 - 1][j1], static_cast<double>(D2O(i2 - 2, j1)));
        }
    }

    for(int i2 = 3; i2 <= n -5; ++i2){
        DeltaStar[i2][i2 + 4] = F[i2][i2 + 2]; 

        for(int j2 = i2 + 5; j2 <= n-1; ++j2){
            DeltaStar[i2][j2] = std::min(DeltaStar[i2][j2 - 1], F[i2][j2 - 2]);

            double Delta_Best_current = D2O(i2, j2) + DeltaStar[i2][j2];

            BEST = std::min(BEST, Delta_Best_current);
        }
        
    }

    // No 4-opt move implemented yet; return the (possibly unchanged) solution
    return solution;
}

long long int HGA::D2O (int i, int j){
    return graph->custo[i-1][i][j+1] + graph->custo[i][j+1][j+2]
         + graph->custo[j][i+1][i+2] + graph->custo[j-1][j][i+1]
         - graph->custo[i-1][i][i+1] - graph->custo[i][i+1][i+2]
         - graph->custo[j-1][j][j+1] - graph->custo[j][j+1][j+2];
}

long long int HGA::delta(int i1, int i2, int j1, int j2){
    return D2O(i1, j1) + D2O(i2, j2);
}

bool HGA::Cond(int i1, int i2, int j1, int j2){
    return ((i1 + 1 == i2) ?  ((i2 + 1 == j1 ) ? true : ((j1 + 1 == j2 ) ? true : false)): false);
}

std::vector<int> HGA::FourOptNeighborhood(std::vector<int> tour){

}








