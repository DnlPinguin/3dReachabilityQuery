#include "helper.h"

bool spareach(queryParameter queryParam, Graph* SocialGraph, rTreePlanes* rTree)
{
    int node = queryParam.queryNode;
    box spatialRegion = queryParam.spatialRegion;

    for (auto it = rTree->qbegin(bgi::intersects(spatialRegion)); it != rTree->qend(); ++it)
    {
        if (SocialGraph->reachNode(node, it->second)) 
        {
            return true;
        }
    }
    return false;
}

bool spareachMbr(queryParameter queryParam, Graph* socialGraph,rTreeSccPlanes* rTree, LocationMap* spatialGraph)
{
    int node = queryParam.queryNode;
    box spatialRegion = queryParam.spatialRegion;

    for (auto it = rTree->qbegin(bgi::intersects(spatialRegion)); it != rTree->qend(); ++it)
    {

        box spatialRegionOfNode = it->first;
        int currnode = it->second.second;
        int isMbr = it->second.first;

        // cout << queryParam->queryNode << " " << currnode << endl;
        pair<int, int> currnodePair = it->second;
        if (!isMbr) {
            if (socialGraph->reachNode(node, currnode)) {
               return true;
            }
        }
        else
        {
            spatialMbrRelation LocationNode = spatialGraph->Map[currnode];
            if (checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion))
            {
                if (socialGraph->reachNode(node, currnode)) {
                    return true;
                }
            }
        }
    }
    return false;
}


