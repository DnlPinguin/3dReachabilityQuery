#include "main.h"



int main(int argc, char **argv){

    Graph SocialGraph;
    LocationMap SpatialGraph;
    Timer clock;

    string filename, queryfile, method, outputFolder;
		
	if (argc == 5){
        filename = argv[1];
		queryfile = argv[2];
        method = argv[3];
        outputFolder = argv[4];

	} else {
        cout << "Input parameters are missing" << endl;
	}

	readSuperConnectedComponents(filename + "_strongly_connected_components", &SocialGraph);
	readPostorder(filename + "_postorder", &SocialGraph);
	readSpatialData(filename + "_reduced_spatial_data", &SpatialGraph);

    readIntervalScheme(filename + "_interval_scheme", &SocialGraph);

    vector<queryParameter> queries = readQueries(queryfile);

    int amountOfExecutions = 1;


	ofstream out("data/results/" + outputFolder + "/" + filename + "_" + method +  "_result.csv");


    if (method ==  "spareach"){
        rTreePlanes rTree = build2dRtreeWithPoints(&SpatialGraph);
        for (int i = 0; i < queries.size(); i++){
            double totalTime = 0;
            bool hit = false;
            for (int j = 0; j< amountOfExecutions; j++){
                clock.start();
                hit = spareach(queries[i], &SocialGraph, &rTree);
                totalTime = totalTime +  clock.stop(); 
            }
            double averageTime = totalTime / amountOfExecutions;
            out << fixed << averageTime << "\t" << hit << endl;
        }

    }

    if (method == "spareachMbr"){
        rTreeSccPlanes rTree = build2dRtreeWithPlanes(&SpatialGraph);
        for (int i = 0; i < queries.size(); i++){
            double totalTime = 0;
            bool hit = false;
            for (int j = 0; j< amountOfExecutions; j++){
                clock.start();
                hit = spareachMbr(queries[i], &SocialGraph, &rTree, &SpatialGraph);
                totalTime = totalTime +  clock.stop(); 
            }
            double averageTime = totalTime / amountOfExecutions;
            out << fixed << averageTime << "\t" << hit << endl;
        }
    }
}