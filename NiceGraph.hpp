/*******************************************************************************
*     This program is free software: you can redistribute it and/or modify     *
*     it under the terms of the GNU General Public License as published by     *
*     the Free Software Foundation, either version 3 of the License, or        *
*     (at your option) any later version.                                      *
*                                                                              *
*     This program is distributed in the hope that it will be useful,          *
*     but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*     GNU General Public License for more details.                             *
*                                                                              *
*     You should have received a copy of the GNU General Public License        *
*     along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
********************************************************************************/
/* 	FILE:  NiceGraph.hpp
*  	DESCRIPTION:  Header file for a class to manage graphs
*  	AUTHOR: Gary Weissman
*  	DATE: August 2008
*  	CONTACT: gary@babelgraph.org
*	URL: http://www.babelgraph.org/
*/



// Header file for a NiceGraph class
// NiceGraph is a friendly interface for graph manipulation and analysis
// filename: NiceGraph.hpp
//
// Author: Gary Weissman
// Email: gweissman@gmail.com
// June 2008

#ifndef NICEGRAPH_HPP_
#define NICEGRAPH_HPP_

#include<cstdlib>
#include<string>
#include<vector>
#include<cmath>
#include<map>
#include<iostream>
#include<fstream>
#include<climits>
#include<queue>
#include<algorithm>

using namespace std;

struct Vertex;
struct Edge;

struct Vertex {

	int vID, vColor;
	string vName;
	float posX, posY, posZ;
	map<int, Edge*> in, out;
} ;

struct Edge {
	Vertex *from, *to;
	int eID;
	float weight;
} ;

typedef map<int,Vertex*> vertexMap;
typedef map<int,Edge*> edgeMap;


class NiceGraph {

	public:
	// Graph initialization
	NiceGraph();
	~NiceGraph();

	// Graph create/manipulate structure
	void makeEmptyGraph(int numVertices);
	void makeCompleteGraph(int numVertices);
	void makeRandomGraph(int numVertices, float density);
	void makeKRegularGraph (int numVertices, int k);
	void makeStrangersBanquetGraph(int numVertices, int groups, float density, float mu);
	void makeBinaryTreeGraph(int numVertices);
	void addVertex();
	void addVertex(string newName);
	void removeVertex (int vertexID);
	void addEdge (int fromID, int toID);
	void removeEdge (int from, int to);
	void removeEdge (int edgeID);
	int getEdgeID(int vertexID1, int vertexID2);
	int getNumVertices();
	int getNumEdges();
	int getDegree (int vertexID);
	int getInDegree (int vertexID);
	int getOutDegree (int vertexID);
	float getWeight(int from, int to);
	bool testConnected (int vertexID1, int vertexID2);
	bool testFromToConnected(int vertexID1, int vertexID2);
	bool checkDirected();
	void makeUndirected();
	bool validVID(int ID);
	bool validEID(int ID);
	void getEdgeIndexList(vector<int> & list);
	void getVertexIndexList(vector<int> & list);

	// Neighborhoods and such...
	// get neighborlist, etc.
	vector<int> getNeighborList(int vertexID);
	vector<int> getInNeighborList(int vertexID);
	vector<int> getOutNeighborList(int vertexID);
	
	// Graph Extrinsic properties
	string getVertexName(int vertexID);
	void setVertexName(int vertexID, string newName);
	int getVertexColor (int vertexID);
	void setVertexColor (int vertexID, int newColor);

	// Graph Analysis
	void avgShortestPathMatrix(map<int,float> &paths );
	float avgShortestPathofGraph_FW();	// better to use FW...
	float avgShortestPathofGraph_BFS(); // works but is SLOW!! WHY??
	float getHomophilicDyadDensity();
	void getPageRank(map<int,float> &pageRank, int iterations = 100, float damping = 0.85);
	float getClusteringCoefficient(int index);
	void getClosenessCentrality(map<int,float> &cCentrality);

	// Graph plotting things
	void setXYZPos(int vertexID, float x, float y, float z);
	void getXYZPos(int vertexID, vector<float> & xyz); 	// takes a reference to 3-vector and returns it full of coordinates
	void getEndpoints (int edgeID, vector<float> & v1, vector<float> &v2); // return endpoints of an edge
	
	// Graph Layout Algorithms
	void randomLayoutAll(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	void randomLayout(int vertexID, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	void selfOrganize(float force, float min, float max);
	void sphereLayout(float radius);
	void circleLayout(float radius);
	void spiralLayout(float radius);
	void fruchtermanReingoldLayout(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	void layersByGroupLayout(float radius, float nodeSize);

	// file operations
	void loadFromFile(string filename);
	void saveToFile(string filename);

	// Other stuff
	enum COLORS {YELLOW=0, PURPLE, RED, ORANGE, WHITE, BLACK, GREEN, BLUE};
	
private:
	int edgeIDCounter, vertexIDCounter;
	vertexMap vertexList;
	edgeMap edgeList;
	bool isDirected;
	void tokenize (const string &str, vector<string> &tokens, const string &delimiters);

};


#endif // NICEGRAPH_HPP_
