#include "../../include/algorithms/greedy_approach.h"
#include <bits/stdc++.h>

int GreedyApproach::calculaTour(std::vector<int> &ordem, Graph &graph)
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

std::pair<int, int> GreedyApproach::getmin(int penultimo, int ultimo, Graph &graph)
{
    std::pair<int, int> menor = std::make_pair(INT_MAX, -1);
    for (int i = 0; i < graph.getMaxM(); i++)
    {
        bool visited = !(this->visto.find(i) != this->visto.end());

        if (visited)
        {
            menor = std::min(menor, std::make_pair(graph.custo[penultimo][ultimo][i], i));
        }
    }

    return menor;
}

std::pair<int,std::vector<int>> GreedyApproach::run(int prim, int sec, Graph &graph)
{
    visto = std::set<int>();

    int n = graph.getMaxM();
    int ultimo = sec;
    int penultimo = prim;

    this->visto.insert(prim);
    this->visto.insert(sec);

    std::vector<int> tour = {prim, sec};
    for (int i = 0; i < (n - 2); i++)
    {
        std::pair<int, int> menor = getmin(penultimo, ultimo, graph);
        penultimo = ultimo;
        ultimo = menor.second;
        this->visto.insert(ultimo);
        tour.push_back(ultimo);
    }

    return std::make_pair(this->calculaTour(tour, graph),tour);
}

std::vector<int> GreedyApproach::run(Graph graph)
{
    int n = graph.getMaxM();
    int menor = INT_MAX;
 
    std::vector<int> tour;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {   
                auto teste = run(i,j,graph);
                if(teste.first<menor){
                    menor = teste.first;
                    tour = teste.second;
                }
                
            }
        }
    }

    return tour;
}
