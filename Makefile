UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	CXX=clang++
	CPPFLAGS=-O2 -std=c++11 -Wall -march=native
	LDFLAGS=-O2
endif

ifeq ($(UNAME_S),Darwin)
	CXX=clang++
	CPPFLAGS=-O2 -std=c++11 -Wall -march=native -I /opt/local/include
	LDFLAGS=-O2 -L/opt/local/lib
endif

RM=rm -f

SRCCPPS=main.cpp
SRCCUS=gc_cuda.cu

OBJCPPS=main.o
OBJCUS=gc_cuda.o
EXEC=main

all: 
	nvcc -c gc_cuda.cu
	g++ -std=c++0x api/utility.cpp main.cpp api/graph.cpp gc_cuda.o -L /usr/local/cuda/lib64 -lcudart -I /usr/local/cuda/include -lcuda -o main

gc_cuda.o:
	nvcc -c gc_cuda.cu
	
clean:
		$(RM) $(OBJS) $(EXEC) *.txt api/*.o gc_cuda.o

dist-clean:
		$(RM) $(EXEC)
