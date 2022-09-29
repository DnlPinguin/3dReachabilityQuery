
#include "graph/graph.h"

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

class Timer
{
private:
    Clock::time_point start_time, stop_time;

public:
    Timer();

    void start();
 
    double getElapsedTimeInSeconds();

    double stop();

};

struct queryParameter
{
	int queryNode;
	box spatialRegion;

	queryParameter(int _queryNode, box _spatialRegion) {
		queryNode = _queryNode;
		spatialRegion = _spatialRegion;
	}
};


rTreeLines build3dRtreeWithLines(Graph* HybridGraph, LocationMap* LocationGraph);

rTreeCubes build3dRtreeWithCuboids(Graph* HybridGraph, LocationMap* LocationGraph);

rTreePoints build3dRtreeWithPoints(Graph* HybridGraph, LocationMap* LocationGraph);

rTreeCubes build3dRtreeWithPlanes(Graph* HybridGraph, LocationMap* LocationGraph);


bool ThreeDReachReverse(queryParameter queryParam, rTreeLines* rTree, int nodeDimension);

bool ThreeDReachReverseCubes(queryParameter queryParam, rTreeCubes* rTree, LocationMap* spatialGraph, int nodeDimension);

bool ThreeDReach(queryParameter queryParam, rTreePoints* rTree, vector<IntervalScheme>* interval);

bool ThreeDReachPlanes(queryParameter queryParam, rTreeCubes* rTree, vector<IntervalScheme>* interval, LocationMap* spatialGraph);