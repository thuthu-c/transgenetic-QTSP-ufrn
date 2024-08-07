#include "../../include/algorithms/branch_and_bound.h"
#include "../../include/algorithms/cheapest_insertion.h"
#include "../../src/data_structures/crazyHeap.cpp"

#include<bits/stdc++.h>
using namespace std;

const int inf = 1e8;
int calculaTour(std::vector<int> &ordem, Graph &graph)
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

int getLowerBound(Graph& graph,const solution& solucao){

    std::vector<int> teste;
    int n=solucao.falta.size();
    int total = 0;

    for(int i = 1;i<solucao.ordem.size()-1;i++){//pegamos todas as arestas que já estão
        total+=graph.custo[solucao.ordem[i-1]][solucao.ordem[i]][solucao.ordem[i+1]];
    }
    if(n>=2){//lower bound
        int primeiro = solucao.ordem[0];
        int segundo = solucao.ordem[1];

        int penultimo = solucao.ordem[solucao.ordem.size()-2];
        int ultimo = solucao.ordem[solucao.ordem.size()-1];

        int minPS = inf;
        int minP = inf;
        
        int minPU = inf;
        int minU = inf;

        for(auto x:solucao.falta){
            minPS = std::min(minPS, graph.custo[x][primeiro][segundo]);
            minPU = std::min(minPS, graph.custo[penultimo][ultimo][x]);

            for(auto y:solucao.falta){
                if(x!=y){
                    minP = std::min(minP, graph.custo[x][y][primeiro]);
                    minU = std::min(minU, graph.custo[ultimo][x][y]);
                }
            }
        }
        total += minPS + minPU + minP + minU;

        for(auto x:solucao.falta){
            for(auto y:solucao.falta){
                for(auto z:solucao.falta){
                    if(x!=y && y!=z && x!=z){
                        teste.push_back(graph.custo[x][y][z]);
                    }
                }
                
            }
        }
        std::sort(teste.begin(),teste.end());
        
        for(int i=0;i<n-2;i++){
            total+=teste[i];
        }
    }
    return total;
}

std::pair<int, int> getmin(set<int>& visto,int penultimo, int ultimo, Graph &graph)
{
    std::pair<int, int> menor = std::make_pair(1e9, -1);
    for (int i = 0; i < graph.getMaxM(); i++)
    {
        if (visto.find(i) == visto.end())
        {
            menor = std::min(menor, std::make_pair(graph.custo[penultimo][ultimo][i], i));
        }
    }
    assert(menor.second!=-1);
    return menor;
}


//propriedade: se for completo tem q retornar o calculaTour 
vector<int> getOrdem(Graph&graph,const solution& solucao){
    vector<int> tour = solucao.ordem;
    assert(tour.size()>=2);
    int penultimo = tour[tour.size()-2];
    int ultimo = tour[tour.size()-1];
    set<int> visto;
    for(auto k:tour)visto.insert(k);
    while(tour.size()<graph.getMaxM())
    {
        std::pair<int, int> menor = getmin(visto,penultimo, ultimo, graph);
        penultimo = ultimo;
        ultimo = menor.second;
        visto.insert(ultimo);
        tour.push_back(ultimo);
    }
    return tour;

}
//TODO: melhor upperbound
int getUpperBound(Graph&graph,const solution& solucao){
    auto ordem = getOrdem(graph,solucao);
    return calculaTour(ordem,graph);

}




solution bestSol;
std::vector<long long> fac;
solution BranchAndBound::runIJ(Graph& graph,int prim,int sec){
    
    crazyHeap<solution> fila;

    auto MaybeBotaNafila = [&](const solution &solut){
        int maybeUpper = solut.myUpperBound;

        if(bestSol.myUpperBound>maybeUpper){
            bestSol.ordem = getOrdem(graph,solut);
            bestSol.falta = set<int>();
            bestSol.myUpperBound = maybeUpper;
            bestSol.myLowerBound = maybeUpper;
            fila.push(solut);
        }
        else if(solut.myLowerBound<bestSol.myUpperBound)
                fila.push(solut);
    };

    solution solucao(graph.getMaxM());
    solucao.adiciona(prim);
    solucao.adiciona(sec);
    solucao.myLowerBound = getLowerBound(graph,solucao);
    solucao.myUpperBound = getUpperBound(graph,solucao);
    //priority_queue<solution,vector<solution>,cust&&omComparator> fila;
    
    MaybeBotaNafila(solucao);
    //cout<<getLowerBound(graph,solucao)<<" :: "<<bestSol.myUpperBound<<endl;
    //cout<<prim<<" :: "<<sec<<endl;
    int cnt = 1; 
    //set<vector<int>> javis;

    
    while(true){
        fila.relax(bestSol.myUpperBound);
        if(fila.empty())break;

        auto topo = fila.top();
        fila.pop();
        
        this->explored++;
        
        if(cnt%10000==0)cout<<cnt<<endl;
        cnt++;
        
        auto teste = topo.falta;
        if(topo.falta.empty()){//solucao finalizada
            int valor = calculaTour(topo.ordem,graph);
            if(valor<bestSol.myUpperBound){
                bestSol = topo;
            }

            continue;
        }
        if(topo.myLowerBound>=bestSol.myUpperBound){
            
            continue;
        }

        for(auto x:topo.falta){
            solution novaSol = topo;
            novaSol.adiciona(x);
            //if(javis.find(novaSol.ordem)!=javis.end())continue;
            //javis.insert(novaSol.ordem);
            novaSol.myLowerBound = getLowerBound(graph,novaSol);
            novaSol.myUpperBound = getUpperBound(graph,novaSol);
            
            MaybeBotaNafila(novaSol);
        }

        
        
    }

    return bestSol;
}

std::vector<int> BranchAndBound::run(Graph& graph){
    int ans=(1<<30);
    solution solucao;
    this->pruned=0;
    this->explored=0;
    CheapestInsertion cheapest;
    
    bestSol.ordem = cheapest.run(graph);
    bestSol.falta = set<int>();
    bestSol.myUpperBound = calculaTour(bestSol.ordem,graph);//upper bound;
    bestSol.myLowerBound = bestSol.myUpperBound;

    fac = {1,1};
    for(int i=2;i<=20;i++){
        fac.push_back(fac.back()*i);
    }
    for(int i=0;i<graph.getMaxM();i++){
        for(int j=0;j<graph.getMaxM();j++){//i+1
            if(i!=j){
                auto teste = BranchAndBound::runIJ(graph,i,j);
                if(bestSol.myUpperBound<ans){
                    ans = bestSol.myUpperBound;
                    solucao = bestSol;
                }
                //goto A;
            }
        }
    }
    cout<<"aqui: "<<bestSol.myUpperBound<<'\n';
    //A:;
    std::cout<<"Prunei "<<fac[graph.getMaxM()]-this->explored<<" Explorei:"<<this->explored<<std::endl;
    return bestSol.ordem;
}
