
#include "graph/graph.h"
#include "helper/clock.h"
// #include "bfl/bfl.h"

#include <algorithm>
#include <thread>
#include <functional>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>

using namespace std;
using Clock = std::chrono::high_resolution_clock;

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<coordinates, 2, bg::cs::cartesian> point;
typedef bg::model::point<coordinates, 3, bg::cs::cartesian> threeDimPoint;
typedef bg::model::segment<threeDimPoint> threeDimLine;
typedef bg::model::box<point> box;
typedef bg::model::box<threeDimPoint> plane;
typedef pair<point, int> SpatialNode;
typedef pair<box, pair<int, int>> SccWithMbr;

typedef pair<plane, int> cuboidStructure;
typedef bgi::rtree<cuboidStructure, bgi::linear<16>> rTreeCubes;

typedef pair<threeDimLine,int> lineStructureForrTree;
typedef pair<threeDimPoint,int> pointStructureForrTree;

typedef bgi::rtree<lineStructureForrTree, bgi::linear<16>> rTreeLines;
typedef bgi::rtree<pointStructureForrTree, bgi::linear<16>> rTreePoints;
typedef bgi::rtree<SccWithMbr, bgi::linear<16>> rTreeSccPlanes;
typedef bgi::rtree<SpatialNode, bgi::linear<16>> rTreePlanes;


struct queryParameter
{
	int queryNode;
	box spatialRegion;
	int degree;
	int cardinality;

	queryParameter(int _queryNode, box _spatialRegion, int _degree, int _cardinality) {
		queryNode = _queryNode;
		spatialRegion = _spatialRegion;
		degree = _degree;
		cardinality = _cardinality;
	}
};


rTreeLines build3dRtreeWithLines(Graph* HybridGraph, LocationMap* LocationGraph);

rTreeCubes build3dRtreeWithCuboids(Graph* HybridGraph, LocationMap* LocationGraph);

rTreePoints build3dRtreeWithPoints(Graph* HybridGraph, LocationMap* LocationGraph);

rTreeCubes build3dRtreeWithPlanes(Graph* HybridGraph, LocationMap* LocationGraph);

rTreePlanes build2dRtreeWithPoints(LocationMap* spatialGraph);

rTreeSccPlanes build2dRtreeWithPlanes(LocationMap* spatialGraph);

bool ThreeDReachReverse(queryParameter queryParam, rTreeLines* rTree, int nodeDimension);

bool ThreeDReachReverseCubes(queryParameter queryParam, rTreeCubes* rTree, LocationMap* spatialGraph, int nodeDimension);

bool ThreeDReach(queryParameter queryParam, rTreePoints* rTree, vector<IntervalScheme>* interval);

bool ThreeDReachPlanes(queryParameter queryParam, rTreeCubes* rTree, vector<IntervalScheme>* interval, LocationMap* spatialGraph);

bool socreach(queryParameter queryParam, Graph* SocialGraph, LocationMap* spatialGraph);

bool socreachMbr(queryParameter queryParam, Graph* SocialGraph, LocationMap* spatialGraph);

bool spareach(queryParameter queryParam, Graph* SocialGraph, rTreePlanes* rTree);

bool spareachMbr(queryParameter queryParam, Graph* socialGraph,rTreeSccPlanes* rTree, LocationMap* spatialGraph);

bool spareachBfl(queryParameter queryParam, Graph* SocialGraph, rTreePlanes* rTree);

bool spareachBflMbr(queryParameter queryParam, Graph* SocialGraph, rTreeSccPlanes* rTree, LocationMap* spatialGraph);

void readPostorder(string filename, Graph* SocialGraph);

void readSuperConnectedComponents(string filename, Graph* SocialGraph);

void readSpatialData(string filename, LocationMap* SpatialGraph);

void readIntervalScheme(string filename, Graph *SocialGraph);

void readReducedGraph(string filename, Graph *SocialGraph);

void readBflForNodeIdentifer(string filename, Graph* SocialGraph);

vector<queryParameter> readQueries(string filename);


class RangeReachVertex {
public: 
	// Boolean map to Check wether a node is able to reach a node with spatial information
	unordered_map<int, bool> B_Vertex;
	// MBR of all nodes reachable by this node
	unordered_map<int, MBR> R_Vertex;
	// Vector of spatial Grids reachable by this node
	unordered_map<int, unordered_set<int>> G_Vertex;

	map<int, MBR> spatialGrid;

	// Maximum Grids spanned by all spatial nodes
	MBR maximumMBR;

	void writeAttributesToFile(string superFile);

	void readAttributesFromFile(string superFile);
	
	void createGridField(int layers);

	//Builds the RangeReachVertex according to the social and spatial Data;
	RangeReachVertex();

	bool SpaReachQuery(int node, box queryWindow, Graph* socialGraph, LocationMap* spatialGraph, int layers);
	void createGVertex(Graph* socialGraph, LocationMap* locationGraph, float MAX_REACH_GRIDS, int layers);
	void checkVertexCorrectnes();
	
	MBR getGridFieldById(int id);

	//Gets the spatial Grid field of the location
	int getSpatialGridField(Location nodeLocation);
	//Performs the rangeReach query for a node and a given spatial region
	bool rangeReach(int node, box spatialRegion);

    #pragma region PRINT STATEMENTS

	void printAttributes();

	void printGridField();

	void printAllGridLayers();
	
	void printBVertex();
	
	void printRVertex();
	
	void printGVertex();
};


