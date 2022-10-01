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

    if (method == "3DReach" || method == "3DReachMbr"){
        readIntervalScheme(filename + "_interval_scheme_reverse", &SocialGraph);
    }
    readIntervalScheme(filename + "_interval_scheme", &SocialGraph);

    vector<queryParameter> queries = readQueries(queryfile);

    int amountOfExecutions = 1;


	ofstream out("data/results/" + outputFolder + "/"+ filename + "_" + method +  "_result.csv");



    if (method == "3DReachReverse"){
        rTreeLines rTree = build3dRtreeWithLines(&SocialGraph, &SpatialGraph);
        for (int i = 0; i < queries.size(); i++){
            double totalTime = 0;
            bool hit = false;
            for (int j = 0; j< amountOfExecutions; j++){
                clock.start();
                hit = ThreeDReachReverse(queries[i], &rTree, SocialGraph.postOrderWithIndex[queries[i].queryNode]);
                totalTime = totalTime +  clock.stop(); 
            }
            double averageTime = totalTime / amountOfExecutions;
            out << fixed << averageTime << "\t"  <<  hit << endl;
        }
    } 

    if (method == "3DReachReverseMbr"){
        rTreeCubes rTree = build3dRtreeWithCuboids(&SocialGraph, &SpatialGraph);
        for (int i = 0; i < queries.size(); i++){
            double totalTime = 0;
            bool hit = false;
            for (int j = 0; j< amountOfExecutions; j++){
                clock.start();
                hit = ThreeDReachReverseCubes(queries[i], &rTree, &SpatialGraph, SocialGraph.postOrderWithIndex[queries[i].queryNode]);
                totalTime = totalTime +  clock.stop(); 
            }
            double averageTime = totalTime / amountOfExecutions;
            out << fixed << averageTime << "\t" << hit << endl;

        }
    }

    if (method ==  "3DReach"){
        rTreePoints rTree = build3dRtreeWithPoints(&SocialGraph, &SpatialGraph);
        for (int i = 0; i < queries.size(); i++){
            double totalTime = 0;
            bool hit = false;
            for (int j = 0; j< amountOfExecutions; j++){
                clock.start();
                hit = ThreeDReach(queries[i], &rTree, &(SocialGraph.IntervalSchemeGraphMap[queries[i].queryNode]));
                totalTime = totalTime +  clock.stop(); 
            }
            double averageTime = totalTime / amountOfExecutions;
            out << fixed << averageTime << "\t" << hit << endl;
        }

    }

    if (method == "3DReachMbr"){
        rTreeCubes rTree = build3dRtreeWithPlanes(&SocialGraph, &SpatialGraph);
        for (int i = 0; i < queries.size(); i++){
            double totalTime = 0;
            bool hit = false;
            for (int j = 0; j< amountOfExecutions; j++){
                clock.start();
                hit = ThreeDReachPlanes(queries[i], &rTree, &(SocialGraph.IntervalSchemeGraphMap[queries[i].queryNode]), &SpatialGraph);
                totalTime = totalTime +  clock.stop(); 
            }
            double averageTime = totalTime / amountOfExecutions;
            out << fixed << averageTime << "\t" << hit << endl;
        }
    }
}