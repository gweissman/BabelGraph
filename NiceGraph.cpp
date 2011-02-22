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
/* 	FILE: NiceGraph.cpp
*  	DESCRIPTION: Implementation of a class to manage the graphs
*  	AUTHOR: Gary Weissman
*  	DATE: August 2008
*  	CONTACT: gary@babelgraph.org
*	URL: http://www.babelgraph.org/
*/

#include "NiceGraph.hpp"

// Graph initialization
//------------------------------------------------------------------------------
NiceGraph::NiceGraph()
{
	edgeIDCounter = 0;
	vertexIDCounter = 0;
	isDirected = true; // the default is a directed graph
				// call makeUndirected() to change this
}
//------------------------------------------------------------------------------
NiceGraph::~NiceGraph()
{
	// destroy vertices
	for (map<int,Vertex*>::iterator iter = vertexList.begin(); iter != vertexList.end(); iter++)
	{
		if (iter->second != NULL) delete iter->second;
	}

	// destroy edges
	for (map<int,Edge*>::iterator iter = edgeList.begin(); iter != edgeList.end(); iter++)
	{
		if (iter->second != NULL) delete iter->second;
	}
}
// Graph create/manipulate structure
//------------------------------------------------------------------------------
void NiceGraph::makeEmptyGraph(int numVertices)
{
	for (int index = 0; index < numVertices; index++)
	{
		addVertex();
	}
}
//------------------------------------------------------------------------------
void NiceGraph::makeCompleteGraph(int numVertices)
{
	makeEmptyGraph(numVertices); 	// first initialize all the vertices
	
	// now loop through all possible edges and create them
	for (int froms = 0; froms < numVertices; froms++)
	{
		for (int tos = 0; tos < numVertices; tos++)
		{
			if ((froms != tos) && (froms < tos))
			{
				addEdge(froms,tos);
				addEdge(tos,froms);
			}
		}
	}
}
//------------------------------------------------------------------------------
void NiceGraph::makeRandomGraph(int numVertices, float density)
{

	makeEmptyGraph(numVertices); 	// first initialize all the vertices
	
	int total = density * (float) numVertices * (numVertices - 1.0);

	
	
	// bounds checking
	if ((density > 0) && (total < 1))
		total = 1;

	while (total > 0)
	{
		int v1 =   rand() % numVertices;
		int v2 =   rand() % numVertices;

		if (isDirected)		// directed graph
		{
			if (!testFromToConnected(v1,v2))
			{
				addEdge(v1,v2);
				total--;
			}
		}
		else			// undirected graph
		{
			if (!testConnected(v1,v2))
			{
				addEdge(v1,v2);
				addEdge(v2,v1);
				total--; total--;
			}
		}
	}
}
//------------------------------------------------------------------------------
void NiceGraph::makeKRegularGraph (int numVertices, int k)
{
	makeEmptyGraph(numVertices); 	// first initialize all the vertices

	int maxIndex = numVertices - 1;
		
	if (k % 2 != 0)		// k is an odd number, can't have an odd k-2 regular lattice graph
		k++;		// just round k up one	

	// special case k = 1?

	// now add k-neighbors to each one
	for (int i = 0; i < numVertices; i++)
	{
		for (int r = 1; r <= k/2; r++)
		{
			int next = i + r;
			if ( next > maxIndex)
				next = next - maxIndex - 1;
			addEdge (i, next);
			addEdge (next , i);
		}
	}
}
//------------------------------------------------------------------------------
void NiceGraph::makeStrangersBanquetGraph(int numVertices, int groups, float density, float mu)
{

	makeEmptyGraph(numVertices); 	// first initialize all the vertices

	makeUndirected(); // the SB is only undirected for now...

	if (groups > numVertices)		// bounds checking on number of groups
		groups = numVertices;
	
	for (int c = 0; c < numVertices; c++)	// now subdivide into groups
	{
		for (int g = 0; g < groups; g++)
		{
			if (c % groups == g)
				setVertexColor(c,g);
		}
	}

	// make declining edge scheme to speed things up
	int total_edges = numVertices * (numVertices - 1) / 2;
	vector<Edge> proposals (total_edges);
	
	int edgeIndex = 0;
	for (int froms = 0; froms < numVertices; froms++)
	{
		for (int tos = 0; tos < numVertices; tos++)
		{
			if ((froms != tos) && (froms < tos))
			{
				proposals[edgeIndex].from = vertexList[froms];
				proposals[edgeIndex].to = vertexList[tos];
				edgeIndex++;
			}
		}
	}
	// propose connections and build graph
	int num_edges = (int) (density * (float) total_edges);
	int edge_counter = 0;

	    while (edge_counter < num_edges)
	    {
		// use declining proposal list to speed things up...
		int randIndex = rand() % proposals.size();
		
	    	// choose two vertices to make the proposal
	    	int vertex1 = proposals[randIndex].from->vID;
	    	int vertex2 = proposals[randIndex].to->vID;
	    	
	    	if ((vertex1 != vertex2) && (!testConnected(vertex1,vertex2))) 			// if isn't itself, and aren't already connected
    		{
				// now check to see if they are accepted
				if (vertexList[vertex1]->vColor == vertexList[vertex2]->vColor) 
				{
					// if same color approve match
					addEdge(vertex1, vertex2);		// add edge
					edge_counter++;
					if (!proposals.empty()) 
						proposals.erase(proposals.begin()+randIndex);	// remove from list

				}
				else		// if different colors check against mu
				{
					float chance1 = rand() / (RAND_MAX+1.0);
					float chance2 = rand() / (RAND_MAX+1.0);
	
					if ((chance1 < mu) || (chance2 < mu))
							continue; // if either one rejects the connection do nothing
					else	
					{	// approve the connection
						addEdge(vertex1, vertex2);		// add edge
						edge_counter++;
						if (!proposals.empty()) 
							proposals.erase(proposals.begin()+randIndex);	// remove from list

					}	
				}		
		}
	}
}
//------------------------------------------------------------------------------
void NiceGraph::makeBinaryTreeGraph(int numVertices)
{
	makeEmptyGraph(numVertices); 	// first initialize all the vertices

	queue<int> parents;
	
	for (int q = 0; q < numVertices;  q++)	// initialize queue with vertex indices
	{
		parents.push(q);
	}

	while (!parents.empty())
	{
		int baby1 = 2*parents.front()+1 ;
		int baby2 = 2*parents.front()+2 ;

		if (baby1 < numVertices)
			addEdge(parents.front(), baby1);
		if (baby2 < numVertices)
			addEdge(parents.front(), baby2);
		parents.pop();
	}
}
//------------------------------------------------------------------------------
void NiceGraph::addVertex()
{
	vertexList[vertexIDCounter] = new Vertex();
	vertexList[vertexIDCounter]->vName = " ";
	vertexList[vertexIDCounter]->vColor = YELLOW;
	vertexList[vertexIDCounter]->vID = vertexIDCounter;
	vertexIDCounter++;
}
//------------------------------------------------------------------------------
void NiceGraph::addVertex(string newName)
{
	addVertex();
	vertexList[vertexIDCounter - 1]->vName = newName;
}
//------------------------------------------------------------------------------
void NiceGraph::removeVertex (int vertexID)
{
	// first make sure the vertex exists

	// then remove all neighboring edges (including those stored by the neighbors)

	// then remove the vertex itself
	// delete the pointer to the vertex for memory then delete the vertex from the map
	vertexList.erase(vertexID);

	// update everyone else's edgeIDs for no confusioN!!
}
//------------------------------------------------------------------------------
void NiceGraph::addEdge (int fromID, int toID)
{
	// if not already connected, add the edge
	// otherwise do nothing
	// also, can't connect to self

	bool goFlag;
	if (isDirected)	// directed graph
		goFlag = !testFromToConnected(fromID,toID);
	else 		// undirected graph
		goFlag = !testConnected(fromID,toID);

	if (fromID == toID)
		goFlag = false;
	
	if (goFlag)
	{
		Vertex *vFrom = vertexList[fromID];
		Vertex *vTo = vertexList[toID];

		Edge *newEdge = new Edge();

		edgeList[edgeIDCounter] = newEdge;
		edgeList[edgeIDCounter]->from = vFrom;
		edgeList[edgeIDCounter]->to = vTo;
		edgeList[edgeIDCounter]->eID = edgeIDCounter;

		// DEFAULT WEIGHT UNLESS OTHERWISE SPECIFIED
		edgeList[edgeIDCounter]->weight = 1.0;
		
		// also add the edge to the vertex neighbor list
		vFrom->out[edgeIDCounter] = newEdge;
		vTo->in[edgeIDCounter] = newEdge;
		
		if (!isDirected)	// add the reciprocal edge if the graph is not directed
			addEdge(toID,fromID);
	
		edgeIDCounter++;
	}
}
//------------------------------------------------------------------------------
void NiceGraph::removeEdge (int from, int to)
{	
	bool okflag;
	if (isDirected)
		okflag = testFromToConnected(from,to);
	else
		okflag = testConnected(from,to);

	// test connected first to make sure the connection exists
	if (okflag)
	{
		int edgeID = getEdgeID(from,to);

		removeEdge(edgeID);

		if (!isDirected)
			removeEdge(to,from);
	}
}
//------------------------------------------------------------------------------
void NiceGraph::removeEdge (int edgeID)
{

	// does this edge exist?
	bool okflag = validEID(edgeID);

	if (okflag)
	{
		Vertex * vfrom, *vto;

		vfrom = edgeList[edgeID]->from;
		vto = edgeList[edgeID]->to;
		
		// remove edge from neighbors vectors of both from and to
		vfrom->out.erase(edgeID);
		vto->in.erase(edgeID);
		if (!isDirected)
		{
			vfrom->in.erase(edgeID);
			vto->out.erase(edgeID);	
		}

		// remove edge from the MAP edgeList
		edgeList.erase(edgeID);
	}

	// also what about the memory leak aspect of the edge, does that get erased, too?


}
//------------------------------------------------------------------------------
int NiceGraph::getEdgeID(int vertexID1, int vertexID2)
{

	// NOTE: for undirected graphs there will be two distint EDGE IDs representing
	// basically the same thing.

	int edgeID = -1;

	// make sure it even exists, return -1 as an error code if we can't find it
	if (!testFromToConnected(vertexID1, vertexID2) && isDirected)
		return -1;
	if (!testFromToConnected(vertexID1, vertexID2) && isDirected)
		return -1;

	// now find the associated edgeid

	Vertex *vFrom = vertexList[vertexID1];
	
	map<int,Edge*> *temp = &vFrom->out;

	for (map<int,Edge*>::iterator iter = temp->begin(); iter != temp->end(); iter++)
	{
		Edge* eTemp = iter->second;

		if (eTemp->to->vID == vertexID2)
			edgeID = iter->first;
	}
	
	return edgeID;	// return edge id
}
//------------------------------------------------------------------------------
int NiceGraph::getNumVertices()
{
	return vertexList.size();
}
//------------------------------------------------------------------------------
int NiceGraph::getNumEdges()
{
	return edgeList.size();
}
//------------------------------------------------------------------------------
int NiceGraph::getDegree (int vertexID) 
{
	return getInDegree(vertexID) + getOutDegree(vertexID);
}
//------------------------------------------------------------------------------
int NiceGraph::getInDegree (int vertexID) 
{
	return vertexList[vertexID]->in.size();
}
//------------------------------------------------------------------------------
int NiceGraph::getOutDegree (int vertexID) 
{
	return vertexList[vertexID]->out.size();
}
//------------------------------------------------------------------------------
float NiceGraph::getWeight(int from, int to)
{
	int eID = getEdgeID(from,to);

	return edgeList[eID]->weight;
}
//------------------------------------------------------------------------------
bool NiceGraph::testConnected (int vertexID1, int vertexID2)
{
	// if it's an undirected graph, only need to check on vertex's neighborhood

	vector<int> temp = getNeighborList(vertexID1);

	if (temp.empty())	
		return false;	// if vertex doesn't have any neighors	
	else 
	{
		vector<int>::iterator it;
		it = find(temp.begin(), temp.end(), vertexID2);
		if (it != temp.end())  
			return true; 		// found the neighbor!
	}
	
	return false;		// default, didn't find anything
}
//------------------------------------------------------------------------------
bool NiceGraph::testFromToConnected (int vertexID1, int vertexID2)
{

	// on a directed graph, only check the outgoing neighbors

	vector<int> temp = getOutNeighborList(vertexID1);

	if (temp.empty())	
		return false;	// if vertex doesn't have any neighors	
	else 
	{
		vector<int>::iterator it;
		it = find(temp.begin(), temp.end(), vertexID2);
		if (it != temp.end())  
			return true; 		// found the neighbor!						
	}
	
	return false;		// default, didn't find anything


}
//------------------------------------------------------------------------------
bool NiceGraph::checkDirected()
{
	return isDirected;
}
//------------------------------------------------------------------------------
void NiceGraph::makeUndirected()
{
	isDirected = false;

	// now make matrix symmetric
	for (map<int,Edge*>::iterator iter = edgeList.begin(); iter != edgeList.end(); iter++)
	{
		Edge *e = iter->second;
		int id1 = e->from->vID;
		int id2 = e->to->vID;
		
		// error checking to make sure they don't already exist happens automatically
		addEdge(id2,id1);
	}
}
// Neighbor(hood) related functions
//------------------------------------------------------------------------------
vector<int> NiceGraph::getNeighborList(int vertexID)
{

	vector<int> total, in, out;

	in = getInNeighborList(vertexID);
	out = getOutNeighborList(vertexID);

	total = in; // added in, now add outs, but check for duplicates

	// check for duplicates
	for (vector<int>::iterator I = out.begin(); I != out.end(); I++)
	{
		vector<int>::iterator it;
		it = find(total.begin(), total.end(), *I);
		if (it == total.end()) 
			total.push_back(*I); 		// found the neighbor in ANOTHER SPOT! remove it!
	}

	return total;
}
//---------------------------------------------------------------------------------
vector<int> NiceGraph::getInNeighborList(int vertexID)
{
	map<int,Edge*> inList = vertexList[vertexID]->in;
	vector<int> tempList;
	
	for (map<int,Edge*>::iterator iter = inList.begin(); iter != inList.end(); iter++)
	{
		Edge *tEdge = iter->second;

		tempList.push_back(tEdge->from->vID);
	}
	return tempList;
}
//---------------------------------------------------------------------------------
vector<int> NiceGraph::getOutNeighborList(int vertexID)
{
	map<int,Edge*> outList = vertexList[vertexID]->out;

	vector<int> tempList;
	
	for (map<int,Edge*>::iterator iter = outList.begin(); iter != outList.end(); iter++)
	{
		Edge*tEdge = iter->second;
		
		tempList.push_back(tEdge->to->vID);
	}
	return tempList;
}
// Graph Extrinsic properties
//---------------------------------------------------------------------------------
string NiceGraph::getVertexName(int vertexID)
{
	return vertexList[vertexID]->vName;
}
//---------------------------------------------------------------------------------
void NiceGraph::setVertexName(int vertexID, string newName)
{
	vertexList[vertexID]->vName = newName;
}
//---------------------------------------------------------------------------------
int NiceGraph::getVertexColor (int vertexID)
{
	return vertexList[vertexID]->vColor;
}
//---------------------------------------------------------------------------------
void NiceGraph::setVertexColor (int vertexID, int newColor)
{
	vertexList[vertexID]->vColor = newColor;
}

// Graph Analysis
//---------------------------------------------------------------------------------
void NiceGraph::avgShortestPathMatrix(map<int,float> &paths )
{ 
	// this approach based on Floyd-Warshall shortest paths algorithm (iterative!)
	// returns -1 if the graph if some nodes are not reachable

	int size = vertexList.size();

	unsigned long int  distance[size][size];
	
	// initialize all values
	for (int q = 0; q < size; q++)
	{
		for (int r = 0; r < size; r++)
		{
			if (testFromToConnected(q,r))
				distance[q][r] = getWeight(q,r);
			else if (q == r)
				distance[q][r] = 0;		// zero distance to self
			else
				distance[q][r] = INT_MAX; // set distance to infinity if not connected directly
		}
	} 

	// now iterate over and over to update shortest paths...
	for (int k = 0; k < size; k++){
		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++){
				unsigned int sum = distance[i][k] + distance[k][j];
				if (sum < distance[i][j]){
					distance[i][j] = sum; }
			}
		}
	}


	// now compute averages for each guy distance matrix
	

	for (int m = 0; m < size; m++)	{
		long double sum = 0;
		for (int n = 0; n < size; n++) {
			sum += distance[m][n];
		}
		sum = sum / (size-1);
		if (sum >=getNumVertices())
				sum = -1;
			paths[m] = sum;
	}
}
//---------------------------------------------------------------------------------
float NiceGraph::avgShortestPathofGraph_FW()
{

	// this approach based on Floyd-Warshall shortest paths algorithm (iterative!)
	// returns -1 if the graph if some nodes are not reachable

	int size = vertexList.size();

	unsigned long int  distance[size][size];
	
	// initialize all values
	for (int q = 0; q < size; q++)
	{
		for (int r = 0; r < size; r++)
		{
			if (testFromToConnected(q,r))
				distance[q][r] = getWeight(q,r);
			else if (q == r)
				distance[q][r] = 0;		// zero distance to self
			else
				distance[q][r] = INT_MAX; // set distance to infinity if not connected directly
		}
	} 

	// now iterate over and over to update shortest paths...
	for (int k = 0; k < size; k++){
		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++){
				unsigned int sum = distance[i][k] + distance[k][j];
				if (sum < distance[i][j]){
					distance[i][j] = sum; }
			}
		}
	}


	// now compute averages using distance matrix
	long double sum = 0;

	for (int m = 0; m < size; m++)	{

		for (int n = 0; n < size; n++) {
			sum += distance[m][n];


		}
	}
	float value = (sum / (size * (size - 1)));
	if (value >= getNumVertices())			// the case where the graph is disconnected
		value=-1;
	return value ;		// return the average over all values not including distance to self
}
//---------------------------------------------------------------------------------
// works but is SLOW!! WHY??
float NiceGraph::avgShortestPathofGraph_BFS()
{
	// This approach based on a breadth first search --> better for undirected graphs?
	int size = vertexList.size();

	unsigned long int  distance[size][size];

	// initialize all values
	for (int q = 0; q < size; q++)
	{
		for (int r = 0; r < size; r++)
		{
				distance[q][r] = INT_MAX; // set distance to infinity if not connected directly
		}
	} 

	for (int q =0; q < size; q++)
	{
		vector<bool> visited (size, false); // initialize all visited values to true so far
		visited[q] = true; 	// has already visited self
		distance[q][q] = 0; // distance to self is always zero
		queue<int> Q;
		Q.push(q);
		
		do {		
			visited[Q.front()] = true;
			vector<int> temp = getOutNeighborList(Q.front());
			if (!temp.empty())
			{
				for (unsigned int t = 0; t < temp.size(); t++)
				{
					distance[Q.front()][temp[t]] = getWeight(Q.front(),temp[t]);
					if (!visited[temp[t]])
						Q.push(temp[t]);
					unsigned int newPath = distance[q][Q.front()] + distance[Q.front()][temp[t]];
					if (newPath < distance[q][temp[t]])	{
						distance[q][temp[t]] = newPath;
						}
					
				}
			} 

			Q.pop();	// pop off the neighbor just examined

		}  while (!Q.empty());
	}

	// now compute averages using distance matrix
	long double sum = 0;

	for (int m = 0; m < size; m++)	{
		for (int n = 0; n < size; n++) {
			sum += distance[m][n];
		}
	}

	float value = (sum / (size * (size - 1)));
	if (value >= getNumVertices())			// the case where the graph is disconnected
		value=-1;
	return value ;		// return the average over all values not including distance to self
	
}
//---------------------------------------------------------------------------------
float NiceGraph::getHomophilicDyadDensity()
{
	// define HDD = s_i / (s_i + d_i), i.e. the fraction of homophilic edges over all edges


	int same = 0, different = 0;

	
	for (map<int,Edge*>::iterator iter = edgeList.begin(); iter != edgeList.end(); iter++)
	{
		Edge* eTemp;
		eTemp = iter->second;


		if (eTemp->from->vColor == eTemp->to->vColor)
			same++;
		else 
			different++;
	}

	float HDD = (float) same / ((float) same + (float) different);
			
	return HDD;
}
//------------------------------------------------------------------------------
void NiceGraph::getPageRank(map<int,float> &pageRank, int iterations, float damping)
{
	int total = getNumVertices();

	// set initial page ranks
	for (map<int,Vertex*>::iterator vIter = vertexList.begin(); vIter != vertexList.end(); vIter++)
	{
		pageRank[vIter->first] = 1.0/(float)total;
	}

	// run arbitrary number of iterations
	for (int i = 0; i < iterations; i++)
	{
		for (map<int,Vertex*>::iterator vIter = vertexList.begin(); vIter != vertexList.end(); vIter++)
		{	
			float votes = 0;
			int id = vIter->first;
			
			vector<int> inList = getInNeighborList(id);

			for (vector<int>::iterator nIter = inList.begin(); nIter != inList.end(); nIter++)
			{
				votes+=pageRank[(*nIter)]/(float)getOutDegree(*nIter);
			}

			pageRank[id] = (1.0-damping) + damping * votes;
		}
	}
}
//------------------------------------------------------------------------------
float NiceGraph::getClusteringCoefficient(int index)
{

	vector<int> ns = getNeighborList(index);

	int size = ns.size();

	int max = size * (size - 1) ;
	
	int cnxs = 0;

	float cc = 0;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int from = ns[i];
			int to = ns[j];
			if (from!=to)	// don't connect to self
			{
				if (testFromToConnected(from,to)) cnxs++;
			}
		}
	}
	if (max<1)
		cc = 0;
	else
		cc = (float)cnxs / (float)max;

	return cc;
}
//------------------------------------------------------------------------------
void NiceGraph::getClosenessCentrality(map<int,float> &cCentrality)
{
	// this approach based on Floyd-Warshall shortest paths algorithm (iterative!)
	// almost the same as avg shortest paths, but only calculates against reachable vertices
	// therefore always returns a normal value even if the graph is partially disconnected

	unsigned int size = vertexList.size();

	unsigned long int  distance[size][size];
	
	// initialize all values
	for (unsigned int q = 0; q < size; q++)
	{
		for (unsigned int r = 0; r < size; r++)
		{
			if (testFromToConnected(q,r))
				distance[q][r] = getWeight(q,r);
			else if (q == r)
				distance[q][r] = 0;		// zero distance to self
			else
				distance[q][r] = INT_MAX; // set distance to infinity if not connected directly
		}
	} 

	// now iterate over and over to update shortest paths...
	for (unsigned int k = 0; k < size; k++){
		for (unsigned int i = 0; i < size; i++){
			for (unsigned int j = 0; j < size; j++){
				unsigned int sum = distance[i][k] + distance[k][j];
				if (sum < distance[i][j]){
					distance[i][j] = sum; }
			}
		}
	}


	// now compute averages for each guy distance matrix
	
	for (unsigned int m = 0; m < size; m++)	{
		long double sum = 0;
		int reachable = 0;
		for (unsigned int n = 0; n < size; n++) {
			if (distance[m][n] < size) // ie. if the vertex is reachable
			{
			sum += distance[m][n];	reachable++;}
		}
		if (!(reachable == 1)) // ie can only get to itself
			sum = sum / (reachable-1);
		else 	sum = 0;
		if (sum >=getNumVertices())		// should never happen here
				sum = -1;
			cCentrality[m] = sum;
	}
}
//------------------------------------------------------------------------------
void NiceGraph::setXYZPos(int vertexID, float x, float y, float z)
{
	vertexList[vertexID]->posX = x;
	vertexList[vertexID]->posY = y;
	vertexList[vertexID]->posZ = z;
}
//------------------------------------------------------------------------------
// takes a reference to 3-vector and returns it full of coordinates
void NiceGraph::getXYZPos(int vertexID, vector<float> & xyz) 	
{
	xyz[0]= vertexList[vertexID]->posX;
	xyz[1]= vertexList[vertexID]->posY;
	xyz[2]= vertexList[vertexID]->posZ;
}
//---------------------------------------------------------------------------------
// return endpoints of an edge
void NiceGraph::getEndpoints (int edgeID, vector<float> & v1, vector<float> &v2) 
{
	if (validEID(edgeID))
	{
		Vertex *vert1 = edgeList[edgeID]->from;
		Vertex *vert2 = edgeList[edgeID]->to;

		v1[0] = vert1->posX;
		v1[1] = vert1->posY;
		v1[2] = vert1->posZ;

		v2[0] = vert2->posX;
		v2[1] = vert2->posY;
		v2[2] = vert2->posZ;
	}
}
//---------------------------------------------------------------------------------
void NiceGraph::randomLayoutAll(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
	for (unsigned int vIndex = 0; vIndex < vertexList.size(); vIndex++)
	{
		randomLayout (vIndex, xmin, xmax, ymin, ymax, zmin, zmax);
	}
}
//---------------------------------------------------------------------------------
void NiceGraph::randomLayout(int vertexID, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
	float randX = (rand() / (RAND_MAX+1.0)) * (xmax - xmin) + xmin;
	float randY = (rand() / (RAND_MAX+1.0)) * (ymax - ymin) + ymin;
	float randZ = (rand() / (RAND_MAX+1.0)) * (zmax - zmin) + zmin;
	
	setXYZPos(vertexID, randX, randY, randZ);
}
//---------------------------------------------------------------------------------
void NiceGraph::selfOrganize(float force, float min, float max)
{
	// this function only self organizes 1 step, must be called multiple times for continous updating
	// the force value ranges from [0,1] and is the fraction of the final step to take
	// min and max values are protected distances for the various edges

	if (force > 1)
		force = 1;
	else if (force <=0)
		force = 0.001;

		// iterate over edges, attract and check bounds, rearrange as necessary

	for (map<int,Edge*>::iterator iter = edgeList.begin(); iter != edgeList.end(); iter++)
	{
		int e = iter->first;
		vector<float> v1 (3), v2 (3);
		float attraction = 0, dx = 0, dy = 0, dz = 0;
		int id1 = edgeList[e]->from->vID;
		int id2 = edgeList[e]->to->vID;
		getEndpoints (e, v1, v2);

		float vx = v2[0] - v1[0];
		float vy = v2[1] - v1[1];
		float vz = v2[2] - v1[2];
		
		float dr = sqrt ( vx*vx + vy*vy + vz*vz );

		attraction = -1.0 * (max - dr) / (100 * (max - min)/2);

		dx += 0.5 * attraction * vx;
		dy += 0.5 * attraction * vy;
		dz += 0.5 * attraction * vz;

		float scale = 1.0;

		setXYZPos (id1, v1[0] + scale * dx, v1[1] + scale * dy, v1[2] + scale * dz);		
		setXYZPos (id2, v2[0] - scale * dx, v2[1] - scale * dy, v2[2] - scale * dz);

	}

	for (map<int,Vertex*>::iterator iter = vertexList.begin(); iter != vertexList.end(); iter++)
	{
		int self = iter->first;

		float dx = 0, dy = 0, dz = 0, repulsion = 0;
		float px = vertexList[self]->posX;
		float py = vertexList[self]->posY;
		float pz = vertexList[self]->posZ;

		// sum repulsion from other vertices
		for (map<int,Vertex*>::iterator iter2 = vertexList.begin(); iter2 != vertexList.end(); iter2++)
		{
			int other = iter2->first;
			if (self == other)		// don't need to repel self, too!
				continue;
			
			float vx = vertexList[other]->posX - px;
			float vy = vertexList[other]->posY - py;
			float vz = vertexList[other]->posZ - pz;

			float dr = sqrt (vx * vx + vy * vy + vz * vz);

			// let the repulsion go as 1/r^2, like the electrostatic force

			repulsion = -0.01 * force*force / (sqrt(dr) * dr);
			
			dx += repulsion * vx;
			dy += repulsion * vy;
			dz += repulsion * vz;
		}

		// update position
			setXYZPos (self, px + dx, py + dy, pz + dz);
	}
}
//------------------------------------------------------------------------------
void NiceGraph::sphereLayout(float radius)
{
	int num = getNumVertices();
	
	float x,y,z, theta = 0, phi = 0;
	float d_theta = 3.14159265 / sqrt (num);
	float d_phi = 2.0 * 3.14159265 / sqrt (num);

	int theta_total = ceil( sqrt (num));
	int phi_total =  ceil( sqrt (num));

	map<int,Vertex*>::iterator iter = vertexList.begin();

	for (int theta_steps = 0; theta_steps < theta_total; theta_steps++)
	{
		for (int phi_steps = 0; phi_steps < phi_total; phi_steps++)
		{
			if (iter != vertexList.end())
			{
				int n = iter->first;
				
				phi += d_phi;
	
				x = radius * sin (theta) * cos (phi);
				y = radius * sin (theta) * sin (phi);
				z = radius * cos (theta);
				setXYZPos(n,x,y,z);
				iter++;
			}
		}
		theta += d_theta;
	}
}
//------------------------------------------------------------------------------
void NiceGraph::circleLayout(float radius)
{
	float x, y, angle;
	float interval = 2.0 * 3.14159265 / getNumVertices(); 

	for (map<int,Vertex*>::iterator iter = vertexList.begin(); iter != vertexList.end(); iter++)
	{
		int index = iter->first;
		angle = interval * index;
		x = radius * cos (angle);
		y = radius * sin (angle);	
		setXYZPos(index,x,y,0);
	}
}
//------------------------------------------------------------------------------
void NiceGraph::spiralLayout(float radius)
{
	float x, y, angle;
	int wraps = 2;
	float interval = wraps * 2.0 * 3.14159265 / getNumVertices(); 
	float radIncrement = radius / (1.1 * getNumVertices());
	float zIncrement = 2.0 * radius / getNumVertices();

	for (map<int,Vertex*>::iterator iter = vertexList.begin(); iter != vertexList.end(); iter++)
	{
		int index = iter->first;
		angle = interval * index;
		x = index * radIncrement * cos (angle);
		y = index * radIncrement * sin (angle);	
		setXYZPos(index,x,y,-1* radius + index * zIncrement);
	}
}
//------------------------------------------------------------------------------
void NiceGraph::layersByGroupLayout(float radius, float nodeSize)
{	
	float count[5] = {0,0,0,0,0}; 	// only handles up to five groups
	float last[5];
	float xStep = radius / (2.0 * nodeSize * getNumVertices());
	float yStep = 2 * xStep;

	// first count how many are in each group for each one
	for (map<int,Vertex*>::iterator iter = vertexList.begin(); iter != vertexList.end(); iter++)
	{
		int group = getVertexColor(iter->first);
		switch (group)
		{
			case 0:count[0]++;break;
			case 1:count[1]++;break;
			case 2:count[2]++;break;
			case 3:count[3]++;break;
			case 4:count[4]++;break;
		}
	}

	// now set starting points
	for (int i = 0; i < 5; i++)
	{
		last[i] = -1.0 * xStep * count[i] / 2.0;	
	}

	// now set the plotting points
	for (map<int,Vertex*>::iterator iter = vertexList.begin(); iter != vertexList.end(); iter++)
	{
		// set position for this vertex
		int v = iter->first;
		int group = getVertexColor(v);
		float x = last[group];
		float y = group * yStep;
		float z = 0;
		setXYZPos (v,x,y,z);

		// now update coordinates for next iteration
		last[group]+=xStep;
	}
}
//------------------------------------------------------------------------------
/*adapted from the 2D model in paper SOFTWAREâ PRACTICE AND EXPERIENCE, 
VOL. 21(1 1), 1129-1164 (NOVEMBER 1991),Fruchtermand and Reingold, 
"Graph Drawing by Force-directed Placement"*/
// NB STILL GETS CAUGHT IN LOCAL MINIMA AND NODES OVERLAP!
void NiceGraph::fruchtermanReingoldLayout(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
	float volume = (xmax - xmin) * (ymax - ymin) * (zmax - zmin);
	float k = 1000 * pow( ( volume / getNumVertices()),1.0f/3.0f );
	if ((zmin == 0) && (zmax ==0))
		k = pow (k,3.0f/2.0f); // for a two dimensional constant

	int iterations = 100;	// this value is arbitrary


	// use FR "quadrant variation" on 1/5 largest axes in zone
	// ie only calculate repulsion within the zone
	float zone = max(xmax - xmin, ymax - ymin);
	zone = max(zone, zmax - zmin);
	zone = zone;
	float t = zone;		// arbitrary initial temperature
	
	// do some arbitrary number of iterations
	for (int i = 0; i < iterations; i++)
	{
		map<int,float> dx,dy,dz; // store displacements by vertex ID until the end
		
		//calculate repulsive forces
		for (map<int,Vertex*>::iterator pU = vertexList.begin(); pU != vertexList.end(); pU++)
		{	// initialize values
			int u_id = pU->first;
			vector<float> u_pos(3);
			getXYZPos (u_id, u_pos);
			for (map<int,Vertex*>::iterator pV = vertexList.begin(); pV != vertexList.end(); pV++)
			{
				vector<float> v_pos(3);
				getXYZPos(pV->first, v_pos);
				float diffx =	v_pos[0]-u_pos[0];
				float diffy =   v_pos[1]-u_pos[1];
				float diffz =   v_pos[2]-u_pos[2];
				float dr = sqrt (diffx*diffx+diffy*diffy+diffz*diffz);		
				if (dr < zone)	// only do the calculations if U is within the zone of V
				{
					float force_r = 0;
					float scale = 0;
					if (dr == 0)
					{
						force_r = -100;
						scale = 1;
					}
					else
					{
						force_r = -1.0*(k * k)/dr; 
						scale = dr;
					}
					dx[u_id] += force_r * diffx/scale ;
					dy[u_id] += force_r * diffy/scale ;
					dz[u_id] += force_r * diffz/scale ;
				}
			}
		}
		
		// calculate attractive forces
		for (map<int,Edge*>::iterator pE = edgeList.begin(); pE != edgeList.end(); pE++)
		{
			vector<float> u_pos(3), v_pos(3);
			getEndpoints(pE->first, u_pos, v_pos);
			float diffx =	v_pos[0]-u_pos[0];
			float diffy =   v_pos[1]-u_pos[1];
			float diffz =   v_pos[2]-u_pos[2];
			float dr = sqrt (diffx*diffx + diffy*diffy + diffz*diffz);	

			float force_a = (dr*dr)/k;
			float scale = dr;
			if (dr ==0) scale = -1.0;
			dx[pE->second->from->vID] += force_a * diffx/scale ;
			dy[pE->second->from->vID] += force_a * diffy/scale ;
			dz[pE->second->from->vID] += force_a * diffz/scale ;
			dx[pE->second->to->vID] += -1.0 * force_a * diffx/scale ;
			dy[pE->second->to->vID] += -1.0 * force_a * diffy/scale ;
			dz[pE->second->to->vID] += -1.0 * force_a * diffz/scale ;
		}
	
		// now do bounds checking and update all the positions
		for (map<int,Vertex*>::iterator V = vertexList.begin(); V != vertexList.end(); V++)
		{
			int id = V->first;
			vector<float> p(3);
			getXYZPos(id,p);
			p[0] += min (dx[id],t);
			p[1] += min (dy[id],t);			
			p[2] += min (dz[id],t);			
			// also do bounds checking on frame??
			p[0] = min(xmax,max(xmin,p[0]));
			p[1] = min(ymax,max(ymin,p[1]));
			p[2] = min(zmax,max(zmin,p[2]));
			
			setXYZPos(id, p[0], p[1], p[2]);
		}
		// now cool the temperature a bit
		t*=.98;
	}
}
//------------------------------------------------------------------------------
bool NiceGraph::validVID(int index)
{
	// find the vertex id
	for (map<int,Vertex*>::iterator iter = vertexList.begin(); iter != vertexList.end(); iter++)
	{
		if (iter->first == index)
			return true;	// found the index, return true, is valid
	}

	return false; // didn't find it, not valid
}
//------------------------------------------------------------------------------
bool NiceGraph::validEID(int index)
{
	// find the vertex id
	for (map<int,Edge*>::iterator iter = edgeList.begin(); iter != edgeList.end(); iter++)
	{
		if (iter->first == index)
			return true;	// found the index, return true, is valid
	}
	return false; // didn't find it, not valid
}
//------------------------------------------------------------------------------
void NiceGraph::getEdgeIndexList(vector<int> &list)
{
	// run through the edge id list
	for (map<int,Edge*>::iterator iter = edgeList.begin(); iter != edgeList.end(); iter++)
	{
		list.push_back(iter->first);
	}
}
//------------------------------------------------------------------------------
void NiceGraph::getVertexIndexList(vector<int> &list)
{
	// run through the vertex id list
	for (map<int,Vertex*>::iterator iter = vertexList.begin(); iter != vertexList.end(); iter++)
	{
		list.push_back(iter->first);
	}
}
// file operations
//------------------------------------------------------------------------------
void NiceGraph::loadFromFile(string filename)
{
	// NB. For loading edited text files, vertex IDs must be unique, but names need not be
	// NB. Loading from a file will automatically clear what is already there, so beware
	vertexList.clear();
	edgeList.clear();
	isDirected = true;
	edgeIDCounter = 0;
	vertexIDCounter = 0;

	// now get the file info
	ifstream inputStream (filename.c_str());
	string nextLine;

	string delims = ",";

	getline(inputStream, nextLine); // get first line

	// first do vertices
	if(nextLine.substr(0,9) == "#VERTICES")
	{
		getline(inputStream, nextLine);
		while (nextLine.substr(0,6) != "#EDGES")
		{
			vector<string> tokens;
			tokenize (nextLine, tokens, delims);
			if (tokens.size() != 6)
				cout << "ERROR: Wrong number of entries for vertex" << endl;
			else
			{	
				int v_id = atoi(tokens[0].c_str());
				vertexList[v_id] = new Vertex();
				vertexList[v_id]->vID = v_id;
				vertexList[v_id]->vColor = atoi(tokens[1].c_str());
				vertexList[v_id]->vName = tokens[2];
				setXYZPos(v_id, atof(tokens[3].c_str()), atof(tokens[4].c_str()), atof(tokens[5].c_str()));
				vertexIDCounter = std::max(vertexIDCounter, v_id);			
			}
			getline(inputStream, nextLine);	
		}
	}
	else
		cout << "Error: Can't find #VERTICES to begin parsing file" << endl;

	// now go though the edges
	while (getline(inputStream, nextLine))
	{		
		vector<string> tokens;
		tokenize (nextLine, tokens, delims);
		if (tokens.size() != 4)
			cout << "ERROR: Wrong number of entries for edge, has " << tokens.size() << endl;
		else
		{
			int e_id = atoi (tokens[0].c_str());
			int from_id = atoi (tokens[1].c_str());
			int to_id = atoi (tokens[2].c_str());
			edgeList[e_id] = new Edge();
			edgeList[e_id]->eID = e_id;
			edgeList[e_id]->from = vertexList[from_id];
			edgeList[e_id]->to = vertexList[to_id];
			edgeList[e_id]->weight = atof(tokens[3].c_str());
			vertexList[from_id]->out[e_id] = edgeList[e_id];
			vertexList[to_id]->in[e_id] = edgeList[e_id];
			edgeIDCounter = std::max(edgeIDCounter,e_id);
		}
	}
	inputStream.close();
	edgeIDCounter++;
	vertexIDCounter++;
}
//---------------------------------------------------------------------------------
void NiceGraph::saveToFile(string filename)
{
 	ofstream outFile (filename.c_str());
		
	outFile << "#VERTICES\n";
	// output the vertices
	for (map<int,Vertex*>::iterator iter = vertexList.begin(); iter != vertexList.end(); iter++)
	{
		outFile << iter->first << "," << iter->second->vColor << "," << iter->second->vName << ","
		<< iter->second->posX << "," << iter->second->posY << "," << iter->second->posZ << "\n";
	}
	outFile << "#EDGES\n";
	// run through the edge id list
	for (map<int,Edge*>::iterator iter = edgeList.begin(); iter != edgeList.end(); iter++)
	{
		Edge* pEdge = iter->second;
		outFile << iter->first << "," << pEdge->from->vID << "," << pEdge->to->vID << ","
		<< pEdge->weight << "\n";
	}
  	outFile.close();
}
//---------------------------------------------------------------------------------
// thanks to http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
void NiceGraph::tokenize (const string &str, vector<string> &tokens, const string &delimiters)
{
  // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
