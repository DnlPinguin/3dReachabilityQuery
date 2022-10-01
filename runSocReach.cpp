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
        return 0;
	}

	readSuperConnectedComponents(filename + "_strongly_connected_components", &SocialGraph);
	readPostorder(filename + "_postorder", &SocialGraph);
	readSpatialData(filename + "_reduced_spatial_data", &SpatialGraph);

    readIntervalScheme(filename + "_interval_scheme_reverse", &SocialGraph);

    vector<queryParameter> queries = readQueries(queryfile);

    int amountOfExecutions = 1;


	ofstream out("data/results/" + outputFolder + "/" + filename + "_" + method +  "_result.csv");




    if (method ==  "socreach"){
        for (int i = 0; i < queries.size(); i++){
            double totalTime = 0;
            bool hit = false;
            for (int j = 0; j< amountOfExecutions; j++){
                clock.start();
                hit = socreach(queries[i], &SocialGraph, &SpatialGraph);
                totalTime = totalTime +  clock.stop(); 
            }
            double averageTime = totalTime / amountOfExecutions;
            out << fixed << averageTime << "\t" << hit << endl;
        }

    }

    if (method == "socreachMbr"){
        for (int i = 0; i < queries.size(); i++){
            double totalTime = 0;
            bool hit = false;
            for (int j = 0; j< amountOfExecutions; j++){
                clock.start();
                hit = socreachMbr(queries[i], &SocialGraph, &SpatialGraph);
                totalTime = totalTime +  clock.stop(); 
            }
            double averageTime = totalTime / amountOfExecutions;
            out << fixed << averageTime << "\t" << hit << endl;
        }
    }
}