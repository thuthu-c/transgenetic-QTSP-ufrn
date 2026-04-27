#include <vector>

struct Plasmid {
    std::vector<int> genes; // uma sequencia de cidades, gerada a partir de algum cromossomo do GIR
    double fitness_gain;    // quanto do custo que se reduz com a transcrição do plasmídeo
};

void transposonOperator(std::vector<int>& individual, const Plasmid& p){
    //a ideia é que a busca local seja otimizada com o 4-opt ()
    
    // 1. Identificar onde as cidades do plasmídeo já estão no indivíduo
    // 2. Remover as cidades originais (para evitar duplicatas)
    // 3. Testar todas as posições de inserção para o bloco p.path_fragment
    // 4. Calcular o custo quadrático c(i, j, k) nas bordas da inserção
    // 5. Fixar na posição de menor custo
}