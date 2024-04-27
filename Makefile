C_FLAGS = -DNDEBUG

CPP = g++
OPTIMIZE = -O2
DEBUG_INFO = -g

SANITIZER = -fsanitize=address

MAIN_NAME  = main
OBJS = obj/$(MAIN_NAME).o obj/common.o obj/list_dump.o obj/linked_list.o obj/hash_table.o obj/hash_functions.o obj/hash_functions_analysis.o obj/strcmp_optd.o
EXEC_NAME = exec

start: main.o common.o list_dump.o linked_list.o  hash_table.o hash_funcs.o hf_analysis.o strcmp_optd.o
	g++ -no-pie -o $(EXEC_NAME) $(OBJS) -lz -lc -mavx2 -fsanitize=address

run:
	./$(EXEC_NAME)

test:
	./$(EXEC_NAME) --analyze-hash-functions

test-performance:
	./$(EXEC_NAME) --test-performance hash_table/analysis/results/performance/res.csv texts/preprocessed/words_ready_chars.txt texts/preprocessed/GeorgeRRMartin_chars.txt

test-performance-fast:
	./$(EXEC_NAME) --test-performance hash_table/analysis/results/performance/res.csv  texts/preprocessed/testcases_small_chars.txt texts/preprocessed/GeorgeRRMartin_chars.txt

main.o: $(MAIN_NAME).cpp
	$(CPP) ./$(MAIN_NAME).cpp -c $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO)  -o obj/$(MAIN_NAME).o

common.o: common/*.*
	$(CPP) ./common/common.cpp -c $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO)  -o obj/common.o

linked_list.o: linked_list/super_list.*
	$(CPP) ./linked_list/super_list.cpp -c $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO)  -o obj/linked_list.o

list_dump.o: linked_list/graph_dump/*
	$(CPP) ./linked_list/graph_dump/list_dump.cpp -c $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO)  -o obj/list_dump.o

hash_table.o: hash_table/*
	$(CPP) ./hash_table/hash_table.cpp -c $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO)  -o obj/hash_table.o

hash_funcs.o: hash_table/hash_functions/*
	$(CPP) ./hash_table/hash_functions/hash_functions.cpp -c $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO) -o obj/hash_functions.o

hf_analysis.o: hash_table/analysis/*
	$(CPP) ./hash_table/analysis/hash_functions_analysis.cpp -c $(C_FLAGS) $(OPTIMIZE) $(DEBUG_INFO)  -o obj/hash_functions_analysis.o

strcmp_optd.o: linked_list/strcmp_optd.s
	nasm -f elf64 -o obj/strcmp_optd.o linked_list/strcmp_optd.s

clean:
	rm -f $(EXEC_NAME)
	rm -f *.out
	rm -f *.exe
	rm -f *.o
	rm -f obj/*

deldumps:
	rm -f ./linked_list/graph_dump/dumps/*
	rm -f *.dot

