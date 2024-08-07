#include "../libs/doctest.h"
#include <iostream>
#include <fstream>
#include "../include/data_structures/graph.h"
#include "../include/helpers/graphio.h"

TEST_CASE("It can write graph on disk")
{
    Graph graph(5);
    graph.addEdge(0, 1, 3, 11);
    graph.addEdge(3, 0, 1, 28);
    graph.addEdge(3, 0, 2, 21);
    graph.addEdge(0, 1, 2, 38);
    graph.addEdge(0, 2, 1, 31);
    graph.addEdge(3, 1, 2, 33);
    graph.addEdge(2, 3, 1, 23);
    graph.addEdge(3, 4, 2, 22);
    graph.addEdge(3, 2, 4, 44);
    graph.addEdge(3, 2, 1, 53);
    graph.addEdge(1, 3, 2, 26);
    graph.addEdge(1, 2, 4, 51);
    graph.addEdge(1, 3, 4, 3);
    graph.addEdge(4, 2, 0, 22);

    GraphIO graphio(graph);
    std::string testFilename = "it_can_write_graph_on_disk.txt";

    graphio.write(testFilename);
    std::ifstream infile(testFilename);

    CHECK(infile.good());
}

TEST_CASE("It can read graph from disk")
{
    std::string testFilename = "it_can_read_graph_from_disk.txt";

    // writing file
    std::ofstream file;
    file.open(testFilename);
    file << 6 << "\n";
    file << 1 << " " << 2 << " " << 4 << " " << 11 << "\n";
    file << 4 << " " << 1 << " " << 2 << " " << 28 << "\n";
    file << 4 << " " << 1 << " " << 3 << " " << 21 << "\n";
    file << 1 << " " << 2 << " " << 3 << " " << 38 << "\n";
    file << 1 << " " << 3 << " " << 2 << " " << 31 << "\n";
    file << 4 << " " << 2 << " " << 3 << " " << 33 << "\n";
    file << 3 << " " << 4 << " " << 2 << " " << 23 << "\n";
    file << 4 << " " << 5 << " " << 3 << " " << 22 << "\n";
    file << 4 << " " << 3 << " " << 5 << " " << 44 << "\n";
    file << 4 << " " << 3 << " " << 2 << " " << 53 << "\n";
    file << 2 << " " << 4 << " " << 3 << " " << 26 << "\n";
    file << 2 << " " << 3 << " " << 5 << " " << 51 << "\n";
    file << 2 << " " << 4 << " " << 5 << " " << 4 << "\n";
    file << 5 << " " << 3 << " " << 1 << " " << 22 << "\n";
    file.close();

    // reading file
    GraphIO graphio;
    graphio.read(testFilename);

    // expected graph
    int n = 6;
    Graph graph(n);
    graph.addEdge(1, 2, 4, 11);
    graph.addEdge(4, 1, 2, 28);
    graph.addEdge(4, 1, 3, 21);
    graph.addEdge(1, 2, 3, 38);
    graph.addEdge(1, 3, 2, 31);
    graph.addEdge(4, 2, 3, 33);
    graph.addEdge(3, 4, 2, 23);
    graph.addEdge(4, 5, 3, 22);
    graph.addEdge(4, 3, 5, 44);
    graph.addEdge(4, 3, 2, 53);
    graph.addEdge(2, 4, 3, 26);
    graph.addEdge(2, 3, 5, 51);
    graph.addEdge(2, 4, 5, 4);
    graph.addEdge(5, 3, 1, 22);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; ++j) {
            for(int k = 0; k < n; k++) {
                CHECK_EQ(graph.custo[i][j][k], graphio.getGraph().custo[i][j][k]);
            }
        }
    }
}
