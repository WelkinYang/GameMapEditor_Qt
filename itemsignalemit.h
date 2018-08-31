#ifndef ITEMSIGNALEMIT_H
#define ITEMSIGNALEMIT_H

#include <QObject>

QT_BEGIN_NAMESPACE
class MainWindow;
class DiagramEllipseItem;
QT_END_NAMESPACE

class ItemSignal : public QObject
{
    Q_OBJECT
public:
    ItemSignal(DiagramEllipseItem *item, MainWindow *mainWindow, QObject *parent = 0);
    void ItemSignalEmit();
    void radiusChangeSignalEmit();

private:
    MainWindow *myMainWindow;
    DiagramEllipseItem *myDiagramEllipseItem;
signals:
    void mySignal(DiagramEllipseItem*);
    void changeRadius(DiagramEllipseItem* item);
};

#endif // ITEMSIGNALEMIT_H

