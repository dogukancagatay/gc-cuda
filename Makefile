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

SRCS=main.cpp
OBJS=main.o
EXEC=main

all: $(OBJS)
		$(CXX) $(LDFLAGS) -o $(EXEC) $(OBJS) $(LDLIBS)

%.o: %.cpp
		$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
		$(RM) $(OBJS) $(EXEC) *.txt

dist-clean:
		$(RM) $(EXEC)
