CXX=g++
CXXFLAGS+=-g -std=c++11 -lpthread
OBJS=graph.o multi_queue.o binary_heap.o main.o sssp.o tests.o
EXEC=concurrent_workshop

$(EXEC):$(OBJS)
	$(CXX) $(OBJS) -o $@
main.o: main.cpp graph.h sssp.h tests.h
	$(CXX) $(CXXFLAGS) -c $*.cpp
sssp.o: sssp.cpp multi_queue.h
	$(CXX) $(CXXFLAGS) -c $*.cpp
multi_queue.o: multi_queue.cpp binary_heap.h
	$(CXX) $(CXXFLAGS) -c $*.cpp
graph.o: graph.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp
tests.o: tests.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp
binary_heap.o: binary_heap.cpp graph.h
	$(CXX) $(CXXFLAGS) -c $*.cpp



clean:
	rm -f concurrent_workshop $(OBJS)
