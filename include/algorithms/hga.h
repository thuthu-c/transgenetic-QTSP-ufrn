/**
 * Implementation of HGA, a liter algorithm
 * @author Thuanny Carvalho Rolim de Albuquerque
 * @date September 14th, 2025
 * @file hga.h
 */ 

#ifndef _HGA_H_
#define _HGA_H_

#include "../algorithms/tsp_solver.h" 
#include "../data_structures/graph.h"
#include <vector>
#include <algorithm>
#include <random>

class HGA : public TspSolver
{  

    //fazer duas versões: uma com randomização dos vértices 

    protected: 
    int populationSize; 
    Graph *graph;
    int itMax;
    
    public:
    std::mt19937 g;
    // cada individuo da população inicial é representado por uma QTSP tour
    struct Individual {
        std::vector<int> tour;
        int cost;
        int costRank; 
        double diversityContribution;
        int diversityRank;
    };

    enum HEURISTICS {
    WORST,
    BLOCK,
    HEURISTICS_COUNT
    };

    std::vector<Individual> population;
    std::vector<Individual>& getPopulation();
    Individual* generateOffspring(std::pair<int, Individual*> &mother,std::pair<int, Individual*>&father);
    std::pair<int, Individual*> crossover(
    std::pair<int, Individual*> &mother,
    std::pair<int, Individual*> &father);
    void shuffle_vertex(std::vector<int> &vertex);
    std::vector<int> getVertex();
    void setGraph(Graph &graph); 
    std::pair<int, Individual*> selectParent(
    std::vector<std::pair<int, Individual*>> &populationEvaluated);

    long long int calculateInsertionCost(const std::vector<int>& tour, int pos, int vertex);

    std::vector<int> run(Graph& graph) override;
    HGA(int populationSize, int itMax);
    ~HGA();
    
    // inicialização população
    
     /*!
     * This function implements the biased fitness, to evaluate the individuals of the original population.
     * 
     * @note There is no need for a comparison function to be passed as argument.
     *
     * @param first Pointer/iterator to the beginning of the range we wish to sort.
     * @param last Pointer/iterator to the location just past the last valid value of the range we wish to sort.
     * @tparam FwrdIt A forward iterator to the range we need to sort.
     * @tparam Comparator A Comparator type function tha returns true if first argument is less than the second argument.
     */
    int biasedFitness (Individual individual); 


     /*!
     * Essa função implementa a classificação do indivíduo P_i na população P nos termos do seu valor objetivo
     Calcular o valor objetivo de todos os individuos e retornar qual posição do individuo
     criar um set de pair <numero_individuo, posicao_
            solution.push_back(t);
        }
        individual.tour = solution;rank>, ordenado pela posicao rank 
     * 
     * @note cost(σPi).
     *
     * @param first Pointer/iterator to the beginning of the range we wish to sort.
     * @param last Pointer/iterator to the location just past the last valid value of the range we wish to sort.
     * @tparam FwrdIt A forward iterator to the range we need to sort.
     * @tparam Comparator A Comparator type function tha returns true if first argument is less than the second argument.
     */
    int fc(Individual individual);

    /*!
     * Essa função implementa a classificação do indivíduo P_i na população P nos termos do seu valor de contribuição da diversidade
     * 
     * @note dc(σPi ).
     *
    a * @param first Pointer/iterator to the beginning of the range we wish to sort.
     * @param last Pointer/iterator to the location just past the last valid value of the range we wish to sort.
     * @tparam FwrdIt A forward iterator to the range we need to sort.
     * @tparam Comparator A Comparator type function tha returns true if first argument is less than the second argument.
     */
    double fd(Individual individual);


    // contribuição de diversidade
    double dc (Individual pj); 
    void individualDiversityRank ();

    std::vector<std::pair<int,int>> generateAllNodesPairs(Individual individual);

    long int numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj(Individual Pi, Individual Pj);

    long long int cost (std::vector<int> tour); 
    void setIndividualCost ( Individual& individual);
    void setIndividualDiversityContribution ( HGA::Individual& individual );

    void setIndividualCostRank(Individual& individual); 

    void individualCostRank(); 

    std::vector<Individual*> generateCopyPopulation (); 

    double normalizeBronkenPairsDistance(Individual Pi, Individual Pj);

    

    Individual createIndividuals(); 
    
    void createPopulation();

    void initializePopulation();
    std::vector<std::pair<int, Individual*>> evaluatePopulation(std::vector<HGA::Individual> &population);

    //fazer o operador de mutacao para diversidade e intensificacao
    //These initial individuals are also diversified and intensified by the mutation operator and LS procedure, respectively.

    // OPERADOR DE MUTACAO PARA DIVERSIDADE R&R (ruin and recreate)
    void ruinAndRecreate(Individual& indi);

    std::vector<int> ruin (Individual& indi);
    void recreate (Individual& indi, std::vector<int> vertexToBeInserted);

    int generateNumberOfVertexToBeRemove(std::vector<int> tour);

    //Heristicas de remocao que serao escolhidas aleatoriamente
    
    //heuristica worst removal
    std::vector<int> worstRemovalHeuristic(Individual indi);

    // heuristica block removal
    std::vector<int> blockRemovalHeuristic(Individual indi);
    HEURISTICS chooseRemovalHeuristic();

    std::vector<int> lsProcedure(std::vector<int> current_solution); 

    std::vector<int> LocalSearch (std::vector<int> current_solution);

    std::vector<int> L (int pos_u, int u, std::vector<int> V);
    int pred ( int pos_u,std::vector<int> tour);

    long long int pi(int pos_u,int u, int v, std::vector<int> tour);

    std::vector<int> best4opt(std::vector<int> solution);

    long long int D2O (int i, int j, std::vector<int> tour);
    long long int delta(int i1, int i2, int j1, int j2);

    bool Cond(int i1, int i2, int j1, int j2); 

    std::vector<int> FourOptNeighborhood(std::vector<int> tour);
    std::vector<int> fourOptMove(int i1, int i2, int j1, int j2, std::vector<int> tour);
    
    //LS procedure para intensificacao

    // seleção de pais
    std::pair<Individual, Individual> tournamentSelection(const std::vector<Individual>& population);
    //crossover
        // ruin-and-recreate mutation
    //local serch
    // administração da população
    // retornar melhor solução 
};
#endif