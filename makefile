OS := $(shell uname)
ifeq ($(OS),Darwin)
		CC	= /usr/local/bin/g++-ls
        CFLAGS  = -O3 -mavx -std=c++17 -w -march=native -fopenmp
        LDFLAGS =
else
        CC      = g++
        CFLAGS  = -O3 -mavx -std=c++17 -w -march=native -fopenmp
        LDFLAGS =
endif

SOURCES = rTree/rTree.cpp  query/hybridQueries.cpp graph/graph.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: runQueries

runQueries: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runQueries.cpp -o Executables/runQueries $(LDADD)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cc.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf */*.o
	rm -rf run
	#rm -rf create
