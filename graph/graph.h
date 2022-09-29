
#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <map>
#include <vector>

typedef float coordinates;


struct IntervalScheme {
    int pre;
    int post;

public:
    IntervalScheme();
    IntervalScheme(int pre, int post);
};

using namespace std;

class Graph {
    public:
        // Set of all Vertices
        unordered_map<int, int> postOrderWithIndex;
        unordered_map<int, int> nodeHasPostorder;

        //Representation of Graphs as adjancency list.
        unordered_map <int, vector<int>> GraphScheme;

        //All Information for the super connected components.
        unordered_map<int, vector<int>> SuperConnectedComponents;
        unordered_map<int, int> NodeBelongsToSCC;

        //Interval Scheme Map of the propagated graph.
        map <int, vector<IntervalScheme>> IntervalSchemeGraphMap;

        //Basic Graph Constructor
        Graph();
        //Adds Edge between node parentNode and childNode
        void addEdge(int parentNode, int childNode);
        //Checks if the given node is reachable depending on the IntervalScheme.
        bool reachNode(int nodeOne, int nodeTwo); 
};

struct Location {
    coordinates x;
    coordinates y;
    public:
        Location();
        Location(coordinates x, coordinates y);
};

struct spatialMbrRelation 
{
	bool isMbr;
	vector<coordinates> spatialData;

	spatialMbrRelation() {
		isMbr = false;
		spatialData = {};
	}
	spatialMbrRelation(bool _isMBr, vector<coordinates> _spatialData)
	{
		isMbr = _isMBr;
		spatialData = _spatialData;
	}
};


class LocationMap {
public:
    unordered_map<int, spatialMbrRelation> Map;
    vector<coordinates> MinMaxCorners;
    LocationMap();
};


#endif