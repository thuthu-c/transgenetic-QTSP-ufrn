PROG = bin/exec
TESTPROG = bin/tests
CC = g++
CPPFLAGS = -O3 -gdwarf-3 -W -Wall -pedantic -std=c++17
MAIN = main.o

OBJS = graph.o graph_generator.o nearest_neighborhood.o cheapest_insertion.o brute_force.o benchmark.o graphio.o memetic.o branch_and_bound.o tabu.o genetic_improved.o random.o tabu_memetic.o hga.o trans_qtsp.o  trans_qtsp_v1.o trans_qtsp_v2.o trans_qtsp_v3.o trans_qtsp_v4.o
TESTOBJECTS = test_base.o test_graph.o test_graph_generator.o test_nearest_neighborhood.o test_brute_force.o test_benchmark.o test_graphio.o test_cheapest_insertion.o test_memetic.o

main: $(MAIN) $(OBJS)
	$(CC) $(MAIN) $(OBJS) -o $(PROG)
	mv *.o build/

test: $(TESTOBJECTS) $(OBJS)
	$(CC) $(CPPFLAGS) $^ -o $(TESTPROG)
	mv *.o build/

test_base.o:
	$(CC) $(CPPFLAGS) -c tests/test_base.cpp

test_graph.o: include/data_structures/graph.h
	$(CC) $(CPPFLAGS) -c tests/test_graph.cpp

test_graph_generator.o: include/benchmark/graph_generator.h
	$(CC) $(CPPFLAGS) -c tests/test_graph_generator.cpp

test_nearest_neighborhood.o: include/algorithms/nearest_neighborhood.h
	$(CC) $(CPPFLAGS) -c tests/test_nearest_neighborhood.cpp

test_brute_force.o: include/algorithms/brute_force.h
	$(CC) $(CPPFLAGS) -c tests/test_brute_force.cpp

test_benchmark.o: include/benchmark/benchmark.h
	$(CC) $(CPPFLAGS) -c tests/test_benchmark.cpp

test_graphio.o: include/helpers/graphio.h
	$(CC) $(CPPFLAGS) -c tests/test_graphio.cpp

test_cheapest_insertion.o: include/algorithms/cheapest_insertion.h
	$(CC) $(CPPFLAGS) -c tests/test_cheapest_insertion.cpp

test_memetic.o: include/algorithms/memetic.h
	$(CC) $(CPPFLAGS) -c tests/test_memetic.cpp

cheapest_insertion.o: include/algorithms/cheapest_insertion.h
	$(CC) $(CPPFLAGS) -c src/algorithms/cheapest_insertion.cpp

tabu_memetic.o: include/algorithms/tabu_memetic.h
	$(CC) $(CPPFLAGS) -c src/algorithms/tabu_memetic.cpp

main.o:
	$(CC) $(CPPFLAGS) -c src/main.cpp

graph.o: include/data_structures/graph.h
	$(CC) $(CPPFLAGS) -c src/data_structures/graph.cpp

graph_generator.o: include/benchmark/graph_generator.h
	$(CC) $(CPPFLAGS) -c src/benchmark/graph_generator.cpp

nearest_neighborhood.o: include/algorithms/nearest_neighborhood.h
	$(CC) $(CPPFLAGS) -c src/algorithms/nearest_neighborhood.cpp

memetic.o: include/algorithms/memetic.h include/helpers/random.h
	$(CC) $(CPPFLAGS) -c src/algorithms/memetic.cpp

genetic_improved.o: include/algorithms/genetic_improved.h include/helpers/random.h
	$(CC) $(CPPFLAGS) -c src/algorithms/genetic_improved.cpp

random.o: include/helpers/random.h
	$(CC) $(CPPFLAGS) -c src/helpers/random.cpp

brute_force.o: include/algorithms/brute_force.h
	$(CC) $(CPPFLAGS) -c src/algorithms/brute_force.cpp

branch_and_bound.o: include/algorithms/branch_and_bound.h
	$(CC) $(CPPFLAGS) -c src/algorithms/branch_and_bound.cpp

tabu.o: include/algorithms/tabu.h
	$(CC) $(CPPFLAGS) -c src/algorithms/tabu.cpp

benchmark.o: include/benchmark/benchmark.h
	$(CC) $(CPPFLAGS) -c src/benchmark/benchmark.cpp

graphio.o: include/helpers/graphio.h
	$(CC) $(CPPFLAGS) -c src/helpers/graphio.cpp

hga.o: include/algorithms/hga.h
	$(CC) $(CPPFLAGS) -c src/algorithms/hga.cpp

trans_qtsp.o: include/algorithms/trans_qtsp.h
	$(CC) $(CPPFLAGS) -c src/algorithms/trans_qtsp.cpp

trans_qtsp_v1.o: include/algorithms/trans_qtsp_v1.h
	$(CC) $(CPPFLAGS) -c src/algorithms/trans_qtsp_v1.cpp

trans_qtsp_v2.o: include/algorithms/trans_qtsp_v2.h
	$(CC) $(CPPFLAGS) -c src/algorithms/trans_qtsp_v2.cpp

trans_qtsp_v3.o: include/algorithms/trans_qtsp_v3.h
	$(CC) $(CPPFLAGS) -c src/algorithms/trans_qtsp_v3.cpp

trans_qtsp_v4.o: include/algorithms/trans_qtsp_v4.h
	$(CC) $(CPPFLAGS) -c src/algorithms/trans_qtsp_v4.cpp

clean:
	rm -f ./bin/exec
	rm -f ./bin/tests
	rm -f ./*.o
	rm -f "!(parameters|instances).txt"
	rm -f ./*.csv
	rm -f ./*.stdout
	rm -f ./*.stderr
