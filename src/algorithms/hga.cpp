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

const long long int  uElite = 5; 
std::vector<long int> costsTriples;
const int gama = 20;


typedef std::tuple<int, int, int> triple;

void print_tour(std::vector<int> tour){
    for(auto t : tour){
        std::cout << t << std::endl;
    }
}




HGA::HGA(int populationSize, int itMax) : populationSize(populationSize), graph(nullptr), itMax(itMax){
    std::random_device rd;
    g = std::mt19937(rd());
}
void HGA::setGraph(Graph &graph){
    this->graph = &graph;
}

std::vector<int> HGA::run(Graph& graphInput){
    graph = &graphInput;

    initializePopulation();

    std::vector<std::pair<int, Individual*>> evaluatedPopulation = evaluatePopulation(population); 

    std::cout<<"a população evaluada:" << std::endl;

    for(auto e : evaluatedPopulation){
        std::cout << "o custo: " << e.first << std::endl;

        std::cout << "os vértices: " << std::endl;
        for(auto v : e.second->tour){
            std::cout << v << std::endl; 
        }
    }

    std::pair<int, Individual*> bestIndividual = evaluatedPopulation[0];
    std::pair<int, Individual*> worstIndividual = evaluatedPopulation[0];
    int worstIdx = 0;


    std::cout << "o valor de itMax: " << itMax << std::endl;

    
    for (int generation = 0; generation < itMax; ++generation) {
        std::cout<<"entrei pro torneio" << std::endl;
        std::pair<int, Individual*> parentOne= selectParent(evaluatedPopulation);
        std::pair<int, Individual*> parentTwo= selectParent(evaluatedPopulation);

        while(parentTwo == parentOne) std::pair<int, Individual*> parentTwo= selectParent(evaluatedPopulation);
        
        std::cout<<" o parente escolhido foi: " << std::endl;
        std::cout<<"primeiro parente: " << std::endl;
        std::cout<<"custo: " << parentOne.first << std::endl;
        for(auto i : parentOne.second->tour){
            std::cout<<i<< std::endl;
        }
        std::cout<<"segundo parente: " << std::endl;
        std::cout<<"custo: " << parentTwo.first << std::endl;
        for(auto i : parentTwo.second->tour){
            std::cout<<i<< std::endl;
        }
      
    

    }

    return population[0].tour;
    
}

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

    std::cout << "Solução antes da diversificação" << " " << std::endl;

    for(auto s : solution){
        std::cout << s << " " << std::endl;
    }
    // diversificar

    std::cout << "Vou arruinar e recriar" << " " << std::endl;
    ruinAndRecreate(individual);

    auto solution2 = individual.tour;
    
    std::cout << "Solução depois da diversificação" << " " << std::endl;
    for(auto s : solution2){
        std::cout << s << " " << std::endl;
    }

    //intensificar

    individual.tour = lsProcedure(solution2);

     std::cout << "Solução depois da intensificação" << " " << std::endl;
    for(auto s : individual.tour){
        std::cout << s << " " << std::endl;
    }
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

    std::cout<< "A tour que eu vou calcular o custo é: "<<std::endl;
    for(auto t : tour){
        std::cout << t << std::endl;
    }

    std::cout<<"sou eu aqui que to dando problema viu" << std::endl;
    long long int totalCost{0}; 

    std::vector<int> custo;
    
    int n = tour.size(); 


    for(auto i{0}; i < n-2; ++i){
        std::cout<< "totalCost+= graph->custo[tour[i]][tour[i+1]][tour[i+2]] = " << graph->custo[tour[i]][tour[i+1]][tour[i+2]] << std::endl;
        totalCost+= graph->custo[tour[i]][tour[i+1]][tour[i+2]];

        std::cout << "tour[i] : " << tour[i] << " tour[i+1] : " << tour[i+1] << " tour[i+2] : " << tour[i+2] << std::endl;
    }


    std::cout<< "totalCost+= graph->custo[tour[n-2]][tour[n-1]][tour[0]] = " << graph->custo[tour[n-2]][tour[n-1]][tour[0]] << std::endl;
    totalCost+= graph->custo[tour[n-2]][tour[n-1]][tour[0]];
    std::cout<< "totalCost+= graph->custo[tour[n-1]][tour[0]][tour[1]] =  " << graph->custo[tour[n-1]][tour[0]][tour[1]] << std::endl;
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

    std::cout<<"O problema é aqui claramente"<<std::endl;

    std::sort(copyPopulation.begin(), copyPopulation.end(), 
    [](const Individual *a, const Individual *b)
        {
            return a->cost < b->cost;
        });

    
        int cont{0}; 
        for(auto i : copyPopulation){
            //seta o rank de cada individuo na populção original
            i->costRank = cont++; 
        }

    std::cout<<"terminei de setar o rank do custo" << std::endl;

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
    // pq ta retonarnando negativo

    std::cout<< "o cálculo é: fc(individual) : " << fc(individual) << "+ " << "(1 - (static_cast<double>(uElite)/populationSize)) : " <<  (1 - (static_cast<double>(uElite)/populationSize)) <<  " * fd(individual)): " << fd(individual) << std::endl;
    return (fc(individual) + (1 - (static_cast<double>(uElite)/populationSize)) * fd(individual));
}

std::vector<std::pair<int, HGA::Individual*>> HGA::evaluatePopulation(std::vector<HGA::Individual> &population){

    std::cout<<"oi chegamos aqui e o problema sou eu?"<<std::endl;
    std::vector<std::pair<int, HGA::Individual*>> evaluatedPopulation;
    int  cost{0};

    for(auto& i : population){
        cost = 0;
        cost = biasedFitness(i);
        std::cout << "O custo depois da função fitness: " << cost << std::endl;
        evaluatedPopulation.push_back(std::make_pair(cost, &i));
    }

    std::sort(evaluatedPopulation.begin(), evaluatedPopulation.end(),
    [] (const std::pair<int, Individual*> &a, const std::pair<int, Individual*> &b)
    {
        return a.first < b.first;
    });

    std::cout<<"o problema não sou eu não viu thuanny"<<std::endl;

    return evaluatedPopulation; 

}


void HGA::initializePopulation(){
    createPopulation();
    individualDiversityRank();
    individualCostRank();
}


int HGA::generateNumberOfVertexToBeRemove(std::vector<int> tour){
    int numero_de_nos = tour.size();
    std::cout << "qual o número de nós? "<< numero_de_nos  << " " << std::endl;
    int p_min = 1;
    int p_max = std::min(40, numero_de_nos/3);

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(p_min, p_max);

    int retorno = distrib(gen);
    
    std::cout << "o retorno é? "<< retorno  << " " << std::endl;

    return retorno;
}


std::vector<int> HGA::worstRemovalHeuristic(HGA::Individual indi){
    std::cout << "entramos na heurística de remoção do pior!" << " " << std::endl;
    auto custo_tour = cost(indi.tour);
    std::cout << "calculei o custo? " << custo_tour << " " << std::endl;
    std::vector<std::pair<int,int>> costs;
    std::cout << "bora gerar o número de vértices para remover?? "  << " " << std::endl;
    int numero_de_vertices_para_remover = generateNumberOfVertexToBeRemove(indi.tour);
    std::cout << "o número de vértices para remover " << numero_de_vertices_para_remover << " " << std::endl;
    std::vector<int> removed_nodes;

    std::cout << "Até aqui tudo bem!" << " " << std::endl;

    for(auto v : indi.tour){
        Individual aux;
        aux.tour = indi.tour;
        int custo_sem_v = cost(aux.tour); 
        costs.emplace_back(std::make_pair(v, custo_tour - custo_sem_v));
    }

    std::cout << " aqui tudo bem também!" << " " << std::endl;

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
        std::cout << " aqui???" << " " << std::endl;

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

std::vector<int> HGA::ruin(Individual &indi){
    std::vector<int> vertexToBeRemoved;

     
    if(chooseRemovalHeuristic() == HEURISTICS::BLOCK){
        std::cout << "O problema sou eu???" << " " << std::endl;
        vertexToBeRemoved = blockRemovalHeuristic(indi);
        std::cout << "A heurística rodou legal!!!" << " " << std::endl;

        for(auto v : vertexToBeRemoved){
            std::cout << "o vértice que eu vou remover é o "<< v << " " << std::endl;
            indi.tour.erase(std::remove(indi.tour.begin(), indi.tour.end(), v), indi.tour.end());
        }
    } else{

        std::cout << "Não, o problema sou eu" << " " << std::endl;
        vertexToBeRemoved = worstRemovalHeuristic(indi);
        std::cout << "A heurística rodou legal!!!" << " " << std::endl;
        for(auto v : vertexToBeRemoved){
            std::cout << "o vértice que eu vou remover é o "<< v << " " << std::endl;
            indi.tour.erase(std::remove(indi.tour.begin(), indi.tour.end(), v), indi.tour.end());
        }

        std::cout << "tour com os vertices removidos" << std::endl;

        for(auto t : indi.tour){
            std::cout << t << " " << std::endl;
        }

        std::cout << "finalizamos o passei" << std::endl;
    }

    return vertexToBeRemoved;
}

long long int HGA::calculateInsertionCost(const std::vector<int>& tour, int pos, int vertex) {
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

void HGA::recreate(Individual &indi, std::vector<int> vertexToBeInserted){
    std::cout<< "Qual é a tour que eu to recebendo??" <<std::endl;
    for(auto s : indi.tour){
        std::cout<< s << std::endl;
    }

    std::vector<int> vertex = indi.tour;
    shuffle_vertex(vertexToBeInserted);
    // indi.tour.clear();
    

    for (int vertex : vertexToBeInserted) {
        long long int bestDelta = LLONG_MAX;
        int bestPos = -1;

        for (int i = 0; i < indi.tour.size(); ++i) {
            long long int currentDelta = calculateInsertionCost(indi.tour, i, vertex);
            
            if (currentDelta < bestDelta) {
                bestDelta = currentDelta;
                bestPos = i;
            }
        }
        
        indi.tour.insert(indi.tour.begin() + bestPos + 1, vertex);
    }

    std::cout<<"A solução depois que eu recriei" << std::endl;

    for(auto t : indi.tour) std::cout<< t << std::endl;
}


void HGA::ruinAndRecreate(Individual& indi) {
    std::cout << "ARRUINANDO" << " " << std::endl;
    std::vector<int> vertex = ruin(indi);

     std::cout << "RECREANDO" << " " << std::endl;
    recreate(indi, vertex);
}


std::vector<int> HGA::lsProcedure(std::vector<int> current_solution){
    return LocalSearch(current_solution);
}

std::vector<int> HGA::LocalSearch (std::vector<int> current_solution){
    // linha 1 
    std::vector<int> solution =  current_solution;

    std::vector<int> V = solution;

    std::random_device rd;
    std::mt19937 gen(rd());

    // linha 2
    double p4Opt = 0.1;
    std::uniform_real_distribution<> dis(0.0, 1.0);
    bool use4Opt = (dis(gen) < p4Opt);

    bool imp = true; 

    
    std::cout<< "Os vértices V é: " << std::endl;
                        for(auto s : V){
                            std::cout<< s << std::endl;
                        }
    std::cout << "vou entrar no while, me aguarde" << std::endl;
    while(imp){
        imp = false; 
        std::vector<int> solu_aux = solution; 
        int pos{0};
        for(auto u: V){
            std::vector<int> Li = L(pos,u, V);
            std::cout<< "A Lista Li é: " << std::endl;
                        for(auto s : Li){
                            std::cout<< s << std::endl;
                        }
            
            for(auto v: Li){
                for(auto i{1}; i <= 7; ++i){
                    std::cout<< "valor de i é: " << i << std::endl;
                    if(i == 1){
                        std:: cout<< "v é: " << v << std::endl;
                        std:: cout<< "u é: " << u << std::endl;
                        auto it_p = std::find(solu_aux.begin(), solu_aux.end(), v);
                        solu_aux.erase(it_p);
                        auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                        solu_aux.insert(it_pos+1, v);
                        std::cout<< "A solução é: " << std::endl;
                        for(auto s : solu_aux){
                            std::cout<< s << std::endl;
                        }
                    }
                    if(i == 2){
                        std:: cout<< "v é: " << v << std::endl;
                        std:: cout<< "u é: " << u << std::endl;
                        auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                        auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                        if(it_v == it_pos+1){
                            continue;
                        }else{
                            std::vector<int> arc(it_v, it_v+1);
                            solu_aux.erase(it_v, it_v+1);
                            solu_aux.insert(solu_aux.begin() + pos + 1, arc.begin(), arc.end());
                        }
                        std::cout<< "A solução é: " << std::endl;
                        for(auto s : solu_aux){
                            std::cout<< s << std::endl;
                        }
                    }
                    if(i == 3){
                         auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                        auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                        if(it_v == it_pos+1){
                            continue;
                        }else{
                            std::swap(solu_aux[pos+1], v); 
                        }
                        
                         std::cout<< "A solução é: " << std::endl;
                        for(auto s : solu_aux){
                            std::cout<< s << std::endl;
                        }
                    }
                    if(i == 4){
                        auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                        auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                        if(it_v == it_pos+1){
                            continue;
                        } else{
                            std::swap(solu_aux[pos+1], v);
                            solu_aux.insert(solu_aux.begin()+pos+2,Li[pos+1]);
                            Li.erase (Li.begin()+pos+1);
                        }
                        
                         std::cout<< "A solução é: " << std::endl;
                        for(auto s : solu_aux){
                            std::cout<< s << std::endl;
                        }
                    }
                    if(i == 5){
                        auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                        auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                        if(it_pos+1 == it_v || it_pos+2 == it_v){
                            continue;
                        }else{
                            std::swap(solu_aux[pos+1], v);
                            solu_aux.erase (solu_aux.begin()+pos+2);
                        }
                        
                         std::cout<< "A solução é: " << std::endl;
                        for(auto s : solu_aux){
                            std::cout<< s << std::endl;
                        }
                    }
                    if(i == 6){
                        auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                        auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                        if(it_v == it_pos+1){
                            continue;
                        }else{
                            std::swap(solu_aux[pos+1], v);
                            std::swap(solu_aux[pos+2], Li[pos+1]);
                        }
                        
                        std::cout<< "A solução é: " << std::endl;
                        for(auto s : solu_aux){
                            std::cout<< s << std::endl;
                        }
                    }
                    if(i ==7){
                        auto it_v = std::find(solu_aux.begin(), solu_aux.end(), v);
                        auto it_pos = std::find(solu_aux.begin(), solu_aux.end(), u);
                        if(it_v == it_pos+1){
                            continue;
                        }else{
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

                        

                         std::cout<< "A solução é: " << std::endl;
                        for(auto s : solu_aux){
                            std::cout<< s << std::endl;
                        }
                    }

                    if(cost(solu_aux) < cost(solution)){
                        solution = solu_aux;
                        imp = true;
                        break;
                    }

                     std::cout << "passei por tudo bonitinho" << std::endl;


                }
            }
            
            ++pos;

            std::cout << "a posição é: " << pos<< std::endl;
            std::cout<< "A solução depois da posição é: " << std::endl;
                        for(auto s : solu_aux){
                            std::cout<< s << std::endl;
                        }
        }

        std::cout << "acabou a putaria!!! " << pos<< std::endl;

        if(use4Opt && !imp){
            solu_aux = best4opt(solution);
            if(cost(solu_aux) < cost(solution)){
                solution = solu_aux;
                imp = true;
            }
        }

    }

    std::cout<< "A solução retornada é: " << std::endl;
                        for(auto s : solution){
                            std::cout<< s << std::endl;
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

std::vector<int> fourOptMove(int i1, int i2, int j1, int j2, std::vector<int> tour){
    // dividir a tour em quatro partes
    std::vector<int> parte_um;
    std::vector<int> parte_dois;
    std::vector<int> parte_tres;
    std::vector<int> parte_quatro;

    int n = static_cast<int>(tour.size());
    if (n == 0) return {};

    auto append_range = [&](int start, int end, std::vector<int> &dest) {
        int idx = start;
        while (true) {
            dest.push_back(tour[idx]);
            if (idx == end) break;
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

std::pair<int, HGA::Individual*> HGA::selectParent(
    std::vector<std::pair<int, Individual*>> &populationEvaluated)
{
   int tournamentSize = 2;
    std::vector<std::pair<int, Individual*>> tournament(tournamentSize);
    
    for(int i = 0; i < tournamentSize; i++) {
        int randIndex = std::rand() % populationEvaluated.size();
        tournament[i] = populationEvaluated[randIndex];
    }

    std::pair<int, Individual*> best = tournament[0];


    for(auto &competitor : tournament) {

        std::cout<<"O torneio  é: " << std::endl;
        print_tour(competitor.second->tour);
        std::cout << "o valor a ser comparado é " << competitor.first << " e o melhor: " << best.first << std::endl;

        if(competitor.first < best.first) {
            best = competitor;
        }
    }

    return best;
}









