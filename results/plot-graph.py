import sys
import networkx as nx
import matplotlib.pyplot as plt


def plot_graph_from_file(filename: str):
    G = nx.Graph()

    with open(filename, 'r') as f:
        lines = f.readlines()
    
        for line in lines:
            data = line.strip().split(" ")
            G.add_edge(data[0], data[1], weight=4)

    nx.draw(G, with_labels = True)
    plt.savefig("plot-graph.png")

if __name__ == '__main__':
    plot_graph_from_file(sys.argv[1])
