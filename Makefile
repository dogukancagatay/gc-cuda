CXX=g++
CPPFLAGS=-O2 -std=c++0x -march=native -I /usr/local/cuda/include
LDFLAGS=-O2 -L/usr/local/cuda/lib64 
LDLIBS=-lcudart -lcuda

CUC=nvcc
CUFLAGS=-O2
CULDFLAGS=-O2

RM=rm -f

SRCCPPS=main.cpp api/utility.cpp api/graph.cpp
OBJCPPS=main.o utility.o graph.o

SRCCUS=gc_cuda.cu
OBJCUS=gc_cuda.o

EXEC=main

all: gc_cuda.o main.o utility.o graph.o
	$(CXX) $(LDFLAGS) -o $(EXEC) $(OBJCUS) $(OBJCPPS) $(LDLIBS)

gc_cuda.o: gc_cuda.cu
	$(CUC) $(CUFLAGS) -c $<
	
main.o: main.cpp 
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

graph.o: api/graph.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

utility.o: api/utility.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJCPPS) $(OBJCUS) $(EXEC) *.txt api/*.o 

dist-clean:
	$(RM) $(EXEC)
