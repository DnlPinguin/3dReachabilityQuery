#include "helper.h"


bool socreach(queryParameter queryParam, Graph* socialGraph, LocationMap* spatialGraph)
{
    int node = queryParam.queryNode;
    if (socialGraph->NodeBelongsToSCC.count(node) != 0){
        node = socialGraph->NodeBelongsToSCC[node];
    }

    box spatialRegion = queryParam.spatialRegion;
    vector<IntervalScheme>*  Intervals = &(socialGraph->IntervalSchemeGraphMap[node]);
    for (vector<IntervalScheme>::iterator iter = Intervals->begin() ; iter != Intervals->end(); iter++)
    {
        for (int it = iter->pre; it <= iter->post; it++) 
        {
            int reachableNode = socialGraph->nodeHasPostorder[it];
            if (spatialGraph->Map.count(reachableNode) > 0)
            {
                spatialMbrRelation LocationNode = spatialGraph->Map[reachableNode];
                box mbr = box(point(LocationNode.spatialData[0], LocationNode.spatialData[1]), point(LocationNode.spatialData[0], LocationNode.spatialData[1]));
                if (!LocationNode.isMbr) 
                {
                    if (boost::geometry::intersects(spatialRegion, mbr))
                    {
                        if (reachableNode != queryParam.queryNode)
                            return true;
                    }
                }
                else
                {
                    for (int i = 4; i != LocationNode.spatialData.size(); i++)
                    {
                        box mbr = box(point(LocationNode.spatialData[i], LocationNode.spatialData[(i + 1)]), point(LocationNode.spatialData[i], LocationNode.spatialData[(i + 1)]));
                        if (boost::geometry::intersects(spatialRegion, mbr))
                        {
                            if (reachableNode != queryParam.queryNode) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool socreachMbr(queryParameter queryParam, Graph* socialGraph, LocationMap* spatialGraph)
{
    int node = queryParam.queryNode;
    if (socialGraph->NodeBelongsToSCC.count(node) != 0){
        node = socialGraph->NodeBelongsToSCC[node];
    }
    box spatialRegion = queryParam.spatialRegion;

    vector<IntervalScheme>*  Intervals = &(socialGraph->IntervalSchemeGraphMap[node]);
    for (vector<IntervalScheme>::iterator iter = Intervals->begin() ; iter != Intervals->end(); iter++)
    {
        for (int it = iter->pre; it <= iter->post; it++) 
        {
            int reachableNode = socialGraph->nodeHasPostorder[it];
            if (spatialGraph->Map.count(reachableNode) > 0)
            {

                spatialMbrRelation LocationNode = spatialGraph->Map[reachableNode];
                bool hit = checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion); 
            
                if (hit)
                {
                    if (queryParam.queryNode != reachableNode)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}