C_FLAGS = -Wall -Wextra

CPP = g++
OPTIMIZE = -O1
DEBUG_INFO = -g

SANITIZER = -fsanitize=address

MAIN_NAME  = main
OBJS = $(MAIN_NAME).o common.o list_dump.o linked_list.o hash_table.o hash_functions.o hash_functions_analysis.o
EXEC_NAME = exec

start: main.o common.o list_dump.o linked_list.o  hash_table.o hash_funcs.o hf_analysis.o
	$(CPP) $(OBJS) -o $(EXEC_NAME) $(C_FLAGS) $(OPTIMIZE) -lz

run:
	./$(EXEC_NAME)

test:
	./$(EXEC_NAME) --analyze-hash-functions

test-performance:
	./$(EXEC_NAME) --test-performance hash_table/analysis/results/performance/res.csv texts/preprocessed/words_ready.txt texts/preprocessed/GeorgeRRMartin.txt

main.o: ./$(MAIN_NAME).cpp
	$(CPP) $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO) ./$(MAIN_NAME).cpp -c -o $(MAIN_NAME).o

common.o: ./common/*.*
	$(CPP) $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO) ./common/common.cpp -c -o common.o

linked_list.o: ./linked_list/super_list.*
	$(CPP) $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO) ./linked_list/super_list.cpp -c -o linked_list.o

list_dump.o: ./linked_list/graph_dump/*
	$(CPP) $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO) ./linked_list/graph_dump/list_dump.cpp -c -o list_dump.o

hash_table.o: ./hash_table/*
	$(CPP) $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO) ./hash_table/hash_table.cpp -c -o hash_table.o

hash_funcs.o: ./hash_table/hash_functions/*
	$(CPP) $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO) ./hash_table/hash_functions/hash_functions.cpp -c -o hash_functions.o

hf_analysis.o: ./hash_table/analysis/*
	$(CPP) $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO) ./hash_table/analysis/hash_functions_analysis.cpp -c -o hash_functions_analysis.o

clean:
	rm -f $(EXEC_NAME)
	rm -f *.out
	rm -f *.exe
	rm -f *.o

deldumps:
	rm -f ./linked_list/graph_dump/dumps/*
	rm -f *.dot

