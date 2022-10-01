#include "helper.h"

/**
 * @brief Query the R-Tree by creating a plane with the query window as x,y and and postorder of the querynode as z
 * 
 * @param queryParam 
 * @param rTree 
 * @param nodeDimension 
 * @return true 
 * @return false 
 */
bool ThreeDReachReverse(queryParameter queryParam, rTreeLines* rTree, int nodeDimension)
{
    box spatialRegion = queryParam.spatialRegion;

    plane planeForQuerying(
        threeDimPoint(spatialRegion.min_corner().get<0>(), spatialRegion.min_corner().get<1>(), nodeDimension),
        threeDimPoint(spatialRegion.max_corner().get<0>(), spatialRegion.max_corner().get<1>(), nodeDimension)
    );

    for (auto it = rTree->qbegin(bgi::intersects(planeForQuerying)); it != rTree->qend(); ++it)
        {  
            if (queryParam.queryNode != it->second){
            return true;
        }
    }
    
    return false;
}

/**
 * @brief Query the R-Tree by creating a plane with the query window as x,y and and postorder of the querynode as z
 * Note: If a cuboid is hit, it doesnt automatically mean a true result, since it doesnt have to intersect with the 
 * original points. More checks are needed
 * @param queryParam 
 * @param rTree 
 * @param spatialGraph 
 * @param nodeDimension 
 * @return true 
 * @return false 
 */
bool ThreeDReachReverseCubes(queryParameter queryParam, rTreeCubes* rTree, LocationMap* spatialGraph, int nodeDimension) 
{
    box spatialRegion = queryParam.spatialRegion;
    int queryNode = queryParam.queryNode;

    coordinates x_low = spatialRegion.min_corner().get<0>();
    coordinates y_low = spatialRegion.min_corner().get<1>();
    coordinates x_upper = spatialRegion.max_corner().get<0>();
    coordinates y_upper = spatialRegion.max_corner().get<1>();

    plane planeForQuerying(
        threeDimPoint(spatialRegion.min_corner().get<0>(), spatialRegion.min_corner().get<1>(), nodeDimension),
        threeDimPoint(spatialRegion.max_corner().get<0>(), spatialRegion.max_corner().get<1>(), nodeDimension)
    );

    for (auto it = rTree->qbegin(bgi::intersects(planeForQuerying)); it != rTree->qend(); ++it)
    {
        spatialMbrRelation LocationNode = spatialGraph->Map[it->second];
        

        bool hit = checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion);

        if (!hit){
            continue;
        }
        if (queryNode != it->second) 
            return true;
    }
    return false;
}

/**
 * @brief Send multiple planes representing the intervals of the query nodes into a R-Tree of points
 * 
 * @param queryParam 
 * @param rTree 
 * @param intervals 
 * @return true 
 * @return false 
 */
bool ThreeDReach(queryParameter queryParam, rTreePoints* rTree, vector<IntervalScheme>* intervals){

    box spatialRegion = queryParam.spatialRegion;
    vector<IntervalScheme>::iterator interval;  
    for (interval = intervals->begin(); interval != intervals->end(); interval++)
    {
        plane planeForQuerying(
            threeDimPoint(spatialRegion.min_corner().get<0>(), spatialRegion.min_corner().get<1>(), interval->pre),
            threeDimPoint(spatialRegion.max_corner().get<0>(), spatialRegion.max_corner().get<1>(), interval->post)
        );
        for (auto it = rTree->qbegin(bgi::intersects(planeForQuerying)); it != rTree->qend(); ++it)
        {
            if (queryParam.queryNode != it->second){
                return true;
            }
        }
    }
    
    return false;
}

/**
 * @brief Send multiple planes representing the intervals of the query nodes into a R-Tree of plnes
 * Note: If a plane is hit, it doesnt automatically mean a true result, since it doesnt have to intersect with the 
 * original points. More checks are needed
 * @param queryParam 
 * @param rTree 
 * @param intervals 
 * @return true 
 * @return false 
 */
bool ThreeDReachPlanes(queryParameter queryParam, rTreeCubes* rTree, vector<IntervalScheme>* intervals, LocationMap* spatialGraph){
    
    box spatialRegion = queryParam.spatialRegion;
    vector<IntervalScheme>::iterator interval;  
    for (interval = intervals->begin(); interval != intervals->end(); interval++)
    {
        plane planeForQuerying(
            threeDimPoint(spatialRegion.min_corner().get<0>(), spatialRegion.min_corner().get<1>(), interval->pre),
            threeDimPoint(spatialRegion.max_corner().get<0>(), spatialRegion.max_corner().get<1>(), interval->post)
        );

        for (auto it = rTree->qbegin(bgi::intersects(planeForQuerying)); it != rTree->qend(); ++it)
        {
            spatialMbrRelation LocationNode = spatialGraph->Map[it->second];
            bool hit = checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion);
            if (!hit)
                continue;
            if (queryParam.queryNode != it->second) 
                return true;
        }
    }
    return false;
}

