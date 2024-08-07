# Results

This module uses the C++ application output to plot results using Python

## plot_graph.py

C++ application can output graph as files using the following file format:

| Node 1 | Node 2 | Weight |
| ------ | ------ | ------ |
| 0      | 1      | 3      |

plot_graph.py can read this file and plot the graph using:

```shell
plot_graph.py ../path/to/graph.txt
```