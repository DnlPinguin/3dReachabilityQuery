#include "helper.h"


bool checkIfRectangleIsContainedInRegion(box rectangle, box spatialRegion) {
    return boost::geometry::within(rectangle, spatialRegion);
}

bool checkIfOneEdgeIsContainedInRectangle(box rectangle, box spatialRegion)
{
    coordinates left_x = rectangle.min_corner().get<0>();
    coordinates bottom_y = rectangle.min_corner().get<1>();

    coordinates right_x = rectangle.max_corner().get<0>();
    coordinates top_y = rectangle.max_corner().get<1>();


    coordinates min_x = spatialRegion.min_corner().get<0>();
    coordinates min_y = spatialRegion.min_corner().get<1>();
    coordinates max_x = spatialRegion.max_corner().get<0>();
    coordinates max_y = spatialRegion.max_corner().get<1>();

    
    point left_bottom_corner(left_x, bottom_y);
    point right_upper_corner(right_x, top_y);
    point left_upper_corner(left_x, top_y);
    point right_bottom_corner(right_x, bottom_y);

    box edge = box (left_bottom_corner, right_bottom_corner);
    if (left_x >= min_x && right_x <= max_x && bottom_y >= min_y && bottom_y <= max_y)
    {
        return true; 
    }

    edge = box(left_bottom_corner, left_upper_corner);
    if (left_x >= min_x && left_x <= max_x && bottom_y >= min_y && top_y <= max_y)
    {
        return true; 
    }

    edge = box(right_bottom_corner, right_upper_corner);
    if (right_x >= min_x && right_x <= max_x && bottom_y >= min_y && top_y <= max_y)
    {
        return true;
    }
    edge = box(left_upper_corner, right_upper_corner);
    if (left_x >= min_x && right_x <= max_x && top_y >= min_y && top_y <= max_y)
    {
        return true;
    }
    return false;
}

bool checkIfNodeIsInSpatialRegion(bool isMbr, vector<coordinates> LocationNode, box spatialRegion){
    int counter = 0; 
    if (isMbr) 
    {
        box mbr = box(point(LocationNode[0], LocationNode[1]), point(LocationNode[2], LocationNode[3]));
        if (checkIfRectangleIsContainedInRegion(mbr, spatialRegion)) 
        {
            return true;
        }
        counter++;
        if (checkIfOneEdgeIsContainedInRectangle(mbr, spatialRegion)) 
        {
            return true;
        }
        counter++;
        if (boost::geometry::intersects(spatialRegion, mbr)) 
        {
            for (int i = 4; i != LocationNode.size(); i++) 
            {
                box mbr = box(point(LocationNode[i], LocationNode[(i + 1)]), point(LocationNode[i], LocationNode[(i + 1)]));
                counter++;
                if (boost::geometry::intersects(spatialRegion, mbr))
                {
                    return true;
                }
            }
        }
    }
    else 
    {
        box mbr = box(point(LocationNode[0], LocationNode[1]), point(LocationNode[0], LocationNode[1]));
        counter++;
        if (boost::geometry::intersects(spatialRegion, mbr))
        {
            return true;
        }
    }
    return false;
}
