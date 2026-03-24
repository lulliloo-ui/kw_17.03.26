CXXFLAGS += -Wall -Wextra -std=c++14 -MMD

main: main.o
	$(CXX) $^ -o $@

-include main.d  //не важно в какой части Makefile находится 
