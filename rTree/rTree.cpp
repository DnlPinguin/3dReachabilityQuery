#include"../main.h"

typedef int node;


/**
 * @brief Create the three dimensional R-Tree with Lines using boost library
 * A Line consists of a connection between two three dimensional points.
 * x and y for point one and point two are representing the same parameters
 * the y coordinate of point one is created by the pre value of an interval while the
 * y coordainte of point two is created by the post value of an interval
 * 
 * e.g.
 * 
 * Node with x=5 and y=7, with intervals (1,2) (4,4)
 * will create the lines (5,7,1)->(5,7,2) and (5,7,4)->(5,7,4)
 * 
 * @param HybridGraph 
 * @param LocationGraph 
 * @return rTreeLines 
 */
rTreeLines build3dRtreeWithLines(Graph* HybridGraph, LocationMap* LocationGraph) {
    cout << "Build 3D R-Tree with Lines: ";

    rTreeLines rtree;
    auto start_time = std::chrono::high_resolution_clock::now();

    int lineCounter = 0;
    for (unordered_map<int,spatialMbrRelation>::iterator iter = LocationGraph->Map.begin(); iter != LocationGraph->Map.end(); iter++)
    {
        cout << "Im here" << endl;
        int node = iter->first;
        if (HybridGraph->NodeBelongsToSCC.count(iter->first) != 0)
        {   
            node = HybridGraph->NodeBelongsToSCC[iter->first];
        }
        vector<coordinates> spatialCoordinates = iter->second.spatialData;
        cout << spatialCoordinates.size() << endl;
        for(int i = 0; i < spatialCoordinates.size(); i = i + 2){
            coordinates x = spatialCoordinates[i];
            coordinates y = spatialCoordinates[i+ 1 ];
            cout << " 2 " << endl;

            for (IntervalScheme it : HybridGraph->IntervalSchemeGraphMap[node])
            {
                threeDimLine line(
                    threeDimPoint(x, y, float(it.pre)),
                    threeDimPoint(x, y, float(it.post))
                );
                lineCounter++;
                rtree.insert(make_pair(line, iter->first));
            }
        }
        
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout << time/std::chrono::milliseconds(1) << "ms" << endl;
    cout << "R-Tree completed: " << lineCounter << " lines added.\n";
    return rtree;
}

/**
 * @brief Create the three dimensional R-Tree with Cuboids using boost library
 * A Cuboid consists of a connection between two three dimensional mbrs.
 * lowerCorner and upperCorner for mbr one and mbr two are representing the same parameters
 * the y coordinate of mbr one is created by the pre value of an interval while the
 * y coordainte of mbr two is created by the post value of an interval
 * 
 * e.g.
 *
 * mbr with lowerCorner = (3,4) and upperCorner = (7,8), with intervals (1,2) (4,4)
 * will create the two cuboids with lower and upper corners consisting of:
 *  (3,4,1),(7,8,1) -> (3,4,2),(7,8,2)  
 *  (3,4,1),(7,8,4) -> (3,4,2),(7,8,4)  
 * 
 * @param HybridGraph 
 * @param LocationGraph 
 * @return rTreeCubes 
 */
rTreeCubes build3dRtreeWithCuboids(Graph* HybridGraph, LocationMap* LocationGraph) {
    cout << "Build 3D R-Tree with cuboids: ";

    rTreeCubes rtree;
    auto start_time = std::chrono::high_resolution_clock::now();

    int counter = 0;
    unordered_map<int, spatialMbrRelation> SpatialData = LocationGraph->Map;
    for (unordered_map<int, spatialMbrRelation>::iterator iter = SpatialData.begin(); iter != SpatialData.end(); iter++)
    {
        spatialMbrRelation Locations = iter->second;
        vector<IntervalScheme> intervalData = HybridGraph->IntervalSchemeGraphMap[iter->first];
        coordinates x_1 = Locations.spatialData[0];
        coordinates y_1 = Locations.spatialData[1];
        coordinates x_2 = Locations.spatialData[2];
        coordinates y_2 = Locations.spatialData[3];

        for (IntervalScheme it : intervalData) {
            int z_1 = it.pre;
            int z_2 = it.post;
            counter++;
            plane cuboid(threeDimPoint(x_1, y_1, z_1), threeDimPoint(x_2, y_2, z_2));
            rtree.insert(make_pair(cuboid, iter->first));
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout << time/std::chrono::milliseconds(1) << "ms" <<endl;
    cout << "\tR-Tree completed." << counter << " cubes added.\n";
    return rtree;
}

/**
 * @brief Create the three dimensional R-Tree with points using boost library
 * A points consists of the spatial attributes and the postorder of the node.

 * e.g.
 *
 * point with x = 3 and y = 4 and postorder = 5
 * =>
 * (3,4,5)
 * 
 * @param HybridGraph 
 * @param LocationGraph 
 * @return rTreePoints 
 */
rTreePoints build3dRtreeWithPoints(Graph* HybridGraph, LocationMap* LocationGraph){
    cout << "Build 3D R-Tree with points: ";
    auto start_time = std::chrono::high_resolution_clock::now();

    rTreePoints rtree;
    int counter = 0;

    unordered_map<int, spatialMbrRelation> SpatialData = LocationGraph->Map;
    for (unordered_map<int,spatialMbrRelation>::iterator iter = LocationGraph->Map.begin(); iter != LocationGraph->Map.end(); iter++)
    {
        int spatialNode = iter->first;
        if (HybridGraph->NodeBelongsToSCC.count(spatialNode) != 0){
            spatialNode = HybridGraph->NodeBelongsToSCC[iter->first];
        }

        vector<coordinates> spatialCoordinates = iter->second.spatialData;
        for(int i = 0; i < spatialCoordinates.size(); i = i + 2){
            coordinates x = spatialCoordinates[i];
            coordinates y = spatialCoordinates[i+ 1 ];
            int postOrder  = HybridGraph->postOrderWithIndex[spatialNode];
            rtree.insert(make_pair(threeDimPoint(x, y, postOrder), iter->first));
            counter++;
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout << time/std::chrono::milliseconds(1) << "ms" << endl;
    cout << "R-Tree completed: " << counter << " points added.\n";
    return rtree;
}

/**
 * @brief Create the three dimensional R-Tree with planes using boost library
 * A plane consists of the spatial attributes of the mbr and the postorder of the node.

 * e.g.
 *
 * point with lowerCorner = (3,4) and upperCorner = (7,8) and postorder = 5
 * =>
 * (3,4,5) and (7,8,5)
 * 
 * @param HybridGraph 
 * @param LocationGraph 
 * @return rTreeCubes 
 */
rTreeCubes build3dRtreeWithPlanes(Graph* HybridGraph, LocationMap* LocationGraph){
    cout << "Build reverse R-Tree with planes: ";
    auto start_time = std::chrono::high_resolution_clock::now();

    rTreeCubes rtree;
    unordered_map<int, spatialMbrRelation> SpatialData = LocationGraph->Map;
    int counter = 0;
    for (unordered_map<int, spatialMbrRelation>::iterator iter = SpatialData.begin(); iter != SpatialData.end(); iter++)
    {
        spatialMbrRelation Locations = iter->second;
        int postOrder  = HybridGraph->postOrderWithIndex[iter->first];
        coordinates x_1 = Locations.spatialData[0];
        coordinates y_1 = Locations.spatialData[1];
        coordinates x_2 = Locations.spatialData[2];
        coordinates y_2 = Locations.spatialData[3];

        plane cuboid(threeDimPoint(x_1, y_1, postOrder), threeDimPoint(x_2, y_2, postOrder));
        rtree.insert(make_pair(cuboid, iter->first));
        counter++;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout << time/std::chrono::milliseconds(1) << "ms" << endl;
    cout << "R-Tree completed: " << counter << " planes added.\n";

    return rtree;
}

