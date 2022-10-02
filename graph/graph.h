
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
        // Node to postorder relation
        vector<int> postOrder;
        unordered_map<int, int> postOrderWithIndex;
        unordered_map<int, int> nodeHasPostorder;

        // Node to Bfl Identifier        
        unordered_map<int, int> nodeToBflIdentifier;
        unordered_map<int, int> bflIdentifierToNode;
        
        //Representation of Graphs as adjancency list.
        unordered_map <int, vector<int>> GraphScheme;
        unordered_map <int, vector<int>> GraphSchemeReverse;

        //All Information for the super connected components.
        unordered_map<int, vector<int>> SuperConnectedComponents;
        unordered_map<int, int> NodeBelongsToSCC;

        //Interval Scheme Map of the propagated graph.
        map <int, vector<IntervalScheme>> IntervalSchemeGraphMap;

        //Basic Graph Constructor
        Graph();
        void addEdge(int parentNode, int childNode);
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

struct MBR {
public:
	coordinates xMin;
	coordinates xMax;
	coordinates yMin;
	coordinates yMax;
	MBR();
	// Creates a Minimum Bounding Reactangle according to the parameter;
	MBR(Location loc);

	MBR(coordinates _xMin, coordinates _yMin, coordinates _xMax, coordinates _yMax);

	//Expands the Miniumum Bound Reactangle according to the parameter;
	void insertLoc(Location loc);
	void insertMBR(MBR mbr);
	bool containsPoint(Location loc);
	
	void print();
	string getString();
	string stringify();
	//Checks if the MBR overlaps with the seconds MBR
	bool overlaps(MBR other);
};



#endif