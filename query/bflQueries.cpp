#include "helper.h"
#include "../bfl/bfl.h"

bool spareachBfl(queryParameter queryParam, Graph* SocialGraph, rTreePlanes* rTree)
{
    int node = queryParam.queryNode;
    box spatialRegion = queryParam.spatialRegion;

    for (auto it = rTree->qbegin(bgi::intersects(spatialRegion)); it != rTree->qend(); ++it)
    {
        int u = SocialGraph->nodeToBflIdentifier[node];
        int v = SocialGraph->nodeToBflIdentifier[it->second]; 
        if (u == v)
            continue;

        if (run_single_bfl_query(u,v)) 
        {
            return true;
        }
    }
    return false;
};

bool spareachBflMbr(queryParameter queryParam, Graph* SocialGraph, rTreePlanes* rTree, LocationMap* spatialGraph)
{
    int node = queryParam.queryNode;
    box spatialRegion = queryParam.spatialRegion;

    for (auto it = rTree->qbegin(bgi::intersects(spatialRegion)); it != rTree->qend(); ++it)
    {
        box spatialRegionOfNode = it->first;
        int currnode = it->second.second;
        int isMbr = it->second.first;
        pair<int, int> currnodePair = it->second;

        int u,v;
        u = node;
        if (socialGraph->NodeBelongsToSCC.count(node) != 0)
        {
            u = socialGraph->NodeBelongsToSCC[node]; 
        }
        v = currnode;
        u = socialGraph->nodeToBflIdentifier[u];
        v = socialGraph->nodeToBflIdentifier[v]; 
        if (!isMbr) {
            if (run_single_bfl_query(u,v)) {
               return true;
            }
        }
        else
        {
            spatialMbrRelation LocationNode = spatialGraph->Map[currnode];
            if (checkIfNodeIsInSpatialRegion(LocationNode.isMbr, LocationNode.spatialData, spatialRegion))
            {
                if (run_single_bfl_query(u,v)) {
                    return true;
                }
            }
        }
    }
    return false;
};
