#include "graph.h"
#include <math.h>
#include <exception>

#pragma region Constructor


using namespace std;

/**
 * @brief Construct a new Graph:: Graph object
 * 
 */
Graph::Graph() {}

/**
 * @brief Add edge into a graph
 * GraphSchemeReverse is needed for creating inverted interval schemes, required for social and hybrid methods
 * 
 * @param parentNode 
 * @param childNode 
 */
void Graph::addEdge(int parentNode, int childNode) {
    this->GraphScheme[parentNode].push_back(childNode);
    this->GraphSchemeReverse[childNode].push_back(parentNode);
}


/**
 * @brief Performs basic reachability check using the interval labeling scheme. If the postOrder of
 * the source node is inside an interval of the target not. The target is reachable, otherwise not.
 * 
 * e.g.
 * 
 * Node 5 with postorder 6 is able to reach Node 7 with Intervals (1,7)(6,8)
 * Node 1 with postorder 10 is not able to reach Node 3 with Intervals (1,9)(13,13)
 * 
 * @param nodeOne 
 * @param nodeTwo 
 * @return true 
 * @return false 
 */
bool Graph::reachNode(int nodeOne, int nodeTwo) {
    if (nodeOne == nodeTwo) {
        return false;
    }
    if (NodeBelongsToSCC.count(nodeOne) != 0) {
        nodeOne = NodeBelongsToSCC[nodeOne];
    }
    for (auto it : IntervalSchemeGraphMap[nodeTwo]) 
    {
        if (postOrderWithIndex[nodeOne] >= it.pre && postOrderWithIndex[nodeOne] <= it.post)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Construct a new Interval Scheme:: Interval Scheme object
 * 
 */
IntervalScheme::IntervalScheme() {

}

/**
 * @brief Construct a new Interval Scheme:: Interval Scheme object
 * 
 * @param pre 
 * @param post 
 */
IntervalScheme::IntervalScheme(int pre, int post) {
    this->post = post;
    this->pre = pre;
}

/**
 * @brief Construct a new Location:: Location object
 * 
 */
Location::Location() {
    this->x = 0;
    this->y = 0;
}

/**
 * @brief Construct a new Location:: Location object
 * 
 * @param x 
 * @param y 
 */
Location::Location(coordinates x, coordinates y) {
    this->x = x;
    this->y = y;
}

/**
 * @brief Construct a new Location Map:: Location Map object
 * 
 */
LocationMap::LocationMap() {
}

/**
 * @brief Construct a new MBR::MBR object
 * 
 */
MBR::MBR() {

}

/**
 * @brief Construct a new MBR::MBR object
 * 
 * @param loc 
 */
MBR::MBR(Location loc) {
	xMax = loc.x;
	xMin = loc.x;
	yMax = loc.y;
	yMin = loc.y;
}

/**
 * @brief Construct a new MBR::MBR object
 * 
 * @param _xMin 
 * @param _yMin 
 * @param _xMax 
 * @param _yMax 
 */
MBR::MBR(coordinates _xMin, coordinates _yMin, coordinates _xMax, coordinates _yMax) {
	xMax = _xMax;
	xMin = _xMin;
	yMax = _yMax;
	yMin = _yMin;
}

/**
 * @brief 
 * 
 * @param mbr 
 */
void MBR::insertMBR(MBR mbr){
	if (mbr.xMax > xMax) { xMax = mbr.xMax; }
	if (mbr.xMin < xMin) { xMin = mbr.xMin; }
	if (mbr.yMax > yMax) { yMax = mbr.yMax; }
	if (mbr.yMin < yMin) { yMin = mbr.yMin; }
}

/**
 * @brief 
 * 
 * @param loc 
 */
void MBR::insertLoc(Location loc) {
	if (loc.x > xMax) { xMax = loc.x; }
	if (loc.x < xMin) { xMin = loc.x; }
	if (loc.y > yMax) { yMax = loc.y; }
	if (loc.y < yMin) { yMin = loc.y; }
}


/**
 * @brief 
 * 
 * @param loc 
 * @return true 
 * @return false 
 */
bool MBR::containsPoint(Location loc){
	if (loc.x < xMin || loc.x > xMax){
		return false;
	}
	if (loc.y < yMin || loc.y > yMax){
		return false;
	}
	return true;
}

/**
 * @brief 
 * 
 * @return string 
 */
string MBR::stringify() {
	return to_string(xMin) + "\t" + to_string(yMin) + "\t"  + to_string(xMax) +"\t"  + to_string(yMax);
}

/**
 * @brief 
 * 
 * @param other 
 * @return true 
 * @return false 
 */
bool MBR::overlaps(MBR other) {
	if (this->xMin <= other.xMin && this->xMax >= other.xMax && this->yMin <= other.yMin && this->yMax >= other.yMax) {
		return true;
	}
	return false;
}


