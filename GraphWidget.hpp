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
/* 	FILE: GraphWidget.hpp
*  	DESCRIPTION:  Header file of a Qt widget that inherits the Qt-OpenGL
*					widget and paints graphs.
*  	AUTHOR: Gary Weissman
*  	DATE: August 2008
*  	CONTACT: gary@babelgraph.org
*	URL: http://www.babelgraph.org/
*/

#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGLWidget>
#include <QWheelEvent>
#include <QtGui>
#include <QtOpenGL>
#ifdef __APPLE__
  #include <OpenGL/glu.h>
  #include <OpenGL/gl.h>
#else
  #ifdef _WIN32
    #include <windows.h>
  #endif
  #include <GL/glu.h>
  #include <GL/gl.h>
#endif

#include <cmath>
#include <string>
#include <vector>
#include "NiceGraph.hpp"

typedef struct
{
	float posX, posY, posZ;		// position vector
	float dirX, dirY, dirZ;		// direction vector
	float upX, upY, upZ;		// up vector
} gCamera;

typedef struct {
   GLfloat r,g,b,a;
} COLOR;

class GraphWidget : public QGLWidget 
 {

	         Q_OBJECT        // must include this if you use Qt signals/slots

 public:
	GraphWidget(QWidget *parent = 0);
	~GraphWidget();

	// input functions from main window
	void clearGraph();
	void makeEmpty(int num);	
	void makeComplete(int num);
	void makeBinaryTree(int generations);
	void addVertex(string name);
	void removeEdge(int from, int to);
	void addEdge(int from, int to);
	int getNumV();
	int getNumE();
	string getName(int index);
	void setName(int index, string name);
	int getColor(int index);
	void setColor (int index, int color);
	vector<int> getOutNeighborIndexList(int index);
	int getInDegree(int index);
	int getOutDegree(int index);
	int getDegree(int index);
	bool isValidVID (int index);
	void loadFile(string filename);
	void saveFile(string filename);
	float getAvgShortestPath(int index);
	float getPageRank(int index);
	float getCCentrality(int index);
	float getClusteringCoefficient(int index);
	pair<float,float> getXRange();
	pair<float,float> getYRange();
	pair<float,float> getZRange();
	float getVertexRadius();
	void setBackgroundColor(int r, int g, int b, int a = 255);
	void setBackgroundColor(float r, float g, float b, float a = 1.0);
	void highlightVertex(int index);

 public slots:
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void toggleAxes();
	void resetView();
	void set2DView();
	void set3DView();
	void zoomIn();
	void zoomOut();
	void makekRegular(int num, int k);
	void makeRandom(int num, double density);
	void makeStrangersBanquet(int num, int groups, float density, float mu);
	void toggleSelfOrganize();
	void selfOrganizeOneStep();
	void setSelfOrganizeDelay(int delay);
	void setSelfOrganizeRepulsion(int repulsion);
	void refresh2DLayout(int index);
	void refresh3DLayout(int index);
	void replot(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, float radius);
	void setDefaultPlot();
	void panToVertex(int index);
	void makeUndirected();
	void updateAnalysis();
	

 signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);
	void graphChanged();
	void plottingAreaUpdated();

 protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);


 private:
     	void normalizeAngle(int *angle);
	bool showAxes;
	void paintGraph();
	void resetCamera();
	void drawSphere(float radius, float x, float y, float z);
	void drawArrow(float x, float y, float z, vector<float> n);
	NiceGraph *g;
	bool is3D, selfOrganize;
	void initializeColors();
	QTimer *selfOrganizeTimer;
	int selfOrganizeDelay;
	float nodeRadius, autoRepulsion, xMin, xMax, yMin, yMax, zMin, zMax;
	vector<int> edgeIndexList;
	vector<int> vertexIndexList;
	map<int,float>shortestPaths;
	map<int,float>pRank;
	map<int,float>cCentrality;


	int currentVertex, highlightV;

	COLOR cYELLOW, cPURPLE, cRED, cORANGE, cWHITE, cBLACK, cGREEN, cBLUE;
	enum {YELLOW=0, PURPLE, RED, ORANGE, WHITE, BLACK, GREEN, BLUE};
	
	vector<COLOR> colorList;

	gCamera gCam;

	float dist;

	GLuint object;
	int xRot, yRot, zRot;
	QPoint lastPos;

private slots:
	void updateGraphIndices();
 };

 #endif
