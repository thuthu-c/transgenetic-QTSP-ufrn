// #include <iostream>
// #include "../include/benchmark/benchmark.h"
// #include<fstream>
// using namespace std;

// #include "../include/algorithms/tabu_memetic.h"
// #include "../include/algorithms/memetic.h"
// #include "../include/algorithms/hga.h"
// #include "../include/algorithms/trans_qtsp.h"
// #include "../include/algorithms/trans_qtsp_v1.h"
// #include <climits>
// #include <string>
// #include <random>

// //stdout is not working due to irace, we can debug by printing to a file
// void debug(std::string st){
//     std::ofstream myfile;
//     myfile.open ("debFile.txt",std::ios_base::app);
//     myfile << st<<"\n";
//     myfile.close();
// }


// //lembra de mudar o parameterFile do irace_config.xml também
// bool isMemetic = false;//gambiarra por enquanto depois refatoramos melhor

// void runMemetic(int argc, char *argv[]){
//     if(argc == 1){
//         Graph graph(3); // Assuming 5 nodes for this example

//         // Add edges with costs (example edges)
//         graph.addEdge(0, 1, 2, 10);
//         graph.addEdge(1, 2, 0, 20);
//         graph.addEdge(2, 0, 1, 30);
//         graph.addEdge(0, 2, 1, 40);
//         graph.addEdge(1, 0, 0, 50);
//         graph.addEdge(2, 1, 1, 60);
//         // graph.addEdge(2, 0, 3, 70);
//         // graph.addEdge(3, 1, 1, 80);

//         // Genetic algorithm parameters
//         int populationSize = 100;
//         int numGenerations = 100;
//         double mutationRate = 0.1;
//         double crossoverRate = 0.2;

//         // Initialize genetic algorithm
//         TabuMemetic taMeAlgorithm(populationSize, numGenerations, mutationRate, crossoverRate);

//         // Run the genetic algorithm
//         std::vector<int> bestTour = taMeAlgorithm.run(graph);
//         std::cout<<"Tour size: "<<bestTour.size()<<std::endl;

//         // Calculate and print the cost of the best tour
//         TabuMemetic::Individual i{bestTour, 0};
//         int bestCost = taMeAlgorithm.calculateTour(0, 0, i);
//         if(bestCost < INT_MAX)std::cout << "Best tour cost: " << bestCost << std::endl;
//         else std::cout<<"There is no valid tour for this graph :("<<std::endl;

//         // Print the best tour
//         std::cout << "Best tour: ";
//         for (int city : bestTour) {
//             std::cout << city << " ";
//         }
//         std::cout << std::endl;

//         return;
//     }
//     if (argc < 5 || argc > 7)
//     {
//         std::cout << "The CLI needs the following parameters: " << std::endl;
//         std::cout << "1. Max evaluations" << std::endl;
//         std::cout << "2. Population size" << std::endl;
//         std::cout << "3. Crossover rate" << std::endl;
//         std::cout << "4. Mutation rate" << std::endl;
//         std::cout << "5. Graph instance (optional)\n" << std::endl;

//         std::cout << "Arguments passed (" << argc << ") were: " << std::endl;

//         for(int i = 0; i < argc; i++) {
//             std::cout << argv[i] << " ";
//         }

//         std::cout << std::endl;

//         return ;
//     }
    
//     int maxEvaluations = std::atoi(argv[1]);
//     int populationSize = std::atoi(argv[2]);
//     float crossoverRate = std::atof(argv[3]);
//     float mutationRate = std::atof(argv[4]);

//     if(argc == 5) {
//         // std::cout << "Running full benchmark! This may take a while..." << std::endl;
//         // std::cout << std::endl;
//         // std::cout << "Max evaluations: " << maxEvaluations << std::endl;
//         // std::cout << "Population size: " << populationSize << std::endl;
//         // std::cout << "Crossover rate: " << crossoverRate << std::endl;
//         // std::cout << "Mutation rate: " << mutationRate << std::endl;

//         Benchmark benchmark(
//             maxEvaluations,
//             populationSize,
//             crossoverRate,
//             mutationRate
//         );

//         benchmark.evaluate();
//     } else {
//         std::string instance = argv[5];
//         std::string algorithm = argv[6];
               
//         // std::cout << "Running irace over the Genetic Improved algorithm\n"
//         //           << "and instance " << instance << std::endl;
//         // std::cout << std::endl;
//         // std::cout << "Max evaluations: " << maxEvaluations << std::endl;
//         // std::cout << "Population size: " << populationSize << std::endl;
//         // std::cout << "Crossover rate: " << crossoverRate << std::endl;
//         // std::cout << "Mutation rate: " << mutationRate << std::endl;
        
//         Benchmark benchmark(
//             maxEvaluations,
//             populationSize,
//             crossoverRate,
//             mutationRate
//         );

//         benchmark.evaluate(instance, algorithm);
//     }
// }

// void runTabu(int argc, char *argv[]){
//     if(argc==5){
//         int tabuTime = std::atoi(argv[1]);
//         int tabuAspirationTime = std::atoi(argv[2]);
//         int tabuMaxIter = std::atoi(argv[3]);
//         std::string instance = argv[4];
//         Benchmark benchmark(
//             tabuTime,
//             tabuAspirationTime,
//             tabuMaxIter
//         );

//         benchmark.evaluate();
//         return ;
//     }
// }

// void runHga(int argc, char *argv[]){
//     std::cout<<"o número de argumentos são: " << argc << std::endl;

//      int maxEvaluations = std::atoi(argv[1]);
//     int populationSize = std::atoi(argv[2]);
//     float crossoverRate = std::atof(argv[3]);
//     float mutationRate = std::atof(argv[4]);
//      if(argc == 5) {

//         int maxEvaluations = std::atoi(argv[1]);
//         int populationSize = std::atoi(argv[2]);
//         float crossoverRate = std::atof(argv[3]);
//         float mutationRate = std::atof(argv[4]);

//         // std::string instance = argv[5];
//         // std::string algorithm = argv[6];
//             /*        
//             std::cout << "Running irace over the Genetic Improved algorithm\n"
//                     << "and instance " << instance << std::endl;
//             std::cout << std::endl;
//             std::cout << "Max evaluations: " << maxEvaluations << std::endl;
//             std::cout << "Population size: " << populationSize << std::endl;
//             std::cout << "Crossover rate: " << crossoverRate << std::endl;
//             std::cout << "Mutation rate: " << mutationRate << std::endl;
//             */
//             Benchmark benchmark(
//                 maxEvaluations,
//                 populationSize,
//                 crossoverRate,
//                 mutationRate
//             );

//             benchmark.evaluate();
//     } else {
//         std::string instance = argv[5];
//         std::string algorithm = argv[6];
               
//         // std::cout << "Running irace over the Genetic Improved algorithm\n"
//         //           << "and instance " << instance << std::endl;
//         // std::cout << std::endl;
//         // std::cout << "Max evaluations: " << maxEvaluations << std::endl;
//         // std::cout << "Population size: " << populationSize << std::endl;
//         // std::cout << "Crossover rate: " << crossoverRate << std::endl;
//         // std::cout << "Mutation rate: " << mutationRate << std::endl;
        
//         Benchmark benchmark(
//             maxEvaluations,
//             populationSize,
//             crossoverRate,
//             mutationRate
//         );

//         benchmark.evaluate(instance, algorithm);
//     }
// }

// // void runTransQTSP(int argc, char *argv[]){

// //      int maxEvaluations = std::atoi(argv[1]);
// //     int populationSize = std::atoi(argv[2]);
// //     double plasmidSize = std::atof(argv[3]);
// //      if(argc == 4) {

// //         int maxEvaluations = std::atoi(argv[1]);
// //         int populationSize = std::atoi(argv[2]);
// //         double plasmidSize = std::atof(argv[3]);

// //         Benchmark benchmark(
// //             maxEvaluations,
// //             populationSize,
// //             (double) plasmidSize
// //         );

// //         benchmark.evaluate();
// //     } else if(argc == 5) {

// //         int maxEvaluations = std::atoi(argv[1]);
// //         int populationSize = std::atoi(argv[2]);
// //         double plasmidSize = std::atof(argv[3]);

// //         // std::string instance = argv[5];
// //         // std::string algorithm = argv[6];
// //             /*        
// //             std::cout << "Running irace over the Genetic Improved algorithm\n"
// //                     << "and instance " << instance << std::endl;
// //             std::cout << std::endl;
// //             std::cout << "Max evaluations: " << maxEvaluations << std::endl;
// //             std::cout << "Population size: " << populationSize << std::endl;
// //             std::cout << "Crossover rate: " << crossoverRate << std::endl;
// //             std::cout << "Mutation rate: " << mutationRate << std::endl;
// //             */
// //             Benchmark benchmark(
// //                 maxEvaluations,
// //                 populationSize,
// //                 (double) plasmidSize
// //             );

// //             benchmark.evaluate();
// //     } else {
// //         std::string instance = argv[5];
// //         std::string algorithm = argv[6];
               
// //         // std::cout << "Running irace over the Genetic Improved algorithm\n"
// //         //           << "and instance " << instance << std::endl;
// //         // std::cout << std::endl;
// //         // std::cout << "Max evaluations: " << maxEvaluations << std::endl;
// //         // std::cout << "Population size: " << populationSize << std::endl;
// //         // std::cout << "Crossover rate: " << crossoverRate << std::endl;
// //         // std::cout << "Mutation rate: " << mutationRate << std::endl;
        
// //         Benchmark benchmark(
// //             maxEvaluations,
// //             populationSize,
// //             (double) plasmidSize
// //         );

// //         benchmark.evaluate(instance, algorithm);
// //     }
// // }

// // void runTransQTSPProbT(int argc, char *argv[]) {
// //     // 1. Verificação Mínima de Segurança
// //     if(argc < 5) {
// //         std::cout << "Erro! Faltam argumentos.\n";
// //         std::cout << "Uso correto: ./bin/exec <maxEvals> <popSize> <probT> <stepProb> [instancia] [algoritmo]\n";
// //         return; // Sai da função sem quebrar o C++
// //     }

// //     // 2. Leitura Segura (os 4 primeiros valores garantidamente existem)
// //     int maxEvaluations = std::atoi(argv[1]);
// //     int populationSize = std::atoi(argv[2]);
// //     double probT = std::atof(argv[3]);     // atof para ler decimais!
// //     double stepProb = std::atof(argv[4]);  // atof para ler decimais!
// //     double plasmidSize = std::atof(argv[5]);

// //     // 3. Roda o Benchmark completo (Todos os grafos)
// //     if(argc == 6) {
// //         std::cout << "=> Iniciando Benchmark completo do TransQTSPProbT..." << std::endl;
// //         Benchmark benchmark(maxEvaluations, populationSize, (double) probT, (double) stepProb);
// //         benchmark.evaluate();
// //     } 
// //     // 4. Roda apenas em uma Instância específica
// //     else if (argc >= 7) {
// //         std::string instance = argv[6];
// //         std::string algorithm = argv[7];
        
// //         std::cout << "=> Executando TransQTSPProbT na instancia: " << instance << std::endl;
// //         Benchmark benchmark(maxEvaluations, populationSize, probT, stepProb, plasmidSize);
// //         benchmark.evaluate(instance, algorithm);
// //     }
// //     // Caso estranho (ex: passou a instância mas esqueceu o nome do algoritmo)
// //     else {
// //         std::cout << "Erro! Se passar a instância, informe também o algoritmo.\n";
// //     }
// // }

// // void runTransQTSPV2(int argc, char *argv[]) {
// //     // 1. Verificação Mínima: Agora o executável + 5 parâmetros = 6 no mínimo!
// //     if(argc < 6) {
// //         std::cout << "Erro! Faltam argumentos.\n";
// //         std::cout << "Uso correto: ./bin/exec <maxEvals> <popSize> <probT> <stepProb> <plasmidSize> [instancia] [algoritmo]\n";
// //         return; 
// //     }

// //     // 2. Leitura Segura
// //     int maxEvaluations = std::atoi(argv[1]);
// //     int populationSize = std::atoi(argv[2]);
// //     double probT = std::atof(argv[3]);     
// //     double stepProb = std::atof(argv[4]);  
// //     double plasmidSize = std::atof(argv[5]); // Lemos o novo parâmetro!

// //     // 3. Roda o Benchmark completo (Passou apenas os 5 números)
// //     if(argc == 6) {
// //         std::cout << "=> Iniciando Benchmark completo do TransQTSPV2..." << std::endl;
        
// //         // CORREÇÃO: Passando o plasmidSize para não gerar lixo de memória!
// //         Benchmark benchmark(maxEvaluations, populationSize, probT, stepProb, plasmidSize);
// //         benchmark.evaluate();
// //     } 
// //     // 4. Roda apenas em uma Instância específica (Passou os números + Instância + Algoritmo = 8 itens)
// //     else if (argc >= 8) {
// //         // CORREÇÃO: Com o plasmidSize novo, os nomes "pularam" uma casa!
// //         std::string instance = argv[6];   
// //         std::string algorithm = argv[7];  
        
// //         std::cout << "=> Executando TransQTSPV2 na instancia: " << instance << std::endl;
// //         Benchmark benchmark(maxEvaluations, populationSize, probT, stepProb, plasmidSize);
// //         benchmark.evaluate(instance, algorithm);
// //     }
// //     else {
// //         std::cout << "Erro! A quantidade de parâmetros passados (" << argc << ") está incorreta.\n";
// //     }
// // }


// // void runTransQTSPProbT(int argc, char *argv[]) {
// //     // Valores padrão de segurança
// //     int maxEvaluations = 50000;
// //     int populationSize = 100;
// //     double probT = 0.8;
// //     double stepProb = 0.05;
// //     double plasmidSize = 0.3;
// //     std::string instance = "";
// //     std::string algorithm = "transQTSPProbT";

// //     // Leitura das flags (ignora a ordem em que o iRace as envia)
// //     for (int i = 1; i < argc; ++i) {
// //         std::string arg = argv[i];
// //         if (arg == "--maxEvals" && i + 1 < argc) {
// //             maxEvaluations = std::stoi(argv[++i]);
// //         } else if (arg == "--popSize" && i + 1 < argc) {
// //             populationSize = std::stoi(argv[++i]);
// //         } else if (arg == "--probT" && i + 1 < argc) {
// //             probT = std::stod(argv[++i]);
// //         } else if (arg == "--stepProb" && i + 1 < argc) {
// //             stepProb = std::stod(argv[++i]);
// //         } else if (arg == "--plasmidSize" && i + 1 < argc) {
// //             plasmidSize = std::stod(argv[++i]);
// //         } else if (arg == "--instancia" && i + 1 < argc) {
// //             instance = argv[++i];
// //         }
// //     }

// //     // NENHUM std::cout AQUI! O iRace quebra se você imprimir texto.
// //     Benchmark benchmark(maxEvaluations, populationSize, probT, stepProb, plasmidSize);
    
// //     if (instance.empty()) {
// //         benchmark.evaluate();
// //     } else {
// //         benchmark.evaluate(instance, algorithm);
// //     }
// // }

// // --- VERSÃO 1: TransQTSP Original ---
// void runTransQTSP(int argc, char *argv[]) {
//     if(argc < 6) return; // Retorno silencioso se faltarem argumentos base

//     int maxEvaluations = std::atoi(argv[1]);
//     int populationSize = std::atoi(argv[2]);
//     // Pula probT e stepProb, e pega o plasmidSize do argv[5]
//     double plasmidSize = std::atof(argv[5]); 

//     if (argc == 6) {
//         std::cout << "\n======================================================\n";
//         std::cout << "=> Iniciando Benchmark completo do TransQTSP Original..." << std::endl;
//         Benchmark benchmark(maxEvaluations, populationSize, plasmidSize);
//         benchmark.evaluate();
//     } else if (argc >= 8) {
//         std::string instance = argv[6];
//         std::string algorithm = argv[7];
        
//         std::cout << "\n======================================================\n";
//         std::cout << "=> Executando TransQTSP Original na instancia: " << instance << std::endl;
//         Benchmark benchmark(maxEvaluations, populationSize, plasmidSize);
//         benchmark.evaluate(instance, algorithm);
//     }
// }

// // --- VERSÃO 2: TransQTSP ProbT ---
// void runTransQTSPProbT(int argc, char *argv[]) {
//     if(argc < 6) return; 

//     int maxEvaluations = std::atoi(argv[1]);
//     int populationSize = std::atoi(argv[2]);
//     double probT = std::atof(argv[3]);     
//     double stepProb = std::atof(argv[4]);  
//     double plasmidSize = std::atof(argv[5]);

//     if(argc == 6) {
//         std::cout << "\n======================================================\n";
//         std::cout << "=> Iniciando Benchmark completo do TransQTSPProbT..." << std::endl;
//         Benchmark benchmark(maxEvaluations, populationSize, probT, stepProb);
//         benchmark.evaluate();
//     } 
//     else if (argc >= 8) {
//         std::string instance = argv[6];
//         std::string algorithm = argv[7];
        
//         std::cout << "\n======================================================\n";
//         std::cout << "=> Executando TransQTSPProbT na instancia: " << instance << std::endl;
//         Benchmark benchmark(maxEvaluations, populationSize, probT, stepProb, plasmidSize);
//         benchmark.evaluate(instance, algorithm);
//     }
// }

// // --- VERSÃO 3: TransQTSP V2 ---


// int main(int argc, char *argv[])
// {
//     // if(isMemetic) runMemetic(argc,argv);
//     // else 
//     // runTabu(argc,argv);
//     runMemetic(argc, argv);
//     runHga(argc, argv);
//     runTransQTSP(argc, argv);
//     runTransQTSPProbT(argc, argv);
//     runTransQTSPV2(argc, argv);
    
//     return 0;
// }

#include <iostream>
#include "../include/benchmark/benchmark.h"
#include <string>
#include <cstdlib>

void runAllAlgorithms(int argc, char *argv[]) {
    // 1. Verifica se temos todos os 7 parâmetros numéricos
    if (argc < 8) {
        std::cout << "Uso correto: ./bin/exec <maxEvals> <popSize> <crossRate> <mutRate> <probT> <stepProb> <plasmidSize> [instancia] [algoritmo]\n";
        return;
    }

    int maxEvaluations = std::atoi(argv[1]);
    int populationSize = std::atoi(argv[2]);
    float crossoverRate = std::atof(argv[3]);
    float mutationRate = std::atof(argv[4]);
    double probT = std::atof(argv[5]);
    double stepProb = std::atof(argv[6]);
    double plasmidSize = std::atof(argv[7]);

    // 2. Cria o Benchmark com TUDO inicializado perfeitamente!
    Benchmark benchmark(maxEvaluations, populationSize, crossoverRate, mutationRate, probT, stepProb, plasmidSize);

    // 3. Roda o benchmark completo (Gera grafos e salva no CSV)
    if (argc == 8) {
        std::cout << "=> Iniciando Benchmark Completo para TODOS os algoritmos..." << std::endl;
        benchmark.evaluate();
    } 
    // 4. Roda apenas uma instancia especifica (Para o iRace)
    else if (argc >= 10) {
        std::string instance = argv[8];
        std::string algorithm = argv[9];
        benchmark.evaluate(instance, algorithm);
    }
}

void runTransQTSPV2(int argc, char *argv[]) {
    if(argc < 6) {
        std::cout << "Erro! Faltam argumentos.\n";
        std::cout << "Uso correto: ./bin/exec <maxEvals> <popSize> <probT> <stepProb> <plasmidSize> [instancia] [algoritmo]\n";
        return; 
    }

    int maxEvaluations = std::atoi(argv[1]);
    int populationSize = std::atoi(argv[2]);
    double probT = std::atof(argv[3]);     
    double stepProb = std::atof(argv[4]);  
    double plasmidSize = std::atof(argv[5]); 

    if(argc == 6) {
        std::cout << "\n======================================================\n";
        std::cout << "=> Iniciando Benchmark completo do TransQTSPV2..." << std::endl;
        Benchmark benchmark(maxEvaluations, populationSize, probT, stepProb, plasmidSize);
        benchmark.evaluate();
    } 
    else if (argc >= 8) {
        std::string instance = argv[6];   
        std::string algorithm = argv[7];  
        
        std::cout << "\n======================================================\n";
        std::cout << "=> Executando TransQTSPV2 na instancia: " << instance << std::endl;
        Benchmark benchmark(maxEvaluations, populationSize, probT, stepProb, plasmidSize);
        benchmark.evaluate(instance, algorithm);
    }
}

void runRemTransp(int argc, char *argv[]) {
    if(argc < 6) {
        std::cout << "Erro! Faltam argumentos.\n";
        std::cout << "Uso correto: ./bin/exec <maxEvals> <popSize> <probT> <stepProb> <plasmidSize> [instancia] [algoritmo]\n";
        return; 
    }

    int maxEvaluations = std::atoi(argv[1]);
    int populationSize = std::atoi(argv[2]);
    double probT = std::atof(argv[3]);     
    double stepProb = std::atof(argv[4]);  
    double plasmidSize = std::atof(argv[5]); 

    if(argc == 6) {
        std::cout << "\n======================================================\n";
        std::cout << "=> Iniciando Benchmark completo do RemTransp..." << std::endl;
        Benchmark benchmark(maxEvaluations, populationSize, probT, stepProb, plasmidSize);
        benchmark.evaluate();
    } 
    else if (argc >= 8) {
        std::string instance = argv[6];   
        std::string algorithm = argv[7];  
        
        std::cout << "\n======================================================\n";
        std::cout << "=> Executando RemTransp na instancia: " << instance << std::endl;
        Benchmark benchmark(maxEvaluations, populationSize, probT, stepProb, plasmidSize);
        benchmark.evaluate(instance, algorithm);
    }
}

int main(int argc, char *argv[])
{
    // runAllAlgorithms(argc, argv);
    // runTransQTSPV2(argc, argv);
    runRemTransp(argc, argv);
    return 0;
}

