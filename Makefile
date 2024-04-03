C_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations  \
-Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wconversion 						\
-Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers 			\
-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel 			\
-Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

CPP = g++
OPTIMIZE = -O0
SANITIZER = -fsanitize=address

MAIN_NAME  = main
OBJS = $(MAIN_NAME).o common.o list_dump.o linked_list.o hash_table.o hash_functions.o
EXEC_NAME = exec

start: main.o common.o list_dump.o linked_list.o  hash_table.o hash_funcs.o
	$(CPP) $(OBJS) -o $(EXEC_NAME) $(C_FLAGS) $(OPTIMIZE)

run:
	./$(EXEC_NAME)

test:
	./$(EXEC_NAME) --test-performance

main.o: ./$(MAIN_NAME).cpp
	$(CPP) ./$(MAIN_NAME).cpp -c -o $(MAIN_NAME).o

common.o: ./common/*.*
	$(CPP) ./common/common.cpp -c -o common.o

linked_list.o: ./linked_list/super_list.*
	$(CPP) ./linked_list/super_list.cpp -c -o linked_list.o

list_dump.o: ./linked_list/graph_dump/*
	$(CPP) ./linked_list/graph_dump/list_dump.cpp -c -o list_dump.o

hash_table.o: ./hash_table/*
	$(CPP) ./hash_table/hash_table.cpp -c -o hash_table.o

hash_funcs.o: ./hash_table/hash_functions/*
	$(CPP) ./hash_table/hash_functions/hash_functions.cpp -c -o hash_functions.o

clean:
	rm -f $(EXEC_NAME)
	rm -f *.out
	rm -f *.exe
	rm -f *.o
