#include "main.h"

/**
 * @brief Given an filled R-Tree and plane, the amount of nodes inside this window is returned
 * 
 * @param rTree 
 * @param spatialRegion 
 * @return int 
 */
int computeCardinality(rTreePlanes* rTree, box spatialRegion)
{
    vector<SpatialNode> possibleHits;
    rTree->query(bgi::intersects(spatialRegion), back_inserter(possibleHits));
    return possibleHits.size();
}

/**
 * @brief Read spatial data stored in data/reduced_spatial_data/ and fill up an R-Tree to compute the cardinality
 * of the queryboxes
 * 
 * @param filename 
 * @param rTree 
 * @return vector<coordinates> 
 */
vector<coordinates> readSpatialData(string filename, rTreePlanes* rTree) {
    cout << "Read spatial data" << endl;
    vector<coordinates> _minMaxCorners {float(INT_MAX), float(INT_MAX), float(INT_MIN), float(INT_MIN)};

    ifstream file;
    file.open("data/reduced_spatial_data/" + filename + "_reduced_spatial_data");
    string line;
    vector<coordinates> SpatialData;
    int node;
    int counter = 0;
    
    if (file.is_open()) {
        while (getline(file, line))
        {
            stringstream linestream(line);
            string data;
            while (getline(linestream, data, '\t')) {
                SpatialData.push_back(stof(data));
            }
            node = SpatialData[0];
            SpatialData.erase(SpatialData.begin());

            if (SpatialData[0] != SpatialData[2] || SpatialData[1] != SpatialData[3]){
                for (int i = 4; i < SpatialData.size() - 1; i = i+2){
                    rTree->insert(make_pair(point(SpatialData[i],SpatialData[i+1]), node));
                    counter++;
                }
            } else {
                rTree->insert(make_pair(point(SpatialData[0],SpatialData[1]), node));
                counter++;
            }

        
            if (SpatialData[0] < _minMaxCorners[0]) _minMaxCorners[0] = SpatialData[0];
            if (SpatialData[1] < _minMaxCorners[1]) _minMaxCorners[1] = SpatialData[1];
            if (SpatialData[2] > _minMaxCorners[2]) _minMaxCorners[2] = SpatialData[2];
            if (SpatialData[3] > _minMaxCorners[3]) _minMaxCorners[3] = SpatialData[3];        
            SpatialData.clear();
        }
    }
    cout << counter << " points loaded." << endl;
    return _minMaxCorners;
}


/**
 * @brief Create an specified amount of query windows. Each bucket can fill up at most {numberOfWindows} QueryWindows. 
 * Parameter {MinimumAmountOfNodesInsideWindow} specifies the least amount of nodes inside a query window to be added.
 * 
 * @param areaSizeVector 
 * @param minMaxCorner 
 * @param rtree 
 * @param numberOfWindows 
 * @return vector<vector<float>> 
 */
vector<vector<float>> createQueryBoxes(vector<float> areaSizeVector, vector<float> minMaxCorner, rTreePlanes *rtree, int numberOfWindows, int cardinalityInWindow){
    cout << "Create Query Boxes" << endl;
    vector<vector<float>> queryWindows; 

    float xLength = minMaxCorner[2] - minMaxCorner[0];
    float yLength = minMaxCorner[3] - minMaxCorner[1];
            
    for(float areaSize : areaSizeVector){
        int amountOfBoxes = 0;
        for(float x_runner = minMaxCorner[0]; x_runner < minMaxCorner[2]; x_runner += 0.10){
            for(float y_runner = minMaxCorner[1]; y_runner < minMaxCorner[3]; y_runner += 0.10){
                float stepX = xLength * areaSize;
                float stepY = yLength * areaSize;
                vector<float> area {
                    x_runner, 
                    y_runner,
                    x_runner + stepX, 
                    y_runner + stepY,
                    areaSize
                };
                int nodesInRegion = computeCardinality(rtree, box(point(area[0], area[1]), point(area[2], area[3])));
                if (nodesInRegion > cardinalityInWindow && amountOfBoxes < numberOfWindows){
                    area.push_back(nodesInRegion);
                    queryWindows.push_back(area);
                    amountOfBoxes++;
                }
            }
        }
    }
    return queryWindows;
}

/**
 * @brief Create Query Nodes for given parameters. query Nodes consist of several buckets, that are specified by vector<int> buckets.
 * e.g. veco<int>buckets {100,50,10,5,0} has nodes with degrees 101+, 51+, 11+, 6+, 1+.
 * amountOfDegreesInBucket specifies the amount of nodes in each of these buckets
 * 
 * @param SocialGeoGraph 
 * @param amountOfDegreesInBucket 
 * @return map<int,int> 
 */
map<int,int> getQueryNodes(Graph* SocialGeoGraph, int amountOfDegreesInBucket){
    cout << "Get Query nodes " << endl;
    map<int,int> node_degree_map;
    map<int,int > queryNodes;

    int queryCounter = 0;
    int lowestDegree = SocialGeoGraph->GraphScheme[0].size();
    int maxDegree = lowestDegree;
    int sumOfAllDegrees = 0;

    vector<int> buckets {100,50,10,5,0};
    vector<int> amountInBucket(buckets.size(),0);
    
    for (unordered_map<int, vector<int>>::iterator node = SocialGeoGraph->GraphScheme.begin(); node != SocialGeoGraph->GraphScheme.end(); node++) {
        int source = node->first;
        int degree = node->second.size();
        for (int i = 0; i < buckets.size(); i++){
            if (degree < 1000 && degree > buckets[i] && amountInBucket[i] < amountOfDegreesInBucket){
                node_degree_map[source] = degree;
                continue;
            }
        }
    }
    return node_degree_map;
}




int main(int argc, char **argv){
    if (argc < 5){
		cout << "Arguments missing! Try createQueries {{fileName}} {{numberOfWindowsPerArea}} {{numberOfNodesPerBucket}} {{minimum cardinality in Window}}" << endl;
		return 0;
	}

    vector<float> areaSize { 0.001, 0.005, 0.01, 0.03, 0.05 };



    string filename = argv[1];
    int numberOfBoxes = stoi(argv[2]);
    int numberOfNodesPerBucket = stoi(argv[3]);
    int cardinalityInWindow = stoi(argv[4]);

    Graph SocialGeoGraph;
    LocationMap SpatialGraph;
    readReducedGraph(filename + "_reduced_scheme", &SocialGeoGraph);

    rTreePlanes rtree;
    vector<float> minMaxCorner = readSpatialData(filename, &rtree);
    
    vector<vector<float>> areaBoxes = createQueryBoxes(areaSize, minMaxCorner, &rtree, numberOfBoxes, cardinalityInWindow);
    cout << areaBoxes.size() << " boxes found." << endl;
    map<int,int> queryNodes = getQueryNodes(&SocialGeoGraph, numberOfNodesPerBucket);
    cout << queryNodes.size() << " nodes found." << endl;

    ofstream file;
    file.open("./data/queries/" + filename +"_queries");
    file << "node\tdegree\txLow\tyLow\txTop\tyLow\tarea\tcardinality" << endl;
    for (vector<float> i : areaBoxes){
        for (map<int,int>::iterator iter = queryNodes.begin(); iter != queryNodes.end(); iter++){
            file << iter->first << "\t" << iter->second <<  "\t"<<  to_string(i[0]) << "\t" << to_string(i[1]) << "\t" << to_string(i[2]) << "\t" << to_string(i[3]) << "\t" << i[4] << "\t" << i[5] << endl;
        }
    }

    file.close();
    return 0;
}