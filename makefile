CXX=g++
CXXFLAGS+=-g -std=c++11 -lpthread
OBJS=main.o binary_heap.o multi_queue.o sssp.o tests.o graph.o
EXEC=concurrent_workshop


all: $(EXEC)

$(EXEC):$(OBJS)

	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)
