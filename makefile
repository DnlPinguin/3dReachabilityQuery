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

SOURCES = rTree/rTree.cpp query/bflQueries.cpp query/spareach.cpp query/3dQueries.cpp query/socReach.cpp query/helper.cpp graph/graph.cpp graph/filereader.cpp graph/geoReach.cpp helper/clock.cpp bfl/bfl.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: run3dReach runSocReach runSpaReach runGeoReach runSpaReachBfl createIntervalScheme createGeoReach createQueries createBflFile

createBflFile: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createBflFile.cpp -o Executables/createBflFile $(LDADD)

createQueries: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createQueries.cpp -o Executables/createQueries $(LDADD)

createGeoReach: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createGeoReach.cpp -o Executables/createGeoReach $(LDADD)

runGeoReach: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runGeoReach.cpp -o Executables/runGeoReach $(LDADD)

createIntervalScheme: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) createIntervalScheme.cpp -o Executables/createIntervalScheme $(LDADD)

run3dReach: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) run3dReach.cpp -o Executables/run3dReach $(LDADD)

runSocReach: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runSocReach.cpp -o Executables/runSocReach $(LDADD)

runSpaReach: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runSpaReach.cpp -o Executables/runSpaReach $(LDADD)

runSpaReachBfl: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) runSpaReachBfl.cpp -o Executables/runSpaReachBfl $(LDADD)


.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cc.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf */*.o
	rm -rf run
	#rm -rf create
