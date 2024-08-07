#include "../../include/algorithms/tabu.h"

#include<iostream>
#include<vector>
#include <algorithm>    // std::random_shuffle


using namespace std;


int n;

const int maxn = 220;

Tabu::Tabu() {}

Tabu::~Tabu() {}

Graph graph;
//calcula o custo final de uma solução
int calculaTour(std::vector<int> &ordem)
{
    
    auto custo = graph.custo;
    int n = graph.getMaxM();

    int ans = custo[ordem[n - 2]][ordem[n - 1]][ordem[0]] + custo[ordem[n - 1]][ordem[0]][ordem[1]];

    for (int i = 0; i < n - 2; i++)
    {
        ans += custo[ordem[i]][ordem[i + 1]][ordem[i + 2]];
    }

    return ans;
}
const int INF = 1e9;
//CONSTANTES 
bool aspiracao = true;//vamos usar critério de aspiração?
int maxTime;//tempo máximo que um movimento fica na lista tabu;
int maxProibition;
int iteracoes;//quantas iterações vamos rodar
bool randomizedInitalSolution = true;//a solução inicial vai ser randomizada?

//VARIAVEIS DE CONTROLE GLOBAIS
int bestSolution = INF;//melhor valor até agora, inicia com infinito
vector<int> whoIsSol;
int tempoAtual = 0;




/*
    Estrutura para armazenar movimentos;
    armazena os dois indices e o tempo em que aquele movimento foi colocado
*/
int lastTimeTabu[maxn][maxn];//matriz armazenando todos os movimentos proibidos
std::vector<int> solucaoInicial;


bool canAspire(int i,int j){
    return aspiracao && lastTimeTabu[i][j]+maxProibition<tempoAtual;
}

//função que checa se swap de i com j está na lista tabu;
bool inListaTabu(int i,int j){
    return lastTimeTabu[i][j]+maxTime>tempoAtual;
}

void addTabu(int a,int b){
    lastTimeTabu[a][b] = lastTimeTabu[b][a] = tempoAtual;
}


//pega melhor neighbor que não está presente na lista tabu (leva em consideração critérios de aspiração)
std::pair<int,int> getBestNeighbor(){
    std::pair<int,int> bestNeighbor = {-1,-1};
    int bestValue = INF;
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            if(inListaTabu(i,j)){
                if(canAspire(i,j)){
                    
                    std::swap(solucaoInicial[i],solucaoInicial[j]);
                    
                    int tour = calculaTour(solucaoInicial);
                    if(tour<bestSolution && tour<bestValue){//aspiracao so funciona se melhorar a solucao final;
                        bestValue = tour;
                        bestNeighbor = {i,j};
                    }

                    std::swap(solucaoInicial[i],solucaoInicial[j]);
                
                }   
                else continue;
            }
            else{
                std::swap(solucaoInicial[i],solucaoInicial[j]);

                int tour = calculaTour(solucaoInicial);
                if(tour<bestValue){
                    bestValue = tour;
                    bestSolution = std::min(bestSolution,bestValue);
                    bestNeighbor = {i,j};
                }

                std::swap(solucaoInicial[i],solucaoInicial[j]);
            }
        }
    }
    return bestNeighbor;
    
}

void print(std::vector<int> v){
    for(auto k:v)cout<<k<<" ";
    cout<<endl;
}

//uma iteração da busca tabu;
void tabu(){
    
    
    std::pair<int,int> melhor = getBestNeighbor();

    std::swap(solucaoInicial[melhor.first],solucaoInicial[melhor.second]);
    if(calculaTour(solucaoInicial)<calculaTour(whoIsSol))whoIsSol = solucaoInicial;
    addTabu(melhor.first,melhor.second);

}

//roda a função de busca tabu várias vezes
void tabuMetaHeuristica(){
    bestSolution = INF;
    for(int i = 0;i<maxn;i++)for(int j = 0;j<maxn;j++) lastTimeTabu[i][j] = -INF;
    for(int _ = 0;_<iteracoes;_++){
        tabu();
        tempoAtual++;
    }
}

std::vector<int> Tabu::run(Graph& graphInput){
    solucaoInicial.clear();
    graph = graphInput;
    n = graph.getMaxM();
    maxTime = this->tempoMaximo;
    maxProibition = this->aspiracaoMaximo;
    iteracoes = this->numIter;
    for(int i=0;i<n;i++){
        solucaoInicial.push_back(i);
    }

    if(randomizedInitalSolution) std::random_shuffle(solucaoInicial.begin(),solucaoInicial.end());
    whoIsSol = solucaoInicial;
    tabuMetaHeuristica();
    return whoIsSol;
}