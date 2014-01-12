UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
CXX=clang++
CPPFLAGS=-O2 -std=c++11 -Wall -march=native
LDFLAGS=-O2 
endif

ifeq ($(UNAME_S),Darwin)
CXX=clang++
CPPFLAGS=-O2 -std=c++11 -Wall -march=native
LDFLAGS=-O2
endif

RM=rm -f

SRC1=from_orig_format.cpp
OBJ1=from_orig_format.o
EXE1=from_orig_format

SRC2=to_orig_format.cpp
OBJ2=to_orig_format.o
EXE2=to_orig_format

all: $(EXE1) $(EXE2)


$(EXE1): $(OBJ1)
	$(CXX) $(LDFLAGS) -o $(EXE1) $(OBJ1) $(LDLIBS)

$(EXE2): $(OBJ2)
	$(CXX) $(LDFLAGS) -o $(EXE2) $(OBJ2) $(LDLIBS)

%.o: %.cpp 
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(OBJ1) $(EXE1)
	$(RM) $(OBJ2) $(EXE2)

dist-clean:
	$(RM) $(EXE1)
	$(RM) $(EXE2)
