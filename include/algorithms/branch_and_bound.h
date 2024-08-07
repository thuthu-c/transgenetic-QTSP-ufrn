#ifndef _BRANCHNBOUND_H_
#define _BRANCHNBOUND_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"
#include "./cheapest_insertion.h"

class solution{
    public:
    virtual ~solution() = default;

    int myUpperBound = 0;
    int myLowerBound = 0;
    std::set<int> falta;//TODO: trocar por histograma;
    std::vector<int> ordem;
    solution(){
        myUpperBound = 0;
    }
    solution(int n){
        for(int i=0;i<n;i++)falta.insert(i);
    }
    void adiciona(int u){
        ordem.push_back(u);
        falta.erase(u);
    }
};

struct customComparator{
    bool operator()(const solution& a,const solution& b) const{
        return a.myUpperBound<b.myUpperBound;
    }
};

class BranchAndBound : public TspSolver
{
private:
    solution runIJ(Graph& graph,int prim,int sec);
    

public:
    long long pruned=0;
    int explored = 0;
    BranchAndBound(){

    }
    ~BranchAndBound(){

    }
    std::vector<int> run(Graph& graph);

};

#endif