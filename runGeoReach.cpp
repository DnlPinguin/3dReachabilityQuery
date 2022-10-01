#include "main.h"

string getFileName(){
	cout << "Enter filename:";
	string input;
	cin >> input;
	return input;
}

int getLayer(){
    cout << "Enter amount of grids in top layer:";
	string input;
	cin >> input;
	return stoi(input);
}

int main(int argc, char **argv) {
	string filename, queryFile, outputFolder;
	int layers;
	if (argc == 4){
		filename = argv[1];
		queryFile = argv[2];
		outputFolder = argv[3];
	}else{
		filename = getFileName();
    }
    Timer clock;
    Graph SocialGeoGraph;
    LocationMap LocationGraph;
    RangeReachVertex RangeReach;


	RangeReach.readAttributesFromFile(filename);
    readReducedGraph(filename + "_reduced_scheme", &SocialGeoGraph);
    readSuperConnectedComponents(filename + "_strongly_connected_components", &SocialGeoGraph);
    readSpatialData(filename + "_reduced_spatial_data", &LocationGraph);

	vector<queryParameter> queries = readQueries(queryFile);
	int amount_of_queries_used_for_averaging = 1; 
	ofstream out("./data/results/"+ outputFolder + "/" + filename + "_geoReach_results.csv");
	out << "time\tresult\tarea\tdegree\tcardinality\n";

	for (vector<queryParameter>::iterator it = queries.begin(); it != queries.end(); it++)
	{
		
        double totalTime = 0;
		bool result;
		for (int i = 0; i < amount_of_queries_used_for_averaging; i++){
			clock.start();
			result = RangeReach.SpaReachQuery(it->queryNode, it->spatialRegion, &SocialGeoGraph, &LocationGraph, layers);
			totalTime += clock.stop();
		}
		double timer = totalTime / amount_of_queries_used_for_averaging; 
		out << fixed << timer << "\t" << result << "\n";
	}

    return 0;
}