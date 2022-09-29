#include "main.h"

Timer::Timer()
{
    start();
}

void Timer::start()
{
    start_time = Clock::now();
}

double Timer::getElapsedTimeInSeconds()
{
    return std::chrono::duration<double>(stop_time - start_time).count();
}

double Timer::stop()
{
    stop_time = Clock::now();
    return getElapsedTimeInSeconds();
}

/**
 * @brief Postorder file has the following structure
 * NodeWithPostOrderIndex1 \n 
 * NodeWithPostOrderIndex2 \n
 * NodeWithPostOrderIndex3 \n
 * NodeWithPostOrderIndex4 \n
 * .... \n
 * 
 * 
 * @param filename 
 * @param SocialGraph 
 */
void readPostorder(string filename, Graph* SocialGraph){
    cout << "Read postorder ";
    ifstream file;
    file.open(filename);
    string line;
    int counter = 1;

    int node;
    if (file.is_open()){
        while (file >> node) {
            SocialGraph->nodeHasPostorder[counter] = node;
            SocialGraph->postOrderWithIndex[node] = counter;
            counter++;
        }
    }
    cout << counter << " points read\n" << endl;
    file.close();
}

/**
 * @brief SCC file has the following structure
 * SccNode \t childNodeOne \t childNodeTwo \t ... \n
 * SccNodeTwo \t childNodeOne \t childNodeTwo \t ... \n
 * SccNodeThree \t childNodeOne \t childNodeTwo \t ... \n
 * ... \n”
 * @param filename 
 * @param SocialGraph 
 */
void readSuperConnectedComponents(string filename, Graph* SocialGraph){
    cout << "Read strongly connected components" << endl;
    ifstream file;
    file.open(filename);
    string line;
    int sccNode, node;
    bool sccDoesntExist = true;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            stringstream   linestream(line);
            string  nodeString;
            while (getline(linestream, nodeString, '\t')) {
                node = stoi(nodeString);
                if (sccDoesntExist) {
                    sccNode = node;
                    sccDoesntExist = false;
                }

                if (sccNode != node) {
                    SocialGraph->SuperConnectedComponents[sccNode].push_back(node);
                    SocialGraph->NodeBelongsToSCC[node] = sccNode;
                }
            }
            sccDoesntExist = true;
        }
    }
    file.close();
    cout << "scc file read" << endl;
}

/**
 * @brief Spatial data file needs to have the following strucutre
 * 
 * For Points:
 *       Node \t xPos \t yPos \t xPos \t yPos \n
 * For Mbrs (multiple points)
 *       Node \t xPosLowest \t yPosLowest \t xPosHighest \t yPosHighest \t xPosNodeOne \t yPosNodeOne \t xPosNodeTwo \t yPosNodeTwo \t ... \n
 * ..... \n
 * 
 * @param filename 
 * @param SpatialGraph 
 */
void readSpatialData(string filename, LocationMap* SpatialGraph){
    cout << "Read spatial data" << endl;
    vector<coordinates> _minMaxCorners {float(INT_MAX), float(INT_MAX), float(INT_MIN), float(INT_MIN)};

    ifstream file;
    file.open(filename);
    string line;
    vector<coordinates> SpatialData;
    int node;
    int counter = 0;
    
    if (file.is_open()) {
        while (getline(file, line))
        {
            counter++;
            stringstream linestream(line);
            string data;
            while (getline(linestream, data, '\t')) {
                SpatialData.push_back(stof(data));
            }
            node = SpatialData[0];
            SpatialData.erase(SpatialData.begin());

            bool isMbr;
            if (SpatialData[0] != SpatialData[2] || SpatialData[1] != SpatialData[3])
            {
                isMbr = true;
            }else
            {
                isMbr = false;
            }
            SpatialGraph->Map[node] = spatialMbrRelation(isMbr, SpatialData);

        
            if (SpatialData[0] < _minMaxCorners[0]) _minMaxCorners[0] = SpatialData[0];
            if (SpatialData[1] < _minMaxCorners[1]) _minMaxCorners[1] = SpatialData[1];
            if (SpatialData[2] > _minMaxCorners[2]) _minMaxCorners[2] = SpatialData[2];
            if (SpatialData[3] > _minMaxCorners[3]) _minMaxCorners[3] = SpatialData[3];        
            SpatialData.clear();
        }
    }
    SpatialGraph->MinMaxCorners = _minMaxCorners;
    cout << counter << " points loaded." << endl;

}

/**
 * @brief Interval scheme needs to have the following structure
 * NodeOne \t intervalOne.pre \t intervalOne.post \t intervalOne.pre \t intervalTwo.post \n 
 * NodeTwo \t intervalOne.pre \t intervalOne.post \t intervalOne.pre \t intervalTwo.post \n 
 * .... \n
 * @param filename 
 * @param SocialGraph 
 */
void readIntervalScheme(string filename, Graph *SocialGraph){
    cout << "Load interval scheme from file... " << endl;
    ifstream file;
    file.open(filename);
    string line;
    vector<int> IntervalData;
    int node;
    int counter  = 0;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            cout << line << endl;
            stringstream   linestream(line);
            string  data;
            while (getline(linestream, data, '\t')) {
                IntervalData.push_back(stoi(data));
            }
            for (int i = 2; i < IntervalData.size(); i = i+2)
            {
                counter++;
                SocialGraph->IntervalSchemeGraphMap[IntervalData[0]].push_back(IntervalScheme(IntervalData[i - 1], IntervalData[i]));
            }
            counter++;
            IntervalData.clear();
        }
    }
    file.close();
    cout << counter << " intervals loaded." << endl;
}

/**
 * @brief Query file needs to have following structure
 * QueryNodeOne \t xMin \t yMin \t xMax \t yMax \n
 * QueryNodeTwo \t xMin \t yMin \t xMax \t yMax \n
 *  .... \n
 * @param filename 
 * @return vector<queryParameter> 
 */
vector<queryParameter> readQueries(string filename){
    vector<queryParameter> queries;
    ifstream queryFile;
    int node;
    coordinates xMin, yMin, xMax, yMax;

    queryFile.open(filename);
    if (queryFile.is_open()) {
        cout << "file is open \n";
        string str;
        getline(queryFile, str);
        while (queryFile >> node >> xMin >> yMin >> xMax >> yMax ) {
            box area_window;
            queries.push_back(queryParameter(node, box(point(xMin, yMin), point(xMax, yMax))));
        }
    }
    cout << queries.size() << " queries loaded.\n";
    return queries;
}

int main(int argc, char **argv){

    Graph SocialGraph;
    LocationMap SpatialGraph;
    Timer clock;

    string filename, queryfile, method;
		
	if (argc == 4){
        filename = argv[1];
		queryfile = argv[2];
        method = argv[3];
	} else {
        cout << "Input parameters are missing" << endl;
	}

	readSuperConnectedComponents(filename + "_strongly_connected_components", &SocialGraph);
	readPostorder(filename + "_postorder", &SocialGraph);
	readSpatialData(filename + "_reduced_spatial_data", &SpatialGraph);

    // readIntervalScheme(filename + "_interval_scheme_reverse", &SocialGraph);
    if (method == "3DReach" || method == "3DReachMbr"){
        readIntervalScheme(filename + "_interval_scheme_reverse", &SocialGraph);
    }
    readIntervalScheme(filename + "_interval_scheme", &SocialGraph);

    vector<queryParameter> queries = readQueries(queryfile);

    int amountOfExecutions = 1;


	ofstream out(filename + "_" + method +  "_result.csv");



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