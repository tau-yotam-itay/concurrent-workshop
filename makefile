CXX=g++
CXXFLAGS+=-g -std=c++11 -lpthread
SL_OBJS=main_skiplist.o priority_queue.o skiplist/skiplist.o sssp.o graph.o
MQ_OBJS=main_multiqueue.o priority_queue.o multiqueue/binary_heap.o multiqueue/multi_queue.o sssp.o graph.o
EXEC_SL=concurrent_skiplist
EXEC_MQ=concurrent_multiqueue


all: $(EXEC_SL) $(EXEC_MQ)

$(EXEC_SL):$(SL_OBJS)

	$(CXX) -o $@ $^ $(CXXFLAGS)

$(EXEC_MQ):$(MQ_OBJS)

	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(EXEC_SL) $(EXEC_MQ) $(SL_OBJS) $(MQ_OBJS)
