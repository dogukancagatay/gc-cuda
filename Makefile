UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
CXX=nvcc
CPPFLAGS=-O2 --compiler-options "-std=c++0x"
LDFLAGS=-O2
endif

ifeq ($(UNAME_S),Darwin)
CXX=nvcc
CPPFLAGS=-O2 -I /opt/local/include --compiler-options "-std=c++0x"
LDFLAGS=-O2 -L/opt/local/lib
endif

RM=rm -f

SRCS=main.cpp
OBJS=main.o
EXEC=main

all: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(EXEC) $(OBJS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJS) $(EXEC)

dist-clean:
	$(RM) $(EXEC)
