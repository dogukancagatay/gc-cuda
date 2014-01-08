CXX=g++
CPPFLAGS=-O2 -std=c++11
LDFLAGS=-O2
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
