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
/* 	FILE: GraphWidget.cpp
*  	DESCRIPTION: Implementation of a Qt widget that inherits the Qt-OpenGL
*					widget and paints graphs.
*  	AUTHOR: Gary Weissman
*  	DATE: August 2008
*  	CONTACT: gary@babelgraph.org
*	URL: http://www.babelgraph.org/
*/

#include "GraphWidget.hpp"

GraphWidget::GraphWidget(QWidget *parent)
     : QGLWidget(parent)
 {
	xRot = 0;
	yRot = 0;
	zRot = 0;

	initializeColors();
	
	showAxes = true;
	is3D = true;
	selfOrganize = false;

	resetCamera();

	setDefaultPlot();

	g = new NiceGraph();

	selfOrganizeDelay = 1000;
	selfOrganizeTimer = new QTimer(this);
     	connect(selfOrganizeTimer, SIGNAL(timeout()), this, SLOT(selfOrganizeOneStep()));
     	selfOrganizeTimer->start(selfOrganizeDelay);

	autoRepulsion = 0.5;
	currentVertex = -1;
	highlightV = -1;

	updateGraphIndices();

	QObject::connect(this,SIGNAL(graphChanged()),this,SLOT(updateGraphIndices()));
	QObject::connect(this,SIGNAL(graphChanged()),this,SLOT(updateGL()));
	//QObject::connect(this,SIGNAL(graphChanged()),this,SLOT(updateAnalysis())); <-- too intensive, user should call it

 }

 GraphWidget::~GraphWidget()
 {
	if (g!=NULL)
		delete g;

	if (selfOrganizeTimer!=NULL)
		delete selfOrganizeTimer;	
 }


 void GraphWidget::setXRotation(int angle)
 {

	if (is3D) {
     normalizeAngle(&angle);
     if (angle != xRot) {
         xRot = angle;
         emit xRotationChanged(angle);
         updateGL();
     }
	}
 }

 void GraphWidget::setYRotation(int angle)
 {
	if (is3D) {
     normalizeAngle(&angle);
     if (angle != yRot) {
         yRot = angle;
         emit yRotationChanged(angle);
         updateGL();
     }
	}
 }

 void GraphWidget::setZRotation(int angle)
 {
     normalizeAngle(&angle);
     if (angle != zRot) {
         zRot = angle;
         emit zRotationChanged(angle);
         updateGL();
     }
 }

void GraphWidget::toggleAxes()
{

	showAxes = !showAxes;

	updateGL();
}

void GraphWidget::toggleSelfOrganize()
{
	selfOrganize = !selfOrganize;
}

void GraphWidget::selfOrganizeOneStep()
{
	if (selfOrganize)
	{
		g->selfOrganize (autoRepulsion, 0.05, .1);
		updateGL();
	}
}
void GraphWidget::setSelfOrganizeDelay(int delay)
{
	selfOrganizeDelay = 1001 - delay;
	selfOrganizeTimer->setInterval(selfOrganizeDelay);
}

void GraphWidget::setSelfOrganizeRepulsion(int repulsion)
{
	autoRepulsion = (repulsion / 1000.0) * (repulsion / 1000.0);
}
void GraphWidget::panToVertex(int index)
{
	//int time = 5;		// choose some number of seconds to do the whole pan
}
void GraphWidget::resetCamera()
{
	// default camera position -- should depend on layout of graph

	gCam.posX = 0.0 ;
	gCam.posY = 0.0 ;
	gCam.posZ = -5.0;

	// default look at origin		
	gCam.dirX = 0.0;
	gCam.dirY = 0.0;
	gCam.dirZ = 0.0;

	// default up position
	gCam.upX = 0.0;
	gCam.upY = 1.0;
	gCam.upZ = 0.0;

}

void GraphWidget::resetView()
{
	resetCamera();
	setXRotation(0);
	setYRotation(0);
	setZRotation(0);
	updateGL();

}

void GraphWidget::set2DView()
{
	resetView();
	is3D = false;	

}
void GraphWidget::set3DView()
{
	resetView();
	is3D = true;
}	

void GraphWidget::makeUndirected()
{
	g->makeUndirected();
	emit graphChanged();
}

void GraphWidget::refresh2DLayout(int index)
{
	switch(index)
	{
		case 0:			// random layout
			g->randomLayoutAll(xMin,xMax,yMin,yMax,0,0);
		break;
		case 1:			// circle layout
			g->circleLayout(zMax - zMin);
		break;

		case 2:			// spiral layout
			g->spiralLayout(zMax - zMin);
		break;

		case 3:			// layers by group
			g->layersByGroupLayout(zMax - zMin, nodeRadius);		
		break;

		case 4:			// 2D Fruchterman Reingold
			g->fruchtermanReingoldLayout(xMin,xMax,yMin,yMax,0,0);
		break;
	}
	updateGL();
}

void GraphWidget::refresh3DLayout(int index) 
{
	switch(index)
	{

		case 0:			// random layout
			g->randomLayoutAll(xMin,xMax,yMin,yMax,zMin,zMax);
		break;

		case 1:			// sphere layout
			g->sphereLayout(zMax - zMin);
		break;


		case 2:			// circle layout
			g->circleLayout(zMax - zMin);
		break;

		case 3:			// spiral layout
			g->spiralLayout(zMax - zMin);
		break;	

		case 4:			// fruchterman-reingold
			g->fruchtermanReingoldLayout(xMin,xMax,yMin,yMax,zMin,zMax);
		break;

		case 5:			// layers by group
			g->layersByGroupLayout(zMax - zMin, nodeRadius);		
		break;
	}
	updateGL();
}
void GraphWidget::updateAnalysis()
{
	g->avgShortestPathMatrix(shortestPaths);
	g->getPageRank(pRank);
	g->getClosenessCentrality(cCentrality);
}
float GraphWidget::getAvgShortestPath(int index)
{
	return shortestPaths[index];
}
float GraphWidget::getPageRank(int index)
{
	return pRank[index];
}
float GraphWidget::getCCentrality(int index)
{
	return cCentrality[index];
}
float GraphWidget::getClusteringCoefficient(int index)
{
	return g->getClusteringCoefficient(index);	
}
void GraphWidget::replot(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, float radius)
{
	// reset coordinates

	xMin = xmin;
	xMax = xmax;
	yMin = ymin;
	yMax = ymax;
	zMin = zmin;
	zMax = zmax;
	nodeRadius = radius;

	// now replot position of each vertex

	// update painting
	updateGL();
}
void GraphWidget::setDefaultPlot()
{
	xMin = -1.0; xMax = 1.0;
	yMin = -1.0; yMax = 1.0;
	zMin = -1.0; zMax = 1.0;
	nodeRadius = .05;
	emit plottingAreaUpdated();
	updateGL();
}
void GraphWidget::highlightVertex(int index)
{
	if (index == highlightV)	// if double click, turn off highlight
		highlightV = -1;
	else				// otherwise highlight the next fellow
		highlightV = index;

	updateGL();
}
pair<float,float> GraphWidget::getXRange() {pair<float,float> xrange (xMin,xMax); return xrange;}
pair<float,float> GraphWidget::getYRange() {pair<float,float> yrange (yMin,yMax); return yrange;}
pair<float,float> GraphWidget::getZRange() {pair<float,float> zrange (zMin,zMax); return zrange;}
float GraphWidget::getVertexRadius() {return nodeRadius;}
void GraphWidget::zoomIn()
{
	gCam.posZ = gCam.posZ * 0.9;
	updateGL();
}
void GraphWidget::zoomOut()
{
	gCam.posZ = gCam.posZ * 1.1;
	updateGL();
}
void GraphWidget::initializeGL()
{
     glClearColor (0.2f, 0.2f, 0.4f, 1.0f);  
     glShadeModel(GL_FLAT);
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
}
void GraphWidget::setBackgroundColor(int r, int g, int b, int a)
{
	glClearColor (r/255.0,g/255.0,b/255.0,a/255.0);
}
void GraphWidget::setBackgroundColor(float r, float g, float b, float a)
{
	glClearColor (r,g,b,a);
}
void GraphWidget::paintGL()
{
	// initialize the window and rotate it as needed

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(	gCam.posX, gCam.posY, gCam.posZ, 
			gCam.dirX, gCam.dirY, gCam.dirZ, 
			gCam.upX, gCam.upY, gCam.upZ ); 
	if (is3D)
	{	
   		glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
   		glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	}
   	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);

// paint the axes of the option is selected
	if (showAxes)
	{
		glBegin(GL_LINES);
		glColor3f(255.0f,255.0f,255.0f);	// white

		glVertex3f (xMin, 0.0, 0.0);
		glVertex3f (xMax, 0.0, 0.0);

		glVertex3f (0.0, yMin, 0.0);
		glVertex3f (0.0, yMax, 0.0);
	
		glVertex3f (0.0, 0.0, zMin);
		glVertex3f (0.0, 0.0, zMax);
		
		glEnd();
	}

	// paint the graph if it exists
	if (g != NULL)
	{
		paintGraph();
	}
}
void GraphWidget::paintGraph()
{
	// paint the vertices
	for (unsigned int v = 0; v < vertexIndexList.size(); v++)
	{
		int index = vertexIndexList[v];
		vector<float> tempXYZ (3);
		g->getXYZPos(index, tempXYZ);
		COLOR color = colorList[ (g->getVertexColor(index)) ];
		if (index == highlightV)
		{	
			color.r*=0.65;color.g*=0.65;color.b*=0.65;color.a*=0.65;
		}
		glColor4f(color.r,color.g,color.b,color.a);
		float Z;
		if (is3D)
			Z = tempXYZ[2];
		else
			Z = 0;
		drawSphere(nodeRadius, tempXYZ[0], tempXYZ[1] , Z);
	}

	// paint the edges
	for (unsigned int e = 0; e < edgeIndexList.size(); e++)
	{
		int index = edgeIndexList[e];
		vector<float> tFrom (3), tTo(3);
		g->getEndpoints (index, tFrom, tTo);	// if this edge ID exists
			
		COLOR color = colorList[GREEN];
		glColor4f(color.r, color.g, color.b, color.a);
		float Zfrom, Zto;
		if (is3D)
		{
			Zfrom = tFrom[2];
			Zto = tTo[2];
		}
		else
		{
			Zfrom = 0;
			Zto = 0;
		}
		glBegin(GL_LINES);
		glVertex3f(tFrom[0], tFrom[1], Zfrom);
		glVertex3f(tTo[0], tTo[1], Zto);		
		glEnd();

		if (g->checkDirected())		//paint arrows if edges are directed
		{
			vector<float> n(3);
			float vx =tTo[0]-tFrom[0];
			float vy =tTo[1]-tFrom[1];
			float vz =tTo[2]-tFrom[2];
			float mag = sqrt (vx*vx+vy*vy+vz*vz);
			n[0]=vx/mag;
			n[1]=vy/mag;
			n[2]=vz/mag;
			drawArrow(tTo[0], tTo[1], Zto, n);
		}
	}
}
void GraphWidget::drawSphere(float radius, float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x,y,z);
	gluSphere(gluNewQuadric(), radius, 15, 15); 
	glPopMatrix();
	//gluDeleteQuadric();
}
// the arrow thing doesn't work!  whoever gets this right gets a prize!!
void GraphWidget::drawArrow(float x, float y, float z, vector<float> n)
{
		float rTd = 180.0/3.1415926;
		glPushMatrix();
		glTranslatef(x, y, z);
	
		float theta = rTd*acos(n[2]);
		glRotatef(180+theta,-1.0*n[1],n[0],0);
		
		gluCylinder(gluNewQuadric(), 0, .05, .2, 30, 20);
		
		glPopMatrix();
}
void GraphWidget::resizeGL(int width, int height)
{
     glViewport( 0 , 0 , width , height);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
	// modify these two so they are proportion...
	gluPerspective(45, (GLdouble)width/(GLdouble)height, 0.1, 50); 
	glOrtho(width / 2, width / 2, height / 2, height / 2, 0.1, 10);

     glMatrixMode(GL_MODELVIEW);
}
void GraphWidget::mousePressEvent(QMouseEvent *event)
{
     lastPos = event->pos();
}
void GraphWidget::mouseMoveEvent(QMouseEvent *event)
{
     int dx = lastPos.x() - event->x()  ;
     int dy = lastPos.y() - event->y()  ;

     if (event->buttons() & Qt::LeftButton) {
         setXRotation(xRot + 8 * dy);
         setYRotation(yRot + 8 * dx);
     } else if (event->buttons() & Qt::RightButton) {
         setXRotation(xRot + 8 * dy);
         setZRotation(zRot + 8 * dx);
     }
     lastPos = event->pos();
}
void GraphWidget::wheelEvent(QWheelEvent *event)
{

	float scale = 0;

	if (event->delta() > 0)
		scale = 1.1;
	else if (event->delta() < 0)
		scale = 0.9;

	gCam.posZ = gCam.posZ * scale;


	updateGL();
}
void GraphWidget::normalizeAngle(int *angle)
{
     while (*angle < 0)
         *angle += 360 * 16;
     while (*angle > 360 * 16)
         *angle -= 360 * 16;
}
void GraphWidget::clearGraph()
{

	if (g)
		delete g;

	g = new NiceGraph();


	emit graphChanged();

}
void GraphWidget::makeEmpty(int num)
{
	if (g)
		delete g;

	g = new NiceGraph();

	g->makeEmptyGraph(num);
	g->randomLayoutAll(xMin,xMax,yMin,yMax,zMin,zMax);

	emit graphChanged();

}
void GraphWidget::makeComplete(int num)
{
	if (g)
		delete g;

	g = new NiceGraph();
	g->makeCompleteGraph(num);
	g->randomLayoutAll(xMin,xMax,yMin,yMax,zMin,zMax);

	emit graphChanged();

}
void GraphWidget::makeBinaryTree(int generations)
{
	if (g)
		delete g;

	g = new NiceGraph();
	g->makeBinaryTreeGraph(pow(2.0,generations) - 1);
	g->randomLayoutAll(xMin,xMax,yMin,yMax,zMin,zMax);

	emit graphChanged();
}
void GraphWidget::makeRandom(int num, double density)
{
	if (g)
		delete g;

	g = new NiceGraph();
	g->makeRandomGraph(num,density);
	g->randomLayoutAll(xMin,xMax,yMin,yMax,zMin,zMax);
	emit graphChanged();
}
void GraphWidget::makekRegular(int num, int k)
{
	if (g)
		delete g;

	g = new NiceGraph();
	g->makeKRegularGraph(num,k);
	g->randomLayoutAll(xMin,xMax,yMin,yMax,zMin,zMax);
	emit graphChanged();

}

void GraphWidget::makeStrangersBanquet(int num, int groups, float density, float mu)
{
	if (g)
		delete g;
	g = new NiceGraph();
	g->makeStrangersBanquetGraph(num, groups, density, mu);
	g->randomLayoutAll(xMin,xMax,yMin,yMax,zMin,zMax);
	emit graphChanged();
}
vector<int> GraphWidget::getOutNeighborIndexList(int index)
{
	vector<int> list = g->getOutNeighborList(index);
	return list;
}
int GraphWidget::getInDegree(int index)
{
	return g->getInDegree(index);
}
int GraphWidget::getOutDegree(int index)
{
	return g->getOutDegree(index);

}
int GraphWidget::getDegree(int index)
{
	return g->getDegree(index);
}
void GraphWidget::addVertex(string name)
{
	if (g)
		g->addVertex(name);
	else {
		g->makeEmptyGraph(0);
		g->addVertex(name);
	}

	g->randomLayout(g->getNumVertices() - 1,xMin, xMax, yMin, yMax, zMin, zMax);
	emit graphChanged();
}
void GraphWidget::removeEdge(int from, int to)
{
	g->removeEdge(from,to);
	emit graphChanged();
}

void GraphWidget::addEdge(int from, int to)
{
	g->addEdge(from,to);
	emit graphChanged();
	
}
bool GraphWidget::isValidVID(int index)
{

	return g->validVID(index);

}
void GraphWidget::saveFile(string filename)
{
	g->saveToFile(filename);
}
void GraphWidget::loadFile(string filename)
{
	if (g)
		delete g;
	g = new NiceGraph();
	g->loadFromFile(filename);

	emit graphChanged();
}
int GraphWidget::getNumV() { return g->getNumVertices(); }
int GraphWidget::getNumE() { return g->getNumEdges(); }
string GraphWidget::getName(int index) { return g->getVertexName(index); }
void GraphWidget::setName(int index, string name) {g->setVertexName(index,name); updateGL();}
int GraphWidget::getColor(int index) {return g->getVertexColor(index); }
void GraphWidget::setColor(int index, int color) { g->setVertexColor(index,color); updateGL(); }
void GraphWidget::updateGraphIndices()
{
	edgeIndexList.clear();
	vertexIndexList.clear();
	g->getEdgeIndexList(edgeIndexList);
	g->getVertexIndexList(vertexIndexList);
}
void GraphWidget::initializeColors()
{
	cYELLOW.r = 1.0f;
	cYELLOW.g = 1.0f;
	cYELLOW.b = 0;
	cYELLOW.a = 0;
	colorList.push_back(cYELLOW);

	cPURPLE.r = 192.0/255.0;
	cPURPLE.g = 64.0/255.0;
	cPURPLE.b = 192.0/255.0;
	cPURPLE.a = 0;
	colorList.push_back(cPURPLE);

	cRED.r = 255.0/255.0;
	cRED.g = 0;
	cRED.b = 0;
	cRED.a = 0;
	colorList.push_back(cRED);

	cORANGE.r = 255.0/255.0;
	cORANGE.g = 165.0/255.0;
	cORANGE.b = 0;
	cORANGE.a = 0;
	colorList.push_back(cORANGE);
 
	cWHITE.r = 255.0/255.0;
	cWHITE.g = 255.0/255.0;
	cWHITE.b = 255.0/255.0;
	cWHITE.a = 0;
	colorList.push_back(cWHITE);

	cBLACK.r = 0;
	cBLACK.g = 0;
	cBLACK.b = 0;
	cBLACK.a = 0;
	colorList.push_back(cBLACK);

	cGREEN.r = 0;
	cGREEN.g = 255.0/255.0;
	cGREEN.b = 0;
	cGREEN.a = 0;
	colorList.push_back(cGREEN);

	cBLUE.r = 0;
	cBLUE.g = 0;
	cBLUE.b = 255.0/255.0;
	cBLUE.a = 0;
	colorList.push_back(cBLUE);
}
