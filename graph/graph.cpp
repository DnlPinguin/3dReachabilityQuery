#include "graph.h"
#include <math.h>
#include <exception>

#pragma region Constructor


using namespace std;

Graph::Graph() {}

void Graph::addEdge(int parentNode, int childNode) {
    this->GraphScheme[parentNode].push_back(childNode);
}

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


IntervalScheme::IntervalScheme() {

}
IntervalScheme::IntervalScheme(int pre, int post) {
    this->post = post;
    this->pre = pre;
}

Location::Location() {
    this->x = 0;
    this->y = 0;
}
Location::Location(coordinates x, coordinates y) {
    this->x = x;
    this->y = y;
}



LocationMap::LocationMap() {
}
