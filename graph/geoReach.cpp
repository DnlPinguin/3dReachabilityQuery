#include "../main.h"

/*
Checks whether the sspatial grid is intersecting with the spatial region. If not we can say the node is not reaching any nodes lying the the spatial target region
*/

bool intersects(MBR queryArea, box queryWindow)
{
	box rec1 = box(point(queryArea.xMin, queryArea.yMin), point(queryArea.xMax, queryArea.yMax));
	return  boost::geometry::intersects(rec1, queryWindow);
}

/*
 Checks if the spatial region is overlapping the spatial grid returned by the vertex. If so, the whole grid lies inside the spatial region and the node is automatically
in this region too.
*/
bool overlaps(MBR spatialGrid, box queryWindow)
{	
	if (spatialGrid.xMin < queryWindow.min_corner().get<0>())
		return false;
	
	if (spatialGrid.yMin < queryWindow.min_corner().get<1>()){
		return false;
	}

	if (spatialGrid.yMax > queryWindow.max_corner().get<1>()){
		return false;
	}
	if (spatialGrid.xMax > queryWindow.max_corner().get<0>()){
		return false;
	}
	return true;
}


RangeReachVertex::RangeReachVertex(){};

/**
 * @brief Create default empty Grid Cell. Layer 0 has LAYERS x LAYERS dimensions
 * E.g Layers Parameter 4 creates
 * Layer 0 : 4x4 = 16 fields
 * Layer 1 : 2x2 = 4 fields
 * Layer 2 : 1x1 = 1 field
 */
void RangeReachVertex::createGridField(int LAYERS)
{
	cout << "Create " << LAYERS << "x" << LAYERS << " grid field" << endl;
	float chunkStepX = abs(maximumMBR.xMax - maximumMBR.xMin) / LAYERS;
	float chunkStepY = abs(maximumMBR.yMax - maximumMBR.yMin) / LAYERS;

	int counter = 0;
	for (int x_row = 0; x_row < LAYERS; x_row++)
	{
		for (int y_row = 0; y_row < LAYERS; y_row++)
		{
			Location gridBox(maximumMBR.xMin + (x_row * chunkStepX), maximumMBR.yMin + (y_row * chunkStepY));
			Location gridBoxUpper(maximumMBR.xMin + ((x_row + 1) * chunkStepX), maximumMBR.yMin + ((y_row + 1) * chunkStepY));
			MBR spatialGridCell(gridBox);
			spatialGridCell.insertLoc(gridBoxUpper);
			this->spatialGrid[counter] = spatialGridCell;
			counter++;
		}
	}
};

/** 
 * @brief Create the GeoReach structure to help with the queries
 * 
 * @param socialGraph 
 * @param locationGraph 
 * @param MAX_RMBR 
 * @param MAX_REACH_GRIDS 
 * @param LAYERS 
 */
void RangeReachVertex::createGVertex(Graph *socialGraph, LocationMap *locationGraph, float MAX_REACH_GRIDS = 3, int LAYERS = 4)
{
	cout << "Create G Vertex" << endl;
	vector<int> topologicalOrderedVertices;
	for (int curr_node : socialGraph->postOrder)
	{
		if (locationGraph->Map.count(curr_node))
		{
			if (locationGraph->Map[curr_node].isMbr)
			{
				for (int point_index = 4; point_index < locationGraph->Map[curr_node].spatialData.size() - 1; point_index += 2)
				{
					Location pos = Location(locationGraph->Map[curr_node].spatialData[point_index], locationGraph->Map[curr_node].spatialData[point_index + 1]);
					G_Vertex[curr_node].insert(getSpatialGridField(pos));
				}
			}
			else
			{
				Location pos = Location(locationGraph->Map[curr_node].spatialData[0], locationGraph->Map[curr_node].spatialData[1]);
				G_Vertex[curr_node].insert(getSpatialGridField(pos));
			}
		}
	}


	for (int curr_node : socialGraph->postOrder)
	{
		for (int reached_by : socialGraph->GraphSchemeReverse[curr_node])
		{
			if (G_Vertex.count(curr_node))
			{
				for (int g_vertex_id : G_Vertex[curr_node])
				{
					if (G_Vertex[reached_by].size() >= 1)
					{

						if (R_Vertex.count(reached_by))
						{
							R_Vertex[reached_by].insertMBR(this->spatialGrid[g_vertex_id]);
						}
						else
						{
							R_Vertex[reached_by] = this->spatialGrid[g_vertex_id];
						}
						if (R_Vertex.count(curr_node))
						{
							R_Vertex[reached_by].insertMBR(R_Vertex[curr_node]);
						}
					}
					else
					{
						G_Vertex[reached_by].insert(g_vertex_id);
					}
				}
			}
		}
	}

	for (int curr_node : socialGraph->postOrder)
	{
		if (G_Vertex.count(curr_node) == 0 && R_Vertex.count(curr_node) == 0)
		{
			B_Vertex[curr_node] = false;
		}
	}

	return;
}

void RangeReachVertex::checkVertexCorrectnes()
{
	unordered_set<int> NodesInSet;
	vector<int> NodesInGVertex;
	vector<int> NodesInRVertex;
	vector<int> NodesInBVertex;

	for (unordered_map<int, unordered_set<int>>::iterator g_iter = G_Vertex.begin(); g_iter != G_Vertex.end(); g_iter++)
	{
		NodesInSet.insert(g_iter->first);
		NodesInGVertex.push_back(g_iter->first);
	}

	for (unordered_map<int, MBR>::iterator r_iter = R_Vertex.begin(); r_iter != R_Vertex.end(); r_iter++)
	{
		NodesInSet.insert(r_iter->first);
		NodesInRVertex.push_back(r_iter->first);
	}

	for (unordered_map<int, bool>::iterator b_iter = B_Vertex.begin(); b_iter != B_Vertex.end(); b_iter++)
	{
		NodesInSet.insert(b_iter->first);
		NodesInBVertex.push_back(b_iter->first);
	}
	cout << "Total Unique Nodes: " << NodesInSet.size();
	cout << "\nB Nodes: " << NodesInBVertex.size() << "\nG Nodes:" << NodesInGVertex.size() << "\nR Nodes: " << NodesInRVertex.size() << endl;
	int totalNodesInVertices = NodesInBVertex.size() + NodesInGVertex.size() + NodesInRVertex.size();
	cout << NodesInSet.size() << "  " << totalNodesInVertices << endl;
	;
}

/**
 * @brief Performs a SpaReachQuery. 
 * In its core principle, its a breadth first search, in which branches are pruned by examining the B, G, R Vertex of the current node.
 * 
 * @param sourceNode 
 * @param queryWindow 
 * @param socialGraph 
 * @param spatialGraph 
 * @param layers 
 * @return true 
 * @return false 
 */
bool RangeReachVertex::SpaReachQuery(int sourceNode, box queryWindow, Graph *socialGraph, LocationMap *spatialGraph, int layers)
{

	MBR queryWindowToMBR(
		queryWindow.min_corner().get<0>(),
		queryWindow.min_corner().get<1>(),
		queryWindow.max_corner().get<0>(),
		queryWindow.max_corner().get<1>());

	int node = sourceNode;
	if (socialGraph->NodeBelongsToSCC.count(node) != 0)
	{
		node = socialGraph->NodeBelongsToSCC[node];
	}
	queue<int> Q;
	for (int reaches : socialGraph->GraphScheme[node]){
		Q.push(reaches);
	}
	while (!Q.empty())
	{
		int curr_node = Q.front();

		Q.pop();
		if (B_Vertex.count(curr_node) && !B_Vertex[curr_node])
		{
			continue;
		}

		if (spatialGraph->Map.count(curr_node))
		{
			for (coordinates coor = 0; coor < spatialGraph->Map[curr_node].spatialData.size(); coor = coor + 2)
			{
				Location pos = Location(spatialGraph->Map[curr_node].spatialData[coor], spatialGraph->Map[curr_node].spatialData[coor + 1]);
				if (queryWindowToMBR.containsPoint(pos))
				{
					return true;
				}
			}
		}

		if (R_Vertex.count(curr_node))
		{
			if (overlaps(R_Vertex[curr_node], queryWindow)){
				return true;
			}
			if (intersects(R_Vertex[curr_node], queryWindow))
			{
				for (int reachable_node : socialGraph->GraphScheme[curr_node])
				{
					Q.push(reachable_node);
				}
			}
		}
		if (G_Vertex.count(curr_node))
		{
			for (int gVertex : G_Vertex[curr_node])
			{
				MBR grid = this->spatialGrid[gVertex];
				if (intersects(grid, queryWindow))
				{
					for (int reachable_node : socialGraph->GraphScheme[curr_node])
					{
						Q.push(reachable_node);
					}
				}
			}
		}
	}
	return false;
}

/*
Returns the spatial gridfield this for the location given in the parameters
*/
int RangeReachVertex::getSpatialGridField(Location nodeLocation)
{
	bool spatialGridFound = false;
	int chunkIteratorX, chunkIteratorY;

	for (auto grid : this->spatialGrid)
	{
		if (grid.second.containsPoint(nodeLocation))
		{
			return grid.first;
		}
	}
	return 10000000;
}


void RangeReachVertex::writeAttributesToFile(string filename)
{
	ofstream bFile, gFile, rFile, gridFile;
	bFile.open("./data/spareach/" + filename + "_b_vertex");
	for (unordered_map<int, bool>::iterator iter = B_Vertex.begin(); iter != B_Vertex.end(); iter++)
	{
		bFile << iter->first << "\t" << iter->second << "\n";
	}
	bFile.close();

	gFile.open("./data/spareach/" + filename + "_g_vertex");
	for (unordered_map<int, unordered_set<int>>::iterator iter = G_Vertex.begin(); iter != G_Vertex.end(); iter++)
	{
		gFile << iter->first;
		for (int node : iter->second)
		{
			gFile << "\t" << node;
		}
		gFile << "\n";
	}
	gFile.close();

	rFile.open("./data/spareach/" + filename + "_r_vertex");
	for (unordered_map<int, MBR>::iterator iter = R_Vertex.begin(); iter != R_Vertex.end(); iter++)
	{
		rFile << iter->first << "\t" << iter->second.stringify() << '\n';
	}
	rFile.close();

	gridFile.open("./data/spareach/" + filename + "_grid");
	for (auto grid : this->spatialGrid)
	{
		gridFile << grid.first << "\t" << grid.second.stringify() << "\n";
	}
	gridFile.close();
}

void RangeReachVertex::readAttributesFromFile(string filename)
{
	cout << "Load attributes file" << endl;
	string bFilePath = "./data/spareach/" + filename + "_b_vertex";
	ifstream file;
	file.open(bFilePath);
	int node, bVertex;
	if (file.is_open())
	{
		while (file >> node >> bVertex)
		{
			this->B_Vertex[node] = bVertex == 1 ? true : false;
		}
	}
	else
	{
		cout << "B-Vertex File not found" << endl;
		return;
	}

	file.close();

	string gFilePath = "./data/spareach/" + filename + "_g_vertex";
	file.open(gFilePath);
	if (!file.is_open())
	{
		cout << "G-Vertex File not found" << endl;
		return;
	}
	string line;
	vector<int> gVertices;
	while (getline(file, line))
	{
		stringstream linestream(line);
		string data;
		while (getline(linestream, data, '\t'))
		{
			gVertices.push_back(stoi(data));
		}
		int node = gVertices[0];
		gVertices.erase(gVertices.begin());
		this->G_Vertex[node] = unordered_set<int>(gVertices.begin(), gVertices.end());
		gVertices.clear();
	}
	file.close();

	string rFilePath = "./data/spareach/" + filename + "_r_vertex";
	file.open(rFilePath);
		
	if (!file.is_open())
	{
		cout << "R-Vertex File not found" << endl;
		return;
	}

	coordinates xMin, xMax, yMin, yMax;
	while (file >> node >> xMin >> yMin >> xMax >> yMax)
	{
		this->R_Vertex[node] = MBR(xMin, yMin, xMax, yMax);
	}

	file.close();

	string gridfile = "./data/spareach/" + filename + "_grid";
	file.open(gridfile);
	if (!file.is_open())
	{
		cout << "Grid File not found" << endl;
		return;
	}
	else
	{
		int layer, gridId;
		coordinates xMin, xMax, yMin, yMax;
		while (file >> gridId >> xMin >> yMin >> xMax >> yMax)
		{
			this->spatialGrid[gridId] = MBR(xMin, yMin, xMax, yMax);
		}
	}

	file.close();
}

#pragma region Print Statements

void RangeReachVertex::printAllGridLayers()
{
	for (auto grid : this->spatialGrid)
	{
		cout << grid.first << " " << grid.second.stringify() << endl;
	}
}

void RangeReachVertex::printGridField()
{
	cout << "Spatial Grid Cell:" << endl;
	for (map<int, MBR>::iterator iter = spatialGrid.begin(); iter != spatialGrid.end(); iter++)
	{
		cout << iter->first << "\t" << iter->second.stringify() << "\n";
	}
}

void RangeReachVertex::printBVertex()
{
	cout << "B Vertex:\n";
	for (unordered_map<int, bool>::iterator it = B_Vertex.begin(); it != B_Vertex.end(); it++)
	{
		cout << "N: " << it->first << "\t" << it->second << endl;
	}
	cout << endl;
}

void RangeReachVertex::printRVertex()
{
	cout << "R Vertex: \n";
	for (unordered_map<int, MBR>::iterator it = R_Vertex.begin(); it != R_Vertex.end(); it++)
	{
		cout << it->first << " -> " << it->second.stringify() << endl;
	}
	cout << "\n";
}

void RangeReachVertex::printGVertex()
{
	cout << "G Vertex: " << endl;
	for (unordered_map<int, unordered_set<int>>::iterator iter = G_Vertex.begin(); iter != G_Vertex.end(); iter++)
	{
		cout << iter->first << " : ";
		for (int grid : iter->second)
		{
			cout << grid << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void printSpatialGridCells(map<int, MBR> spatialGrid)
{
	cout << "Print Statial Grids" << endl;
	for (auto i : spatialGrid)
	{
		cout << "SpatialGrid: " << i.first << " -> " << i.second.stringify();
	}
	cout << endl;
}

#pragma endregion
