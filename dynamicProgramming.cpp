#include <bits/stdc++.h>
 
using namespace std;


//IGNORE THE BELOW
//--------------------------- DEBBUGING PURPOSES BEGIN ---------------------------

#define deb(...) logger(#__VA_ARGS__, __VA_ARGS__)
template<typename ...Args>
void logger(string vars, Args&&... values) {
    cout << vars << " = ";
    string delim = "";
    (..., (cout << delim << values, delim = ", "));
    cout<<endl;
}

template<typename Container>
void print(Container beg){
    cout<<"[";
    for(auto k:beg){
        cout<<k<<" ";
    }
    cout<<"]"<<endl;
}

void print(vector<int>v){
	cout<<"[";
	for(int i=0;i<v.size();i++){
		cout<<v[i];
		if(i+1!=v.size())cout<<", ";
	}
	cout<<"]"<<endl;
}

//--------------------------- DEBBUGING PURPOSES END ---------------------------


int n;
const int maxn = 50;
int custo[maxn][maxn][maxn];

int calculaTour(std::vector<int>& ordem){
    int ans = custo[ordem[n-2]][ordem[n-1]][ordem[0]]
            + custo[ordem[n-1]][ordem[0]][ordem[1]];
    for(int i=0;i<n-2;i++){
        ans+=custo[ordem[i]][ordem[i+1]][ordem[i+2]];
    }
    return ans;
}


namespace dynamicProgramming{
    int first,second;
    std::vector<std::vector<std::vector<int>>> dp;
    int solve(int mask,int penultimo,int ultimo){
        if(__builtin_popcount(mask)==n){
            return custo[penultimo][ultimo][first]+custo[ultimo][first][second];
        }
        if(dp[mask][penultimo][ultimo]!=-1) return dp[mask][penultimo][ultimo];
        int tot = INT_MAX;
        
        for(int i = 0;i<n;i++){
            if((mask&(1<<i))==0){
                int teste = custo[penultimo][ultimo][i] + solve(mask^(1<<i),ultimo,i);
                if(tot>teste){
                    tot = teste;
                    
                }
            }
        }
        
        return dp[mask][penultimo][ultimo] = tot;
    
    }

    std::vector<int> brute(){
        int ans = INT_MAX;
        vector<int> tour;
        for(first = 0;first<n;first++){
            for(second=0;second<n;second++){
                if(first!=second){
                    dp = std::vector<std::vector<std::vector<int>>>((1<<n),std::vector<std::vector<int>>(n,std::vector<int>(n,-1)));
                    auto resposta = solve((1<<first)+(1<<second),first,second);
                    if(resposta<ans){
                        ans = resposta;
                    }
                }
            }
        }
        return tour;
    }
};
