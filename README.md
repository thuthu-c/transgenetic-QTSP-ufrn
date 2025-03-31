# Travel Salesman Second Order

## Running

### Compile and executing

After compiling (`make clean && make && make test`) there are two ways of executing it. For a full benchmark evaluation use:

```
./bin/exec <max-evaluations> <population-size> <crossover-rate> <mutation-rate>

./bin/exec 50000 100 0.9 0.1
```

The second exection is used by Irace to evaluate the genetic improvement algorithm parameters. It uses the following parameters on CLI:

```
./bin/exec <max-evaluations> <population-size> <crossover-rate> <mutation-rate> <instance>

./bin/exec 50000 100 0.9 0.1 data/FIS5-0.txt <genetic|memetic>
```

To run irace use `irace --scenario irace_config.txt` (remember to compile the program before running it)

### Running Tests

```
make clean && make && make test && ./bin/tests
```

### Results

```
make clean && make && make test && ./bin/tests && cd results && poetry run python plot-graph.py ../test_graph_generator.txt && cd ../
```
