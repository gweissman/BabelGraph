######################################################################
# Automatically generated by qmake (2.01a) Wed Jul 30 13:10:17 2008
######################################################################

TEMPLATE = app
TARGET = BabelGraph
DEPENDPATH += .
INCLUDEPATH += .
QT += opengl
SUBDIRS += src
RESOURCES = BG_resources.qrc

# Input
HEADERS += GraphWidget.hpp \
           kregulargraphdialog.hpp \
           MainWindow.hpp \
           randomgraphdialog.hpp \
           strangersbanquetgraphdialog.hpp \
           NiceGraph.hpp
SOURCES += GraphWidget.cpp \
           main.cpp \
           MainWindow.cpp \
           NiceGraph.cpp
