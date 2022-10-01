#include "main.h"


string getFileName(){
	cout << "Enter filename:";
	string input;
	cin >> input;
	return input;
}

int main(int argc, char **argv){
    string filename;
	if (argc == 2)
    {
        filename = argv[1];
	} else {
        filename = getFileName();
	}

    Graph SocialGeoGraph;
    LocationMap LocationGraph;
    RangeReachVertex RangeReach;

    readReducedGraph(filename + "_reduced_scheme", &SocialGeoGraph);
    readPostorder(filename + "_postorder", &SocialGeoGraph);
    readSuperConnectedComponents(filename + "_strongly_connected_components", &SocialGeoGraph);
    readSpatialData( filename + "_reduced_spatial_data", &LocationGraph);



    auto start_time = std::chrono::high_resolution_clock::now();
	RangeReach.maximumMBR = MBR(LocationGraph.MinMaxCorners[0],LocationGraph.MinMaxCorners[1],LocationGraph.MinMaxCorners[2],LocationGraph.MinMaxCorners[3]);
    
    RangeReach.createGridField(3);
    cout << "create SPA-Graph structure  \n";
    RangeReach.createGVertex(&SocialGeoGraph, &LocationGraph, 3, 3);


    float totalSizeOfArea = fabs(RangeReach.maximumMBR.xMax - RangeReach.maximumMBR.xMin) * (RangeReach.maximumMBR.yMax - RangeReach.maximumMBR.yMin);
    RangeReach.writeAttributesToFile(filename);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout << "creation time " << time/std::chrono::milliseconds(1) << endl;;

    return 0;
}