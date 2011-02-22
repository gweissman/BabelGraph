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
/* 	FILE:  MainWindow.hpp
*  	DESCRIPTION:  Header file for the main window and user interface
*  	AUTHOR: Gary Weissman
*  	DATE: August 2008
*  	CONTACT: gary@babelgraph.org
*	URL: http://www.babelgraph.org/
*/

#ifndef MAINWINDOWT15451_H
#define MAINWINDOWT15451_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QDialog>
#include <QInputDialog>
#include <QString>

#include <vector>

#include "GraphWidget.hpp"
#include "randomgraphdialog.hpp"
#include "kregulargraphdialog.hpp"
#include "strangersbanquetgraphdialog.hpp"

class MainWindow : public QMainWindow
{

	Q_OBJECT

public:
	MainWindow();

private slots:
	void newGraph();
	void makeEmptyGraph();
	void makeCompleteGraph();
	void makeBinaryTreeGraph();
	void makeConnections();
	void showAboutWindow();
	void showHelp();
	void addVertex();
	void addEdge();
	void resetViewDim();
	void replotGraph();
	void makeVertexBrowserList();
	void loadVertexData(int index);
	void updateVertexName(QString name);
	void updateVertexColor(int newColor);
	void addNeighbor();
	void removeNeighbor();

	void makeStyleWindows();
	void makeStyleCDE();
	void makeStyleMotif();
	void makeStyleCleanLooks();
	void makeStylePlastique();

	void updatePlotValues();
	void changeBackgroundColor();
	void updateAnalysis();
	void highlightVertex();

	void exportImagePNG();
	void loadGraph();
	void saveGraph();

private:
	void setupUi(QMainWindow *MainWindow);
	void retranslateUi(QMainWindow *MainWindow);
	void makeDialogs(QMainWindow *MainWindow);

	RandomGraphDialog *randGraphDial;
	KRegularGraphDialog *kregGraphDial;
	StrangersBanquetGraphDialog *strangersbGraphDial;

    QAction *actionLoad_Graph;
    QAction *actionEdge_Data;
    QAction *actionAverage_Path_Length;
    QAction *actionExit;
    QAction *actionPreferences;
    QAction *actionBabelGraph_Help;
    QAction *actionAbout_BabelGraph;
    QAction *actionEdit_matrix;
    QAction *actionExport_Image;
    QAction *actionClustering;
    QAction *actionSave_Graph;
    QAction *actionDiameter;
    QAction *actionCentrality;
    QAction *actionPath_Length;
    QAction *actionClustering_2;
    QAction *actionDegree_Distribution;
    QAction *actionEmpty;
    QAction *actionRandom;
    QAction *actionK_Regular;
    QAction *actionComplete;
    QAction *actionSmall_World;
    QAction *actionBinary_Tree;
    QAction *actionSave_Graph_As;
    QAction *actionEmpty_2;
    QAction *actionRandom_2;
    QAction *actionK_Regular_2;
    QAction *actionBinary_Tree_2;
    QAction *actionComplete_2;
    QAction *actionStrangers_Banquet;
    QAction *actionNew_Graph;
    QAction *actionImport_Graph;
    QAction *actionExport_Graph;
  QAction *actionMake_Undirected;
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget_6;
    QHBoxLayout *top_button_bar;
    QPushButton *addVertexButton;
    QPushButton *addEdgeButton;
    QPushButton *zoomInButton;
    QPushButton *zoomOutButton;
    QPushButton *toggleAxesButton;
    QPushButton *resetViewButton;
    QPushButton *pushButton_Update_Analysis;
    QSlider *sliderZ;
    QSlider *sliderY;
    QSlider *sliderX;
    GraphWidget *graphWidget;
    QTabWidget *SideBar;
    QWidget *tab_browse_edit;
    QGroupBox *groupBox_Properties;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_Name;
    QLineEdit *lineEdit_NameEdit;
    QLabel *label_Color;
    QComboBox *comboBox_ColorSelect;
    QGroupBox *groupBox_Neighbors;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QComboBox *comboBox_AddNodeList;
    QComboBox *comboBox_RemoveNodeList;
	QPushButton *addNeighborButton;
    	QPushButton *removeNeighborButton;
    QGroupBox *groupBox_Analysis;
    QWidget *formLayoutWidget_3;
    QFormLayout *formLayout_3;
 QLabel *label_Degree;
    QLCDNumber *lcdNumber_Degree;
    QLabel *label_Degree_in;
    QLCDNumber *lcdNumber_Degree_in;
    QLabel *label_Degree_out;
    QLCDNumber *lcdNumber_Degree_out;
    QLabel *label_AvgShortestPath;
    QLCDNumber *lcdNumber_AvgShortestPath;
    QLabel *label_PageRank;
    QLCDNumber *lcdNumber_PageRank;
    QLabel *label_CloesnessCentrality;
    QLCDNumber *lcdNumber_ClosenessCentrality;
    QLabel *label_ClusteringCoefficient;
    QLCDNumber *lcdNumber_ClusteringCoefficient;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QComboBox *vertexSelectorComboBox;
    QPushButton *zoomToVertex;
    QLabel *label_Vertex;
    QWidget *tab_layout;
    QGroupBox *layoutOptionsGroup;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton_3D;
    QComboBox *comboBox_3DLayout;
    QRadioButton *radioButton_2D;
    QComboBox *comboBox_2DLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSlider *horizontalSlider_Spring;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QSlider *horizontalSlider_Repulsion;
    QHBoxLayout *horizontalLayout;
    QToolButton *toolButton_AutoArrange;
    QGroupBox *plottingOptionsGroup;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *labelXRange;
    QDoubleSpinBox *xMin;
    QDoubleSpinBox *yMin;
    QLabel *labelYRange;
    QDoubleSpinBox *yMax;
    QDoubleSpinBox *xMax;
    QDoubleSpinBox *zMin;
    QDoubleSpinBox *zMax;
    QLabel *labelZRange;
    QLabel *labelVertexRadius;
    QDoubleSpinBox *vertexRadius;
	    QPushButton *button_Replot;
		QPushButton *button_Default_Plot;
	    QAction *actionSet_Background_Color;
    QAction *actionShow_Vertex_Names;
    QAction *actionShow_Vertex_Indices;
    QAction *actionCleanLooks;
    QAction *actionWindows;
    QAction *actionPlastique;
    QAction *actionMotif;
    QAction *actionCDE;
    QWidget *tab_analysis;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuAnalysis;
    QMenu *menuGraph_Data;
    QMenu *menuVertex_Data;
    QMenu *menuGraph;
    QMenu *menuGenerate_Graph;
    QMenu *menuView;
    QMenu *menuSet_Window_Style;

	
};

#endif // MAINWINDOWT15451_H
