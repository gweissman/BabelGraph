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
/* 	FILE: MainWindow.cpp
*  	DESCRIPTION: Implementation of the main window and user interface
*  	AUTHOR: Gary Weissman
*  	DATE: August 2008
*  	CONTACT: gary@babelgraph.org
*	URL: http://www.babelgraph.org/
*/

#include "MainWindow.hpp"

MainWindow::MainWindow() 
{
	setupUi(this);
	
	makeDialogs(this);

	makeConnections();

	resetViewDim(); // call again here during setup to make sure the the right widgets are enabled/disabled

}

void MainWindow::makeConnections()
{
	// top button row related signals
	QObject::connect(toggleAxesButton, SIGNAL( clicked() ), graphWidget, SLOT( toggleAxes() ) );
	QObject::connect(toolButton_AutoArrange, SIGNAL( clicked() ), graphWidget, SLOT( toggleSelfOrganize() ) );
    	QObject::connect(resetViewButton, SIGNAL ( clicked() ), graphWidget, SLOT (resetView() ));
	QObject::connect(zoomInButton, SIGNAL (clicked() ), graphWidget, SLOT (zoomIn() ));
	QObject::connect(zoomOutButton, SIGNAL (clicked() ), graphWidget, SLOT (zoomOut() ));
	QObject::connect(addVertexButton, SIGNAL (clicked() ), this, SLOT (addVertex() ));
	QObject::connect(addEdgeButton, SIGNAL (clicked() ), this, SLOT (addEdge() ));
	
	// window menu related signals
	QObject::connect(actionNew_Graph, SIGNAL ( triggered() ), this, SLOT( newGraph() ));
    	QObject::connect(actionExit, SIGNAL ( triggered() ), this, SLOT (close() ));
	QObject::connect(actionEmpty_2, SIGNAL( triggered() ), this, SLOT(makeEmptyGraph()));
	QObject::connect(actionComplete_2, SIGNAL (triggered() ), this, SLOT (makeCompleteGraph()));
	QObject::connect(actionBinary_Tree_2, SIGNAL(triggered() ), this, SLOT (makeBinaryTreeGraph()));
	QObject::connect(actionRandom_2, SIGNAL (triggered() ), randGraphDial, SLOT (exec() ));
	QObject::connect(randGraphDial, SIGNAL (randomGraphInfo(int,double)) , graphWidget, SLOT (makeRandom(int, double) ));
	QObject::connect(actionK_Regular_2, SIGNAL (triggered() ), kregGraphDial, SLOT (exec() ));
	QObject::connect(kregGraphDial, SIGNAL (kRegularGraphInfo(int,int)) , graphWidget, SLOT (makekRegular(int, int) ));
	QObject::connect(actionStrangers_Banquet, SIGNAL (triggered() ), strangersbGraphDial, SLOT (exec() ));
	QObject::connect(strangersbGraphDial, SIGNAL (strangersbGraphInfo(int,int,float,float)) , graphWidget, 
				SLOT (makeStrangersBanquet(int,int,float,float) ));
	QObject::connect(actionWindows, SIGNAL( triggered() ), this, SLOT (makeStyleWindows() ));
	QObject::connect(actionCDE, SIGNAL( triggered() ), this, SLOT (makeStyleCDE() ));
	QObject::connect(actionMotif, SIGNAL( triggered() ), this, SLOT (makeStyleMotif() ));
	QObject::connect(actionCleanLooks, SIGNAL( triggered() ), this, SLOT (makeStyleCleanLooks() ));
	QObject::connect(actionPlastique, SIGNAL( triggered() ), this, SLOT (makeStylePlastique() ));
	QObject::connect(actionAbout_BabelGraph, SIGNAL ( triggered() ), this, SLOT (showAboutWindow() ));
	QObject::connect(actionExport_Image, SIGNAL (triggered() ), this, SLOT (exportImagePNG() ));
	QObject::connect(actionMake_Undirected, SIGNAL (triggered() ), graphWidget, SLOT( makeUndirected()   ));
	QObject::connect(actionSave_Graph, SIGNAL(triggered() ), this, SLOT(saveGraph()));
	QObject::connect(actionLoad_Graph, SIGNAL(triggered() ), this, SLOT(loadGraph()));
	QObject::connect(actionBabelGraph_Help, SIGNAL(triggered() ), this, SLOT(showHelp()));
	QObject::connect(actionSet_Background_Color, SIGNAL(triggered() ), this, SLOT (changeBackgroundColor()));

   	
	// slider related signals
	QObject::connect(sliderX, SIGNAL(valueChanged(int)), graphWidget, SLOT(setXRotation(int)));
  	QObject::connect(graphWidget, SIGNAL(xRotationChanged(int)), sliderX, SLOT(setValue(int)));
 	QObject::connect(sliderY, SIGNAL(valueChanged(int)), graphWidget, SLOT(setYRotation(int)));
  	QObject::connect(graphWidget, SIGNAL(yRotationChanged(int)), sliderY, SLOT(setValue(int)));
  	QObject::connect(sliderZ, SIGNAL(valueChanged(int)), graphWidget, SLOT(setZRotation(int)));
  	QObject::connect(graphWidget, SIGNAL(zRotationChanged(int)), sliderZ, SLOT(setValue(int)));

	//sidebar related activity
	QObject::connect(horizontalSlider_Spring, SIGNAL(valueChanged(int)), graphWidget, SLOT(setSelfOrganizeDelay(int)));
	QObject::connect(horizontalSlider_Repulsion, SIGNAL(valueChanged(int)), graphWidget, SLOT(setSelfOrganizeRepulsion(int)));
	QObject::connect(radioButton_3D, SIGNAL(toggled(bool)), this, SLOT(resetViewDim()));
	QObject::connect(comboBox_3DLayout, SIGNAL(currentIndexChanged(int)), graphWidget, SLOT(refresh3DLayout(int)));
	QObject::connect(comboBox_2DLayout, SIGNAL(currentIndexChanged(int)), graphWidget, SLOT(refresh2DLayout(int)));
	QObject::connect(button_Replot, SIGNAL(clicked()), this, SLOT(replotGraph()));
	QObject::connect(button_Default_Plot, SIGNAL(clicked()), graphWidget, SLOT(setDefaultPlot()));
	QObject::connect(graphWidget,SIGNAL(plottingAreaUpdated()), this, SLOT(updatePlotValues()));
	QObject::connect(zoomToVertex, SIGNAL(clicked()), this, SLOT(highlightVertex() )); 
	QObject::connect(vertexSelectorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadVertexData(int)));
	QObject::connect(lineEdit_NameEdit, SIGNAL(textEdited(QString)), this, SLOT(updateVertexName(QString)));
	QObject::connect(comboBox_ColorSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVertexColor(int)));
	QObject::connect(addNeighborButton, SIGNAL(clicked()), this, SLOT(addNeighbor()));
	QObject::connect(removeNeighborButton, SIGNAL(clicked()), this, SLOT(removeNeighbor()));
	QObject::connect(pushButton_Update_Analysis, SIGNAL(clicked()), this, SLOT(updateAnalysis()));

	// GraphWidget Signals
	QObject::connect(graphWidget, SIGNAL(graphChanged()), this, SLOT(makeVertexBrowserList()));
}

void MainWindow::highlightVertex()
{
	int index = vertexSelectorComboBox->currentIndex();

	if (graphWidget->isValidVID(index))
	{
		graphWidget->highlightVertex(index);
	}
}

void MainWindow::changeBackgroundColor()
{
	bool ok;
     	QRgb newColor = QColorDialog::getRgba(Qt::blue, &ok, this);
     	if (ok) 
	{
		graphWidget->setBackgroundColor(qRed(newColor), qGreen(newColor), qBlue(newColor), qAlpha(newColor) );
     	}
}
void MainWindow::showHelp()
{
	QString help_info = "For more help see http://www.babelgraph.org/<br>More help coming soon!<br>For bugs email gary@babelgraph.org";
	
     	QMessageBox::information(this, "BabelGraph Help", help_info);

}
void MainWindow::updateAnalysis()
{
	graphWidget->updateAnalysis();

	makeVertexBrowserList();

	pushButton_Update_Analysis->setEnabled(false);
}

void MainWindow::saveGraph()
{
	QString title = "Save graph as BabelGraph .bgx file";
	QString fileName = QFileDialog::getSaveFileName(this,
                                 title,
				"new_graph.bgx",
                                 "BabelGraph Files (*.bgx);;All Files (*)");
	graphWidget->saveFile(fileName.toStdString());
}

void MainWindow::loadGraph()
{
	QString title = "Load graph from BabelGraph .bgx file";
  	QString fileName = QFileDialog::getOpenFileName(this,
                                 title,
                                 " ",
                                 "BabelGraph Files (*.bgx);;All Files (*)");

	graphWidget->loadFile(fileName.toStdString());

}

void MainWindow::exportImagePNG()
{
	QString title = "Export image as PNG file";
	QString fileName = QFileDialog::getSaveFileName(this,
                                 title,
				"graph_image.png",
                                 "PNG Files (*.png);;All Files (*)");

	QImage image = graphWidget->grabFrameBuffer();

	int quality = 100;

	image.save (fileName,"PNG", quality );
}

void MainWindow::showAboutWindow()
{
	QString title = "About BabelGraph 0.2";
	QString message = "<b>BabelGraph v0.2</b><br>Written by Gary Weissman<br>http://www.babelgraph.org/<br>Distributed under GPLv3";

	QMessageBox::about(this, title, message);
}
void MainWindow::newGraph()
{
	graphWidget->clearGraph();
	makeVertexBrowserList();
}

void MainWindow::makeStyleCDE()
{
	QApplication::setStyle(new QCDEStyle);
}
void MainWindow::makeStyleMotif()
{
	QApplication::setStyle(new QMotifStyle);
}
void MainWindow::makeStyleCleanLooks()
{
	QApplication::setStyle(new QCleanlooksStyle);
}
void MainWindow::makeStylePlastique()
{
	QApplication::setStyle(new QPlastiqueStyle);
}
void MainWindow::makeStyleWindows()
{
	QApplication::setStyle(new QWindowsStyle);
}

void MainWindow::removeNeighbor()
{

	int from = vertexSelectorComboBox->currentIndex();
	int index = comboBox_RemoveNodeList->currentIndex();
	QVariant to_value = comboBox_RemoveNodeList->itemData(index, Qt::UserRole);
	int to = to_value.toInt();
	
	if ((from >= 0) && (to >= 0))
		graphWidget->removeEdge(from,to);

	vertexSelectorComboBox->setCurrentIndex(from);

}

void MainWindow::addNeighbor()
{
	int from = vertexSelectorComboBox->currentIndex();
	int index = comboBox_AddNodeList->currentIndex();
	QVariant to_value = comboBox_AddNodeList->itemData(index, Qt::UserRole);
	int to = to_value.toInt();
	
	if ((from >= 0) && (to >= 0))
		graphWidget->addEdge(from,to);

	vertexSelectorComboBox->setCurrentIndex(from);
}

void MainWindow::updateVertexColor(int newColor)
{
	int index = vertexSelectorComboBox->currentIndex();

	if ((index >= 0) && (newColor >= 0))
		graphWidget->setColor(index,newColor);
}
void MainWindow::makeVertexBrowserList()
{
	// any time there is a change, be sure to allow for update analysis
	pushButton_Update_Analysis->setEnabled(true);
	
	vertexSelectorComboBox->clear();	// clears all entries to make it again from scratch

	QString entry = "";
	
	for (int i = 0; i < graphWidget->getNumV(); i++)
	{
		entry = QString("(%1)") .arg (i,0,10);

		QString name = QString::fromStdString(graphWidget->getName(i));

		entry += " " + name;

		vertexSelectorComboBox->addItem(entry);
	}
}
void MainWindow::updateVertexName(QString name)
{

	int index = vertexSelectorComboBox->currentIndex();
	graphWidget->setName(index, name.toStdString());

	makeVertexBrowserList();
	vertexSelectorComboBox->setCurrentIndex(index);

}
void MainWindow::loadVertexData(int index)
{
	// clear the old stuff
	lineEdit_NameEdit->setText("");

	// clear neighbor add/remove combo boxes and reconstruct
	comboBox_RemoveNodeList->clear();
	comboBox_AddNodeList->clear();

	lcdNumber_Degree->display(0);
	lcdNumber_Degree_in->display(0);
	lcdNumber_Degree_out->display(0);

	// make the new stuff
	if (graphWidget->isValidVID(index))
	{
	//	if (index < 0)			// sometimes returns index=-1 on update!
		//	index = 0;

		// load name field
		QString name = QString::fromStdString(graphWidget->getName(index));
		lineEdit_NameEdit->setText(name);
	
		// get color info
		comboBox_ColorSelect->setCurrentIndex(graphWidget->getColor(index));
	

		// get list of outgoing neighbors
		vector<int> neighbors = graphWidget->getOutNeighborIndexList( index);

		// set size
		lcdNumber_Degree->display(graphWidget->getDegree(index));
		lcdNumber_Degree_in->display(graphWidget->getInDegree(index));
		lcdNumber_Degree_out->display(graphWidget->getOutDegree(index));
		lcdNumber_AvgShortestPath->display(graphWidget->getAvgShortestPath(index));
		lcdNumber_PageRank->display(graphWidget->getPageRank(index));
		lcdNumber_ClosenessCentrality->display(graphWidget->getCCentrality(index));
		lcdNumber_ClusteringCoefficient->display(graphWidget->getClusteringCoefficient(index));
	
		int rCounter = 0, aCounter = 0;

		for (int i = 0; i < graphWidget->getNumV(); i++)
		{
			QString entry = QString("(%1)") .arg (i,0,10);
			QString name = QString::fromStdString(graphWidget->getName(i));
			entry += " " + name;

			// is it already connected or not?
			vector<int>::iterator iter;
			iter = std::find(neighbors.begin(), neighbors.end(), i);
		
			if (i != index)		// can't connect to self
			{
				if (iter != neighbors.end())  // add to remove list, since already neighbor
				{
					comboBox_RemoveNodeList->addItem(entry); 		// found the neighbor!
					comboBox_RemoveNodeList->setItemData(rCounter, i, Qt::UserRole);
					rCounter++;
				}
				else		// add to "add" list since not yet neighbors
				{
					comboBox_AddNodeList->addItem(entry);
					comboBox_AddNodeList->setItemData(aCounter, i, Qt::UserRole);
					aCounter++;
				}
			}
		}
	
		// load graph analysis stuff
	}
}
void MainWindow::makeEmptyGraph()
{
     		bool ok;

		QString title = "Create a new empty graph";
		QString message = "Enter number of vertices";
		
     		int numVertices = QInputDialog::getInteger(this, title,
                                      message, 1, 1, 10000, 1, &ok);
     		if (ok)
         		graphWidget->makeEmpty(numVertices); 
}

void MainWindow::makeCompleteGraph()
{
     		bool ok;

		QString title = "Create a new complete graph";
		QString message = "Enter number of vertices";
		
     		int numVertices = QInputDialog::getInteger(this, title,
                                      message, 1, 1, 10000, 1, &ok);
     		if (ok)
         		graphWidget->makeComplete(numVertices); 
}

void MainWindow::makeBinaryTreeGraph()
{
	     	bool ok;

		QString title = "Create a new binary tree graph";
		QString message = "Enter number of generations";
		
     		int generations = QInputDialog::getInteger(this, title,
                                      message, 1, 1, 100, 1, &ok);
     		if (ok)
         		graphWidget->makeBinaryTree(generations); 
}


void MainWindow::addVertex()
{
	bool ok;

	QString title = "Add a vertex";
	QString message = "Enter a name for the new vertex";
	QString defaultText = "(name)";

     QString text = QInputDialog::getText(this, title, message, QLineEdit::Normal,
                                         defaultText , &ok);
	if (text.isEmpty())
		text = defaultText;

     if (ok)
	{
         	graphWidget->addVertex( text.toStdString() );
		vertexSelectorComboBox->setCurrentIndex((int)graphWidget->getNumV() - 1);

	}
}

void MainWindow::addEdge()
{
	bool ok;

	QString title = "Add an edge";
	QString message1 = "Choose the index of the first vertex";
	QString message2 = "Choose the index of the second vertex";

     	int vertex1 = QInputDialog::getInteger(this, title,
                                      message1, 0, 0, graphWidget->getNumV() - 1, 1, &ok);

	int vertex2 = QInputDialog::getInteger(this, title,
                                      message2, 0, 0, graphWidget->getNumV() - 1, 1, &ok);

     if (ok)
         graphWidget->addEdge( vertex1, vertex2 );
}

void MainWindow::resetViewDim()
{
	if (radioButton_3D->isChecked())
	{
		graphWidget->set3DView();
		comboBox_3DLayout->setEnabled(true);
		comboBox_2DLayout->setEnabled(false);
		sliderX->setEnabled(true);
		sliderY->setEnabled(true);
	}
	else if (radioButton_2D->isChecked())
	{
		graphWidget->set2DView();
		comboBox_2DLayout->setEnabled(true);
		comboBox_3DLayout->setEnabled(false);
		sliderX->setEnabled(false);
		sliderY->setEnabled(false);
	}
}
void MainWindow::updatePlotValues()
{
	pair<float,float> xRange = graphWidget->getXRange();
	pair<float,float> yRange = graphWidget->getYRange();
	pair<float,float> zRange = graphWidget->getZRange();
	xMin->setValue(xRange.first); xMax->setValue(xRange.second);
	yMin->setValue(yRange.first); yMax->setValue(yRange.second);
	zMin->setValue(zRange.first); zMax->setValue(zRange.second);
	vertexRadius->setValue(graphWidget->getVertexRadius());

}

void MainWindow::replotGraph()
{
	graphWidget->replot(xMin->value(),xMax->value(),yMin->value(),yMax->value(),zMin->value(),zMax->value(), vertexRadius->value());
}

void MainWindow::makeDialogs(QMainWindow *MainWindow)
{

	randGraphDial = new RandomGraphDialog(MainWindow);
	kregGraphDial = new KRegularGraphDialog(MainWindow);
	strangersbGraphDial = new StrangersBanquetGraphDialog(MainWindow);

}

void MainWindow::setupUi(QMainWindow *MainWindow)
    {
 if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(1100, 750);
    MainWindow->setMinimumSize(QSize(1100, 750));
    MainWindow->setMaximumSize(QSize(1100, 750));
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/images/babelgraph_logo_iconsize.png")), QIcon::Normal, QIcon::Off);
    MainWindow->setWindowIcon(icon);
    actionLoad_Graph = new QAction(MainWindow);
    actionLoad_Graph->setObjectName(QString::fromUtf8("actionLoad_Graph"));
    actionEdge_Data = new QAction(MainWindow);
    actionEdge_Data->setObjectName(QString::fromUtf8("actionEdge_Data"));
    actionAverage_Path_Length = new QAction(MainWindow);
    actionAverage_Path_Length->setObjectName(QString::fromUtf8("actionAverage_Path_Length"));
    actionExit = new QAction(MainWindow);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
    actionPreferences = new QAction(MainWindow);
    actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
    actionBabelGraph_Help = new QAction(MainWindow);
    actionBabelGraph_Help->setObjectName(QString::fromUtf8("actionBabelGraph_Help"));
    actionAbout_BabelGraph = new QAction(MainWindow);
    actionAbout_BabelGraph->setObjectName(QString::fromUtf8("actionAbout_BabelGraph"));
    actionEdit_matrix = new QAction(MainWindow);
    actionEdit_matrix->setObjectName(QString::fromUtf8("actionEdit_matrix"));
    actionExport_Image = new QAction(MainWindow);
    actionExport_Image->setObjectName(QString::fromUtf8("actionExport_Image"));
    actionClustering = new QAction(MainWindow);
    actionClustering->setObjectName(QString::fromUtf8("actionClustering"));
    actionSave_Graph = new QAction(MainWindow);
    actionSave_Graph->setObjectName(QString::fromUtf8("actionSave_Graph"));
    actionDiameter = new QAction(MainWindow);
    actionDiameter->setObjectName(QString::fromUtf8("actionDiameter"));
    actionCentrality = new QAction(MainWindow);
    actionCentrality->setObjectName(QString::fromUtf8("actionCentrality"));
    actionPath_Length = new QAction(MainWindow);
    actionPath_Length->setObjectName(QString::fromUtf8("actionPath_Length"));
    actionClustering_2 = new QAction(MainWindow);
    actionClustering_2->setObjectName(QString::fromUtf8("actionClustering_2"));
    actionDegree_Distribution = new QAction(MainWindow);
    actionDegree_Distribution->setObjectName(QString::fromUtf8("actionDegree_Distribution"));
    actionEmpty = new QAction(MainWindow);
    actionEmpty->setObjectName(QString::fromUtf8("actionEmpty"));
    actionRandom = new QAction(MainWindow);
    actionRandom->setObjectName(QString::fromUtf8("actionRandom"));
    actionK_Regular = new QAction(MainWindow);
    actionK_Regular->setObjectName(QString::fromUtf8("actionK_Regular"));
    actionComplete = new QAction(MainWindow);
    actionComplete->setObjectName(QString::fromUtf8("actionComplete"));
    actionSmall_World = new QAction(MainWindow);
    actionSmall_World->setObjectName(QString::fromUtf8("actionSmall_World"));
    actionBinary_Tree = new QAction(MainWindow);
    actionBinary_Tree->setObjectName(QString::fromUtf8("actionBinary_Tree"));
    actionSave_Graph_As = new QAction(MainWindow);
    actionSave_Graph_As->setObjectName(QString::fromUtf8("actionSave_Graph_As"));
    actionEmpty_2 = new QAction(MainWindow);
    actionEmpty_2->setObjectName(QString::fromUtf8("actionEmpty_2"));
    actionRandom_2 = new QAction(MainWindow);
    actionRandom_2->setObjectName(QString::fromUtf8("actionRandom_2"));
    actionK_Regular_2 = new QAction(MainWindow);
    actionK_Regular_2->setObjectName(QString::fromUtf8("actionK_Regular_2"));
    actionBinary_Tree_2 = new QAction(MainWindow);
    actionBinary_Tree_2->setObjectName(QString::fromUtf8("actionBinary_Tree_2"));
    actionComplete_2 = new QAction(MainWindow);
    actionComplete_2->setObjectName(QString::fromUtf8("actionComplete_2"));
    actionStrangers_Banquet = new QAction(MainWindow);
    actionStrangers_Banquet->setObjectName(QString::fromUtf8("actionStrangers_Banquet"));
    actionNew_Graph = new QAction(MainWindow);
    actionNew_Graph->setObjectName(QString::fromUtf8("actionNew_Graph"));
    actionImport_Graph = new QAction(MainWindow);
    actionImport_Graph->setObjectName(QString::fromUtf8("actionImport_Graph"));
    actionExport_Graph = new QAction(MainWindow);
    actionExport_Graph->setObjectName(QString::fromUtf8("actionExport_Graph"));
    actionSet_Background_Color = new QAction(MainWindow);
    actionSet_Background_Color->setObjectName(QString::fromUtf8("actionSet_Background_Color"));
    actionShow_Vertex_Names = new QAction(MainWindow);
    actionShow_Vertex_Names->setObjectName(QString::fromUtf8("actionShow_Vertex_Names"));
    actionShow_Vertex_Indices = new QAction(MainWindow);
    actionShow_Vertex_Indices->setObjectName(QString::fromUtf8("actionShow_Vertex_Indices"));
    actionCleanLooks = new QAction(MainWindow);
    actionCleanLooks->setObjectName(QString::fromUtf8("actionCleanLooks"));
    actionWindows = new QAction(MainWindow);
    actionWindows->setObjectName(QString::fromUtf8("actionWindows"));
    actionPlastique = new QAction(MainWindow);
    actionPlastique->setObjectName(QString::fromUtf8("actionPlastique"));
    actionMotif = new QAction(MainWindow);
    actionMotif->setObjectName(QString::fromUtf8("actionMotif"));
    actionCDE = new QAction(MainWindow);
    actionCDE->setObjectName(QString::fromUtf8("actionCDE"));


    actionMake_Undirected = new QAction(MainWindow);
    actionMake_Undirected->setObjectName(QString::fromUtf8("actionMake_Undirected"));

    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    centralwidget->setGeometry(QRect(0, 26, 1000, 674));
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
    centralwidget->setSizePolicy(sizePolicy);
    horizontalLayoutWidget_6 = new QWidget(centralwidget);
    horizontalLayoutWidget_6->setObjectName(QString::fromUtf8("horizontalLayoutWidget_6"));
    horizontalLayoutWidget_6->setGeometry(QRect(10, 10, 871, 31));
    top_button_bar = new QHBoxLayout(horizontalLayoutWidget_6);
    top_button_bar->setObjectName(QString::fromUtf8("top_button_bar"));
    top_button_bar->setSizeConstraint(QLayout::SetNoConstraint);
    top_button_bar->setContentsMargins(0, 0, 0, 0);
    addVertexButton = new QPushButton(horizontalLayoutWidget_6);
    addVertexButton->setObjectName(QString::fromUtf8("addVertexButton"));

    top_button_bar->addWidget(addVertexButton);

    addEdgeButton = new QPushButton(horizontalLayoutWidget_6);
    addEdgeButton->setObjectName(QString::fromUtf8("addEdgeButton"));

    top_button_bar->addWidget(addEdgeButton);

 zoomInButton = new QPushButton(horizontalLayoutWidget_6);
    zoomInButton->setObjectName(QString::fromUtf8("zoomInButton"));
    zoomInButton->setMinimumSize(QSize(0, 28));
 

    top_button_bar->addWidget(zoomInButton);

  	zoomOutButton = new QPushButton(horizontalLayoutWidget_6);
    zoomOutButton->setObjectName(QString::fromUtf8("zoomOutButton"));
    zoomOutButton->setMinimumSize(QSize(0, 28));


    top_button_bar->addWidget(zoomOutButton);
    toggleAxesButton = new QPushButton(horizontalLayoutWidget_6);
    toggleAxesButton->setObjectName(QString::fromUtf8("toggleAxesButton"));
    toggleAxesButton->setMinimumSize(QSize(0, 28));

    top_button_bar->addWidget(toggleAxesButton);

    resetViewButton = new QPushButton(horizontalLayoutWidget_6);
    resetViewButton->setObjectName(QString::fromUtf8("resetViewButton"));
    resetViewButton->setMinimumSize(QSize(0, 28));

    top_button_bar->addWidget(resetViewButton);

       sliderZ = new QSlider(centralwidget);
    sliderZ->setObjectName(QString::fromUtf8("sliderZ"));
    sliderZ->setGeometry(QRect(30, 700, 781, 20));
    sliderZ->setOrientation(Qt::Horizontal);
    sliderZ->setTickPosition(QSlider::TicksAbove);
	sliderZ->setRange(0, 360 * 16);
     	sliderZ->setSingleStep(16);
     	sliderZ->setPageStep(15 * 16);
     	sliderZ->setTickInterval(15 * 16);
    sliderY = new QSlider(centralwidget);
    sliderY->setObjectName(QString::fromUtf8("sliderY"));
    sliderY->setGeometry(QRect(820, 50, 20, 641));
    sliderY->setOrientation(Qt::Vertical);
    sliderY->setTickPosition(QSlider::TicksAbove);
    sliderY->setTickInterval(0);
	sliderY->setRange(0, 360 * 16);
     	sliderY->setSingleStep(16);
     	sliderY->setPageStep(15 * 16);
     	sliderY->setTickInterval(15 * 16);
    sliderX = new QSlider(centralwidget);
    sliderX->setObjectName(QString::fromUtf8("sliderX"));
    sliderX->setGeometry(QRect(0, 50, 20, 631));
    sliderX->setOrientation(Qt::Vertical);
    sliderX->setTickPosition(QSlider::TicksBelow);
	sliderX->setRange(0, 360 * 16);
     	sliderX->setSingleStep(16);
     	sliderX->setPageStep(15 * 16 );
     	sliderX->setTickInterval(15 * 16 );
    graphWidget = new GraphWidget(centralwidget);
    graphWidget->setObjectName(QString::fromUtf8("graphWidget"));
    graphWidget->setGeometry(QRect(29, 49, 781, 641));
    SideBar = new QTabWidget(centralwidget);
    SideBar->setObjectName(QString::fromUtf8("SideBar"));
    SideBar->setWindowModality(Qt::NonModal);
    SideBar->setEnabled(true);
    SideBar->setGeometry(QRect(840, 51, 250, 650));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(SideBar->sizePolicy().hasHeightForWidth());
    SideBar->setSizePolicy(sizePolicy1);
    SideBar->setMinimumSize(QSize(250, 650));
    SideBar->setMaximumSize(QSize(250, 650));
    SideBar->setAutoFillBackground(false);
    SideBar->setTabShape(QTabWidget::Rounded);
    SideBar->setElideMode(Qt::ElideNone);
    tab_browse_edit = new QWidget();
    tab_browse_edit->setObjectName(QString::fromUtf8("tab_browse_edit"));
    tab_browse_edit->setGeometry(QRect(0, 0, 246, 622));
    groupBox_Properties = new QGroupBox(tab_browse_edit);
    groupBox_Properties->setObjectName(QString::fromUtf8("groupBox_Properties"));
    groupBox_Properties->setGeometry(QRect(10, 50, 231, 101));
    formLayoutWidget = new QWidget(groupBox_Properties);
    formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
    formLayoutWidget->setGeometry(QRect(10, 20, 211, 71));
    formLayout = new QFormLayout(formLayoutWidget);
    formLayout->setObjectName(QString::fromUtf8("formLayout"));
    formLayout->setContentsMargins(0, 0, 0, 0);
    label_Name = new QLabel(formLayoutWidget);
    label_Name->setObjectName(QString::fromUtf8("label_Name"));

    formLayout->setWidget(0, QFormLayout::LabelRole, label_Name);

    lineEdit_NameEdit = new QLineEdit(formLayoutWidget);
    lineEdit_NameEdit->setObjectName(QString::fromUtf8("lineEdit_NameEdit"));

    formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_NameEdit);

    label_Color = new QLabel(formLayoutWidget);
    label_Color->setObjectName(QString::fromUtf8("label_Color"));

    formLayout->setWidget(1, QFormLayout::LabelRole, label_Color);

    comboBox_ColorSelect = new QComboBox(formLayoutWidget);
    comboBox_ColorSelect->setObjectName(QString::fromUtf8("comboBox_ColorSelect"));
    comboBox_ColorSelect->setMinimumSize(QSize(100, 0));

    formLayout->setWidget(1, QFormLayout::FieldRole, comboBox_ColorSelect);

    groupBox_Neighbors = new QGroupBox(tab_browse_edit);
    groupBox_Neighbors->setObjectName(QString::fromUtf8("groupBox_Neighbors"));
    groupBox_Neighbors->setGeometry(QRect(10, 160, 231, 101));
    formLayoutWidget_2 = new QWidget(groupBox_Neighbors);
    formLayoutWidget_2->setObjectName(QString::fromUtf8("formLayoutWidget_2"));
    formLayoutWidget_2->setGeometry(QRect(10, 20, 211, 80));
    formLayout_2 = new QFormLayout(formLayoutWidget_2);
    formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
    formLayout_2->setContentsMargins(0, 0, 0, 0);
    comboBox_AddNodeList = new QComboBox(formLayoutWidget_2);
    comboBox_AddNodeList->setObjectName(QString::fromUtf8("comboBox_AddNodeList"));
    comboBox_AddNodeList->setMinimumSize(QSize(152, 28));
    comboBox_AddNodeList->setMaximumSize(QSize(152, 28));

    formLayout_2->setWidget(0, QFormLayout::FieldRole, comboBox_AddNodeList);

    comboBox_RemoveNodeList = new QComboBox(formLayoutWidget_2);
    comboBox_RemoveNodeList->setObjectName(QString::fromUtf8("comboBox_RemoveNodeList"));
    comboBox_RemoveNodeList->setMinimumSize(QSize(152, 28));
    comboBox_RemoveNodeList->setMaximumSize(QSize(152, 28));

    formLayout_2->setWidget(1, QFormLayout::FieldRole, comboBox_RemoveNodeList);

   addNeighborButton = new QPushButton(formLayoutWidget_2);
    addNeighborButton->setObjectName(QString::fromUtf8("addNeighborButton"));
    QIcon icon3;
    icon3.addPixmap(QPixmap(QString::fromUtf8(":/images/edit_add.png")), QIcon::Normal, QIcon::Off);
    addNeighborButton->setIcon(icon3);

    formLayout_2->setWidget(0, QFormLayout::LabelRole, addNeighborButton);

    removeNeighborButton = new QPushButton(formLayoutWidget_2);
    removeNeighborButton->setObjectName(QString::fromUtf8("removeNeighborButton"));
    QIcon icon4;
    icon4.addPixmap(QPixmap(QString::fromUtf8(":/images/edit_remove.png")), QIcon::Normal, QIcon::Off);
    removeNeighborButton->setIcon(icon4);
    formLayout_2->setWidget(1, QFormLayout::LabelRole, removeNeighborButton);

        groupBox_Analysis = new QGroupBox(tab_browse_edit);
    groupBox_Analysis->setObjectName(QString::fromUtf8("groupBox_Analysis"));
    groupBox_Analysis->setGeometry(QRect(10, 270, 231, 271));
    formLayoutWidget_3 = new QWidget(groupBox_Analysis);
    formLayoutWidget_3->setObjectName(QString::fromUtf8("formLayoutWidget_3"));
    formLayoutWidget_3->setGeometry(QRect(10, 20, 211, 241));
    formLayout_3 = new QFormLayout(formLayoutWidget_3);
    formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
    formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout_3->setContentsMargins(0, 0, 0, 0);
    label_Degree = new QLabel(formLayoutWidget_3);
    label_Degree->setObjectName(QString::fromUtf8("label_Degree"));

    formLayout_3->setWidget(0, QFormLayout::LabelRole, label_Degree);

    lcdNumber_Degree = new QLCDNumber(formLayoutWidget_3);
    lcdNumber_Degree->setObjectName(QString::fromUtf8("lcdNumber_Degree"));
	lcdNumber_Degree->setSegmentStyle(QLCDNumber::Flat);

    formLayout_3->setWidget(0, QFormLayout::FieldRole, lcdNumber_Degree);

    label_Degree_in = new QLabel(formLayoutWidget_3);
    label_Degree_in->setObjectName(QString::fromUtf8("label_Degree_in"));

    formLayout_3->setWidget(1, QFormLayout::LabelRole, label_Degree_in);

    lcdNumber_Degree_in = new QLCDNumber(formLayoutWidget_3);
    lcdNumber_Degree_in->setObjectName(QString::fromUtf8("lcdNumber_Degree_in"));
	lcdNumber_Degree_in->setSegmentStyle(QLCDNumber::Flat);

    formLayout_3->setWidget(1, QFormLayout::FieldRole, lcdNumber_Degree_in);

    label_Degree_out = new QLabel(formLayoutWidget_3);
    label_Degree_out->setObjectName(QString::fromUtf8("label_Degree_out"));

    formLayout_3->setWidget(2, QFormLayout::LabelRole, label_Degree_out);

    lcdNumber_Degree_out = new QLCDNumber(formLayoutWidget_3);
    lcdNumber_Degree_out->setObjectName(QString::fromUtf8("lcdNumber_Degree_out"));
	lcdNumber_Degree_out->setSegmentStyle(QLCDNumber::Flat);

    formLayout_3->setWidget(2, QFormLayout::FieldRole, lcdNumber_Degree_out);

    label_AvgShortestPath = new QLabel(formLayoutWidget_3);
    label_AvgShortestPath->setObjectName(QString::fromUtf8("label_AvgShortestPath"));

    formLayout_3->setWidget(3, QFormLayout::LabelRole, label_AvgShortestPath);

    lcdNumber_AvgShortestPath = new QLCDNumber(formLayoutWidget_3);
    lcdNumber_AvgShortestPath->setObjectName(QString::fromUtf8("lcdNumber_AvgShortestPath"));
	lcdNumber_AvgShortestPath->setSegmentStyle(QLCDNumber::Flat);

    formLayout_3->setWidget(3, QFormLayout::FieldRole, lcdNumber_AvgShortestPath);

    label_PageRank = new QLabel(formLayoutWidget_3);
    label_PageRank->setObjectName(QString::fromUtf8("label_PageRank"));

    formLayout_3->setWidget(4, QFormLayout::LabelRole, label_PageRank);

    lcdNumber_PageRank = new QLCDNumber(formLayoutWidget_3);
    lcdNumber_PageRank->setObjectName(QString::fromUtf8("lcdNumber_PageRank"));
	lcdNumber_PageRank->setSegmentStyle(QLCDNumber::Flat);

    formLayout_3->setWidget(4, QFormLayout::FieldRole, lcdNumber_PageRank);

    label_CloesnessCentrality = new QLabel(formLayoutWidget_3);
    label_CloesnessCentrality->setObjectName(QString::fromUtf8("label_CloesnessCentrality"));

    formLayout_3->setWidget(5, QFormLayout::LabelRole, label_CloesnessCentrality);

    label_ClusteringCoefficient = new QLabel(formLayoutWidget_3);
    label_ClusteringCoefficient->setObjectName(QString::fromUtf8("label_ClusteringCoefficient"));

    formLayout_3->setWidget(6, QFormLayout::LabelRole, label_ClusteringCoefficient);

    lcdNumber_ClosenessCentrality = new QLCDNumber(formLayoutWidget_3);
    lcdNumber_ClosenessCentrality->setObjectName(QString::fromUtf8("lcdNumber_ClosenessCentrality"));
	lcdNumber_ClosenessCentrality->setSegmentStyle(QLCDNumber::Flat);

    formLayout_3->setWidget(5, QFormLayout::FieldRole, lcdNumber_ClosenessCentrality);

    lcdNumber_ClusteringCoefficient = new QLCDNumber(formLayoutWidget_3);
    lcdNumber_ClusteringCoefficient->setObjectName(QString::fromUtf8("lcdNumber_ClusteringCoefficient"));
	lcdNumber_ClusteringCoefficient->setSegmentStyle(QLCDNumber::Flat);

    formLayout_3->setWidget(6, QFormLayout::FieldRole, lcdNumber_ClusteringCoefficient);


    gridLayoutWidget = new QWidget(tab_browse_edit);
    gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
    gridLayoutWidget->setGeometry(QRect(10, 10, 231, 34));
    gridLayout = new QGridLayout(gridLayoutWidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
    gridLayout->setHorizontalSpacing(8);
    gridLayout->setContentsMargins(4, 0, 4, 0);
    vertexSelectorComboBox = new QComboBox(gridLayoutWidget);
    vertexSelectorComboBox->setObjectName(QString::fromUtf8("vertexSelectorComboBox"));
    vertexSelectorComboBox->setMinimumSize(QSize(100, 0));
	vertexSelectorComboBox->setMaxVisibleItems(5);

    gridLayout->addWidget(vertexSelectorComboBox, 0, 1, 1, 1);

    zoomToVertex = new QPushButton(gridLayoutWidget);
    zoomToVertex->setObjectName(QString::fromUtf8("zoomToVertex"));
    zoomToVertex->setMaximumSize(QSize(30, 16777215));
    QIcon icon5;
    icon5.addPixmap(QPixmap(QString::fromUtf8(":/images/find.png")), QIcon::Normal, QIcon::Off);
    zoomToVertex->setIcon(icon5);

	gridLayout->addWidget(zoomToVertex, 0, 2, 1, 1);

    label_Vertex = new QLabel(gridLayoutWidget);
    label_Vertex->setObjectName(QString::fromUtf8("label_Vertex"));
    label_Vertex->setMaximumSize(QSize(39, 32));

    gridLayout->addWidget(label_Vertex, 0, 0, 1, 1);

    pushButton_Update_Analysis = new QPushButton(tab_browse_edit);
    pushButton_Update_Analysis->setObjectName(QString::fromUtf8("pushButton_Update_Analysis"));
    pushButton_Update_Analysis->setGeometry(QRect(60, 550, 101, 28));
    pushButton_Update_Analysis->setMinimumSize(QSize(130,28));	
pushButton_Update_Analysis->setEnabled(false);


    SideBar->addTab(tab_browse_edit, QString());
    tab_layout = new QWidget();
    tab_layout->setObjectName(QString::fromUtf8("tab_layout"));
    tab_layout->setGeometry(QRect(0, 0, 246, 562));
    layoutOptionsGroup = new QGroupBox(tab_layout);
    layoutOptionsGroup->setObjectName(QString::fromUtf8("layoutOptionsGroup"));
    layoutOptionsGroup->setGeometry(QRect(10, 0, 229, 281));
    verticalLayoutWidget = new QWidget(layoutOptionsGroup);
    verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
    verticalLayoutWidget->setGeometry(QRect(10, 19, 211, 252));
    verticalLayout = new QVBoxLayout(verticalLayoutWidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    radioButton_3D = new QRadioButton(verticalLayoutWidget);
    radioButton_3D->setObjectName(QString::fromUtf8("radioButton_3D"));
    radioButton_3D->setChecked(true);

    verticalLayout->addWidget(radioButton_3D);

    comboBox_3DLayout = new QComboBox(verticalLayoutWidget);
    comboBox_3DLayout->setObjectName(QString::fromUtf8("comboBox_3DLayout"));

    verticalLayout->addWidget(comboBox_3DLayout);

    radioButton_2D = new QRadioButton(verticalLayoutWidget);
    radioButton_2D->setObjectName(QString::fromUtf8("radioButton_2D"));

    verticalLayout->addWidget(radioButton_2D);

    comboBox_2DLayout = new QComboBox(verticalLayoutWidget);
    comboBox_2DLayout->setObjectName(QString::fromUtf8("comboBox_2DLayout"));

    verticalLayout->addWidget(comboBox_2DLayout);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label = new QLabel(verticalLayoutWidget);
    label->setObjectName(QString::fromUtf8("label"));
    label->setMaximumSize(QSize(41, 20));

    horizontalLayout_2->addWidget(label);


    verticalLayout->addLayout(horizontalLayout_2);

    	horizontalSlider_Spring = new QSlider(verticalLayoutWidget);
    	horizontalSlider_Spring->setObjectName(QString::fromUtf8("horizontalSlider_Spring"));
   	horizontalSlider_Spring->setOrientation(Qt::Horizontal);
	horizontalSlider_Spring->setTickPosition(QSlider::TicksAbove);
	horizontalSlider_Spring->setRange(1, 1000);
     	horizontalSlider_Spring->setSingleStep(10);
     	horizontalSlider_Spring->setPageStep(100);
     	horizontalSlider_Spring->setTickInterval(100);	
	horizontalSlider_Spring->setValue(500);

    verticalLayout->addWidget(horizontalSlider_Spring);

    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    label_2 = new QLabel(verticalLayoutWidget);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setMaximumSize(QSize(61, 22));

    horizontalLayout_3->addWidget(label_2);


    verticalLayout->addLayout(horizontalLayout_3);

    horizontalSlider_Repulsion = new QSlider(verticalLayoutWidget);
    horizontalSlider_Repulsion->setObjectName(QString::fromUtf8("horizontalSlider_Repulsion"));
    horizontalSlider_Repulsion->setOrientation(Qt::Horizontal);
	horizontalSlider_Repulsion->setTickPosition(QSlider::TicksAbove);
	horizontalSlider_Repulsion->setRange(1, 1000);
     	horizontalSlider_Repulsion->setSingleStep(10);
     	horizontalSlider_Repulsion->setPageStep(100);
     	horizontalSlider_Repulsion->setTickInterval(100);	
	horizontalSlider_Repulsion->setValue(500);

    verticalLayout->addWidget(horizontalSlider_Repulsion);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    toolButton_AutoArrange = new QToolButton(verticalLayoutWidget);
    toolButton_AutoArrange->setObjectName(QString::fromUtf8("toolButton_AutoArrange"));
    toolButton_AutoArrange->setCheckable(true);

    horizontalLayout->addWidget(toolButton_AutoArrange);


    verticalLayout->addLayout(horizontalLayout);

    plottingOptionsGroup = new QGroupBox(tab_layout);
    plottingOptionsGroup->setObjectName(QString::fromUtf8("plottingOptionsGroup"));
    plottingOptionsGroup->setGeometry(QRect(10, 290, 229, 261));
    gridLayoutWidget_2 = new QWidget(plottingOptionsGroup);
    gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
    gridLayoutWidget_2->setGeometry(QRect(10, 20, 211, 201));
    gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout_2->setContentsMargins(0, 0, 0, 0);
    labelXRange = new QLabel(gridLayoutWidget_2);
    labelXRange->setObjectName(QString::fromUtf8("labelXRange"));

    gridLayout_2->addWidget(labelXRange, 0, 0, 1, 1);

    xMin = new QDoubleSpinBox(gridLayoutWidget_2);
    xMin->setObjectName(QString::fromUtf8("xMin"));
	xMin->setMinimum(-1000);
	xMin->setMaximum(0);
	xMin->setValue(-1);

    gridLayout_2->addWidget(xMin, 0, 1, 1, 1);

    yMin = new QDoubleSpinBox(gridLayoutWidget_2);
    yMin->setObjectName(QString::fromUtf8("yMin"));
	yMin->setMinimum(-1000);
	yMin->setMaximum(0);
	yMin->setValue(-1);

    gridLayout_2->addWidget(yMin, 1, 1, 1, 1);

    labelYRange = new QLabel(gridLayoutWidget_2);
    labelYRange->setObjectName(QString::fromUtf8("labelYRange"));

    gridLayout_2->addWidget(labelYRange, 1, 0, 1, 1);

    yMax = new QDoubleSpinBox(gridLayoutWidget_2);
    yMax->setObjectName(QString::fromUtf8("yMax"));
	yMax->setMinimum(0);
	yMax->setMaximum(1000);
	yMax->setValue(1);

    gridLayout_2->addWidget(yMax, 1, 2, 1, 1);

    xMax = new QDoubleSpinBox(gridLayoutWidget_2);
    xMax->setObjectName(QString::fromUtf8("xMax"));
	xMax->setMinimum(0);
	xMax->setMaximum(1000);
	xMax->setValue(1);

    gridLayout_2->addWidget(xMax, 0, 2, 1, 1);

    zMin = new QDoubleSpinBox(gridLayoutWidget_2);
    zMin->setObjectName(QString::fromUtf8("zMin"));
	zMin->setMinimum(-1000);
	zMin->setMaximum(0);
	zMin->setValue(-1);

    gridLayout_2->addWidget(zMin, 2, 1, 1, 1);

    zMax = new QDoubleSpinBox(gridLayoutWidget_2);
    zMax->setObjectName(QString::fromUtf8("zMax"));
	zMax->setMinimum(0);
	zMax->setMaximum(1000);
	zMax->setValue(1);

    gridLayout_2->addWidget(zMax, 2, 2, 1, 1);

    labelZRange = new QLabel(gridLayoutWidget_2);
    labelZRange->setObjectName(QString::fromUtf8("labelZRange"));

    gridLayout_2->addWidget(labelZRange, 2, 0, 1, 1);

    labelVertexRadius = new QLabel(gridLayoutWidget_2);
    labelVertexRadius->setObjectName(QString::fromUtf8("labelVertexRadius"));

    gridLayout_2->addWidget(labelVertexRadius, 3, 0, 1, 1);

    vertexRadius = new QDoubleSpinBox(gridLayoutWidget_2);
    vertexRadius->setObjectName(QString::fromUtf8("vertexRadius"));
	vertexRadius->setMinimum(0.01);
	vertexRadius->setMaximum(10);
	vertexRadius->setValue(.05);
	vertexRadius->setSingleStep(.01);

    gridLayout_2->addWidget(vertexRadius, 3, 1, 1, 1);


    button_Replot = new QPushButton(plottingOptionsGroup);
    button_Replot->setObjectName(QString::fromUtf8("button_Replot"));
    button_Replot->setGeometry(QRect(130, 220, 80, 28));
    button_Replot->setMaximumSize(QSize(80, 28));
    button_Default_Plot = new QPushButton(plottingOptionsGroup);
    button_Default_Plot->setObjectName(QString::fromUtf8("button_Default_Plot"));
    button_Default_Plot->setGeometry(QRect(30, 220, 80, 28));
    button_Default_Plot->setMaximumSize(QSize(80, 28));

    SideBar->addTab(tab_layout, QString());
    /*
	Nothing here yet, so comment out... maybe in the next release...
	tab_analysis = new QWidget();
    tab_analysis->setObjectName(QString::fromUtf8("tab_analysis"));
    tab_analysis->setGeometry(QRect(0, 0, 246, 562));
    SideBar->addTab(tab_analysis, QString());*/
    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 1000, 26));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuHelp = new QMenu(menubar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    /*
	Nothing here yet, so comment out... maybe in the next release...
	menuAnalysis = new QMenu(menubar);
    menuAnalysis->setObjectName(QString::fromUtf8("menuAnalysis"));
    menuGraph_Data = new QMenu(menuAnalysis);
    menuGraph_Data->setObjectName(QString::fromUtf8("menuGraph_Data"));
    menuVertex_Data = new QMenu(menuAnalysis);
    menuVertex_Data->setObjectName(QString::fromUtf8("menuVertex_Data"));
	NB->DONT FORGET TO UNCOMMENT THE MENU ACTIONS BELOW!, and the RETRANSLATE AT THE BOTTOM*/
    menuGraph = new QMenu(menubar);
    menuGraph->setObjectName(QString::fromUtf8("menuGraph"));
    menuGenerate_Graph = new QMenu(menuGraph);
    menuGenerate_Graph->setObjectName(QString::fromUtf8("menuGenerate_Graph"));

    menuView = new QMenu(menubar);
    menuView->setObjectName(QString::fromUtf8("menuView"));
    menuSet_Window_Style = new QMenu(menuView);
    menuSet_Window_Style->setObjectName(QString::fromUtf8("menuSet_Window_Style"));

    MainWindow->setMenuBar(menubar);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(menuGraph->menuAction());
    //menubar->addAction(menuAnalysis->menuAction());
	menubar->addAction(menuView->menuAction());
    menubar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionNew_Graph);
    menuFile->addAction(actionSave_Graph);
    //menuFile->addAction(actionSave_Graph_As);
    menuFile->addAction(actionLoad_Graph);
    menuFile->addSeparator();
    menuFile->addAction(actionExport_Image);
    //menuFile->addAction(actionExport_Graph);
    //menuFile->addAction(actionImport_Graph);
    menuFile->addSeparator();
    //menuFile->addAction(actionPreferences);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);
    menuHelp->addAction(actionBabelGraph_Help);
    menuHelp->addAction(actionAbout_BabelGraph);
    /*menuAnalysis->addAction(menuGraph_Data->menuAction());
    menuAnalysis->addAction(menuVertex_Data->menuAction());
    menuAnalysis->addAction(actionEdge_Data);
    menuGraph_Data->addAction(actionClustering);
    menuGraph_Data->addAction(actionDegree_Distribution);
    menuGraph_Data->addAction(actionDiameter);
    menuGraph_Data->addAction(actionAverage_Path_Length);
    menuVertex_Data->addAction(actionCentrality);
    menuVertex_Data->addAction(actionPath_Length);
    menuVertex_Data->addAction(actionClustering_2); */
    menuGraph->addAction(menuGenerate_Graph->menuAction());
	//menuGraph->addAction(actionEdit_matrix); <-- wait for next release
    menuGraph->addAction(actionMake_Undirected);
    menuGenerate_Graph->addAction(actionEmpty_2);
    menuGenerate_Graph->addAction(actionRandom_2);
    menuGenerate_Graph->addAction(actionK_Regular_2);
    menuGenerate_Graph->addAction(actionBinary_Tree_2);
    menuGenerate_Graph->addAction(actionComplete_2);
    menuGenerate_Graph->addAction(actionStrangers_Banquet);

    menuView->addAction(actionSet_Background_Color);
    //menuView->addAction(actionShow_Vertex_Names);
    //menuView->addAction(actionShow_Vertex_Indices);
    menuView->addAction(menuSet_Window_Style->menuAction());
    menuSet_Window_Style->addAction(actionCleanLooks);
    menuSet_Window_Style->addAction(actionWindows);
    menuSet_Window_Style->addAction(actionPlastique);
    menuSet_Window_Style->addAction(actionMotif);
    menuSet_Window_Style->addAction(actionCDE);


    retranslateUi(MainWindow);

    SideBar->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi


    void MainWindow::retranslateUi(QMainWindow *MainWindow)
    {
 MainWindow->setWindowTitle(QApplication::translate("MainWindow", "BabelGraph 0.2", 0, QApplication::UnicodeUTF8));
    actionLoad_Graph->setText(QApplication::translate("MainWindow", "Load Graph", 0, QApplication::UnicodeUTF8));
	actionLoad_Graph->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
    //actionEdge_Data->setText(QApplication::translate("MainWindow", "Edge Data", 0, QApplication::UnicodeUTF8));
    actionAverage_Path_Length->setText(QApplication::translate("MainWindow", "Path Length", 0, QApplication::UnicodeUTF8));
    actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
    actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
    //actionPreferences->setText(QApplication::translate("MainWindow", "Preferences", 0, QApplication::UnicodeUTF8));
    actionBabelGraph_Help->setText(QApplication::translate("MainWindow", "BabelGraph Help", 0, QApplication::UnicodeUTF8));
    actionAbout_BabelGraph->setText(QApplication::translate("MainWindow", "About BabelGraph", 0, QApplication::UnicodeUTF8));
    actionExport_Image->setText(QApplication::translate("MainWindow", "Export Image", 0, QApplication::UnicodeUTF8));
    actionClustering->setText(QApplication::translate("MainWindow", "Clustering", 0, QApplication::UnicodeUTF8));
    actionSave_Graph->setText(QApplication::translate("MainWindow", "Save Graph", 0, QApplication::UnicodeUTF8));
    actionSave_Graph->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
    //actionEdit_matrix->setText(QApplication::translate("MainWindow", "Edit matrix", 0, QApplication::UnicodeUTF8));
    actionDiameter->setText(QApplication::translate("MainWindow", "Diameter", 0, QApplication::UnicodeUTF8));
    actionCentrality->setText(QApplication::translate("MainWindow", "Centrality", 0, QApplication::UnicodeUTF8));
    actionPath_Length->setText(QApplication::translate("MainWindow", "Path Length", 0, QApplication::UnicodeUTF8));
    actionClustering_2->setText(QApplication::translate("MainWindow", "Clustering", 0, QApplication::UnicodeUTF8));
    label_ClusteringCoefficient->setText(QApplication::translate("MainWindow", "Clustering Coefficient", 0, QApplication::UnicodeUTF8));
    actionDegree_Distribution->setText(QApplication::translate("MainWindow", "Degree Distribution", 0, QApplication::UnicodeUTF8));
    actionEmpty->setText(QApplication::translate("MainWindow", "Empty", 0, QApplication::UnicodeUTF8));
    actionRandom->setText(QApplication::translate("MainWindow", "Random", 0, QApplication::UnicodeUTF8));
    actionK_Regular->setText(QApplication::translate("MainWindow", "k-Regular", 0, QApplication::UnicodeUTF8));
    actionComplete->setText(QApplication::translate("MainWindow", "Complete", 0, QApplication::UnicodeUTF8));
    actionSmall_World->setText(QApplication::translate("MainWindow", "Strangers' Banquet", 0, QApplication::UnicodeUTF8));
    actionBinary_Tree->setText(QApplication::translate("MainWindow", "Binary Tree", 0, QApplication::UnicodeUTF8));
   // actionSave_Graph_As->setText(QApplication::translate("MainWindow", "Save Graph As", 0, QApplication::UnicodeUTF8));
    actionEmpty_2->setText(QApplication::translate("MainWindow", "Empty", 0, QApplication::UnicodeUTF8));
    actionRandom_2->setText(QApplication::translate("MainWindow", "Random", 0, QApplication::UnicodeUTF8));
    actionK_Regular_2->setText(QApplication::translate("MainWindow", "k-Regular", 0, QApplication::UnicodeUTF8));
    actionBinary_Tree_2->setText(QApplication::translate("MainWindow", "Binary Tree", 0, QApplication::UnicodeUTF8));
    actionComplete_2->setText(QApplication::translate("MainWindow", "Complete", 0, QApplication::UnicodeUTF8));
    actionStrangers_Banquet->setText(QApplication::translate("MainWindow", "Strangers' Banquet", 0, QApplication::UnicodeUTF8));
    actionNew_Graph->setText(QApplication::translate("MainWindow", "New Graph", 0, QApplication::UnicodeUTF8));
    actionNew_Graph->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
    //actionImport_Graph->setText(QApplication::translate("MainWindow", "Import Graph", 0, QApplication::UnicodeUTF8));
    //actionExport_Graph->setText(QApplication::translate("MainWindow", "Export Graph", 0, QApplication::UnicodeUTF8));
   
   actionSet_Background_Color->setText(QApplication::translate("MainWindow", "Set Background Color", 0, QApplication::UnicodeUTF8));
    //actionShow_Vertex_Names->setText(QApplication::translate("MainWindow", "Show Vertex Names", 0, QApplication::UnicodeUTF8));
    //actionShow_Vertex_Indices->setText(QApplication::translate("MainWindow", "Show Vertex Indices", 0, QApplication::UnicodeUTF8));
    actionCleanLooks->setText(QApplication::translate("MainWindow", "CleanLooks", 0, QApplication::UnicodeUTF8));
    actionWindows->setText(QApplication::translate("MainWindow", "Windows", 0, QApplication::UnicodeUTF8));
    actionPlastique->setText(QApplication::translate("MainWindow", "Plastique", 0, QApplication::UnicodeUTF8));
    actionMotif->setText(QApplication::translate("MainWindow", "Motif", 0, QApplication::UnicodeUTF8));
    actionCDE->setText(QApplication::translate("MainWindow", "CDE", 0, QApplication::UnicodeUTF8));
 addVertexButton->setText(QApplication::translate("MainWindow", "Add Vertex", 0, QApplication::UnicodeUTF8));
    addVertexButton->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
    addEdgeButton->setText(QApplication::translate("MainWindow", "Add Edge", 0, QApplication::UnicodeUTF8));
    addEdgeButton->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    zoomInButton->setToolTip(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    zoomInButton->setText(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
    zoomInButton->setShortcut(QApplication::translate("MainWindow", "Ctrl+=", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    zoomOutButton->setToolTip(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    zoomOutButton->setText(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
    zoomOutButton->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0, QApplication::UnicodeUTF8));
    toggleAxesButton->setText(QApplication::translate("MainWindow", "Toggle Axes", 0, QApplication::UnicodeUTF8));
    resetViewButton->setText(QApplication::translate("MainWindow", "Reset View", 0, QApplication::UnicodeUTF8));
    groupBox_Properties->setTitle(QApplication::translate("MainWindow", "Properties", 0, QApplication::UnicodeUTF8));
    label_Name->setText(QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
    label_Color->setText(QApplication::translate("MainWindow", "Color", 0, QApplication::UnicodeUTF8));
    comboBox_ColorSelect->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Yellow", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Purple", 0, QApplication::UnicodeUTF8)
	<< QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8)
	<< QApplication::translate("MainWindow", "Orange", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "White", 0, QApplication::UnicodeUTF8)
    );
    groupBox_Neighbors->setTitle(QApplication::translate("MainWindow", "Out Neighbors", 0, QApplication::UnicodeUTF8));

    groupBox_Analysis->setTitle(QApplication::translate("MainWindow", "Analysis", 0, QApplication::UnicodeUTF8));
    label_Degree->setText(QApplication::translate("MainWindow", "Total Degree", 0, QApplication::UnicodeUTF8));
    label_Degree_in->setText(QApplication::translate("MainWindow", "In Degree", 0, QApplication::UnicodeUTF8));
    label_Degree_out->setText(QApplication::translate("MainWindow", "Out Degree", 0, QApplication::UnicodeUTF8));    
label_AvgShortestPath->setText(QApplication::translate("MainWindow", "Average Shortest Path", 0, QApplication::UnicodeUTF8));
    label_PageRank->setText(QApplication::translate("MainWindow", "Page Rank", 0, QApplication::UnicodeUTF8));
    label_CloesnessCentrality->setText(QApplication::translate("MainWindow", "Closeness Centrality", 0, QApplication::UnicodeUTF8));
	zoomToVertex->setText(QString());
    pushButton_Update_Analysis->setText(QApplication::translate("MainWindow", "Update Analysis", 0, QApplication::UnicodeUTF8));
	    addNeighborButton->setText(QString());
    removeNeighborButton->setText(QString());
    label_Vertex->setText(QApplication::translate("MainWindow", "Vertex", 0, QApplication::UnicodeUTF8));
    SideBar->setTabText(SideBar->indexOf(tab_browse_edit), QApplication::translate("MainWindow", "Browse/Edit", 0, QApplication::UnicodeUTF8));
    layoutOptionsGroup->setTitle(QApplication::translate("MainWindow", "Layout", 0, QApplication::UnicodeUTF8));
    radioButton_3D->setText(QApplication::translate("MainWindow", "3D Layout", 0, QApplication::UnicodeUTF8));
    comboBox_3DLayout->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Random", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Sphere", 0, QApplication::UnicodeUTF8)
	// CHANGE
	<< QApplication::translate("MainWindow", "Circle", 0, QApplication::UnicodeUTF8)
	// END CHANGE
     << QApplication::translate("MainWindow", "Spiral", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Fruchterman-Reingold", 0, QApplication::UnicodeUTF8)
	<< QApplication::translate("MainWindow", "Layers by Group", 0, QApplication::UnicodeUTF8)
    );
    radioButton_2D->setText(QApplication::translate("MainWindow", "2D Layout", 0, QApplication::UnicodeUTF8));
    comboBox_2DLayout->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Random", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Circle", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("MainWindow", "Spiral", 0, QApplication::UnicodeUTF8)
	<< QApplication::translate("MainWindow", "Layers by Group", 0, QApplication::UnicodeUTF8)
	<< QApplication::translate("MainWindow", "Fruchterman-Reingold", 0, QApplication::UnicodeUTF8)
    );
    label->setText(QApplication::translate("MainWindow", "Speed", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("MainWindow", "Repulsion", 0, QApplication::UnicodeUTF8));
    toolButton_AutoArrange->setText(QApplication::translate("MainWindow", "Auto Arrange", 0, QApplication::UnicodeUTF8));
    plottingOptionsGroup->setTitle(QApplication::translate("MainWindow", "Plotting Options", 0, QApplication::UnicodeUTF8));
    labelXRange->setText(QApplication::translate("MainWindow", "X Range", 0, QApplication::UnicodeUTF8));
    labelYRange->setText(QApplication::translate("MainWindow", "Y Range", 0, QApplication::UnicodeUTF8));
    labelZRange->setText(QApplication::translate("MainWindow", "Z Range", 0, QApplication::UnicodeUTF8));
    labelVertexRadius->setText(QApplication::translate("MainWindow", "Vertex Radius", 0, QApplication::UnicodeUTF8));
	    button_Replot->setText(QApplication::translate("MainWindow", "Replot", 0, QApplication::UnicodeUTF8));    
	    button_Default_Plot->setText(QApplication::translate("MainWindow", "Default", 0, QApplication::UnicodeUTF8));
SideBar->setTabText(SideBar->indexOf(tab_layout), QApplication::translate("MainWindow", "Layout", 0, QApplication::UnicodeUTF8));
    //SideBar->setTabText(SideBar->indexOf(tab_analysis), QApplication::translate("MainWindow", "Analysis", 0, QApplication::UnicodeUTF8));
    menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    /*menuAnalysis->setTitle(QApplication::translate("MainWindow", "Analysis", 0, QApplication::UnicodeUTF8));
    menuGraph_Data->setTitle(QApplication::translate("MainWindow", "Graph Data", 0, QApplication::UnicodeUTF8));
    menuVertex_Data->setTitle(QApplication::translate("MainWindow", "Vertex Data", 0, QApplication::UnicodeUTF8));*/
    menuGraph->setTitle(QApplication::translate("MainWindow", "Graph", 0, QApplication::UnicodeUTF8));
    menuGenerate_Graph->setTitle(QApplication::translate("MainWindow", "Generate Graph", 0, QApplication::UnicodeUTF8));
    actionMake_Undirected->setText(QApplication::translate("MainWindow", "Make Undirected", 0, QApplication::UnicodeUTF8));
    menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
    menuSet_Window_Style->setTitle(QApplication::translate("MainWindow", "Set Window Style", 0, QApplication::UnicodeUTF8));

    } // retranslateUi
