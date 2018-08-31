#include "mainwindow.h"
#include "diagramellipseitem.h"
#include "itemsignalemit.h"

ItemSignal::ItemSignal(DiagramEllipseItem *diagramEllipseItem, MainWindow *mainWindow, QObject *parent)
    :QObject(parent)
{
    myMainWindow = mainWindow;
    myDiagramEllipseItem = diagramEllipseItem;
    connect(this, SIGNAL(mySignal(DiagramEllipseItem*)), myMainWindow, SLOT(infoUpdate(DiagramEllipseItem*)));
    connect(this, SIGNAL(changeRadius(DiagramEllipseItem*)), myMainWindow, SLOT(changeRadius(DiagramEllipseItem*)));
}

void ItemSignal:: ItemSignalEmit()
{
    emit mySignal(myDiagramEllipseItem);
}
void ItemSignal:: radiusChangeSignalEmit()
{
    emit changeRadius(myDiagramEllipseItem);
}
