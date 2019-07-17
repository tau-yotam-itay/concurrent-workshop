CXX=g++
CXXFLAGS+=-g -std=c++11 -lpthread
OBJS=main.o priority_queue.o multiqueue/binary_heap.o multiqueue/multi_queue.o skiplist/skiplist.o sssp.o tests.o graph.o
EXEC=concurrent_workshop


all: $(EXEC)

$(EXEC):$(OBJS)

	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)
