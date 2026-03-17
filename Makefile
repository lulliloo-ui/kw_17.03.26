CXXFLAGS += -Wall -Wextra -std=c++14
main: main.o
	$(CXX) $^ -0 $@
