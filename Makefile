UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
CXX=g++
CPPFLAGS=-O2 -std=c++11 -Wall -march=native
LDFLAGS=-O2 -lboost_program_options-mt
endif

ifeq ($(UNAME_S),Darwin)
CXX=llvm-g++
CPPFLAGS=-O2 -std=c++11 -Wall -march=native -I /opt/local/include/
LDFLAGS=-O2 -L/opt/local/lib -lboost_program_options-mt
endif

RM=rm -f

SRCS=main.cpp
OBJS=main.o
EXEC=main

all: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(EXEC) $(OBJS) $(LDLIBS)

%.o: %.cu 
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS) $(EXEC)

dist-clean:
	$(RM) $(EXEC)
