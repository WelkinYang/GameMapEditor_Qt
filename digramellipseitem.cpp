#include "diagramellipseitem.h"
#include "arrow.h"
#include "mainwindow.h"
#include "itemsignalemit.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

//! [0]

int DiagramEllipseItem::base_id = 0;
DiagramEllipseItem::DiagramEllipseItem(QMenu *contextMenu, MainWindow *mainWindow, const QRectF& rect, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    myContextMenu = contextMenu;
    myRect = rect;
    myMainWindow = mainWindow;
    myItemSignal = new ItemSignal(this, myMainWindow);
    setRect(myRect);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
//! [0]
//!
DiagramEllipseItem::DiagramEllipseItem(QMenu *contextMenu, MainWindow *mainWindow, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    myContextMenu = contextMenu;
    QRectF r(-100, -100, 200, 200);
    myRect = r;
    myMainWindow = mainWindow;
    myItemSignal = new ItemSignal(this, myMainWindow);
    setRect(myRect);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

DiagramEllipseItem::DiagramEllipseItem(QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    myContextMenu = contextMenu;
    QRectF r(-100, -100, 200, 200);
    myRect = r;
    setRect(myRect);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

DiagramEllipseItem::DiagramEllipseItem(QMenu *contextMenu, MainWindow *mainWindow, int id, const QRectF& rect, int belong_wid, QString &name, int wid_type, int radius, int facade, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    myContextMenu = contextMenu;
    myRect = rect;
    myId = id;
    myBelong_wid = belong_wid;
    myName = name;
    myWid_type = wid_type;
    myFacade = facade;
    myMainWindow = mainWindow;
    myRadius = radius;
    myItemSignal = new ItemSignal(this, myMainWindow);
    setRect(myRect);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//! [1]
void DiagramEllipseItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}
//! [1]

//! [2]
void DiagramEllipseItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
//! [2]

//! [3]
void DiagramEllipseItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}
//! [3]

//! [4]
QPixmap DiagramEllipseItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawEllipse(myRect);

    return pixmap;
}
//! [4]

//! [5]
void DiagramEllipseItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
//! [5]

//! [6]
QVariant DiagramEllipseItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}
//! [6]
//!
void DiagramEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;
    myItemSignal->ItemSignalEmit();
    scene()->clearSelection();
    setSelected(true);
}
QRectF DiagramEllipseItem::getMyRectF()
{
    return myRect;
}


QString DiagramEllipseItem::getMyName()
{
    return myName;
}
int DiagramEllipseItem::getMyWid_tpye()
{
    return myWid_type;
}
int DiagramEllipseItem::getMyRank()
{
    return myRank;
}
int DiagramEllipseItem::getMyFacade()
{
    return myFacade;
}
int DiagramEllipseItem::getMyBelong_wid()
{
    return myBelong_wid;
}
int DiagramEllipseItem::getMyId()
{
    return myId;
}
void DiagramEllipseItem::setMyId(int id)
{
    myId = id;
}
void DiagramEllipseItem::setMyDashItem(DiagramDashItem* DashItem)
{
    myDashItem = DashItem;
}
DiagramDashItem* DiagramEllipseItem::getMyDashItem()
{
    return myDashItem;
}
int DiagramEllipseItem::getMySecondId()
{
    return mySecondId;
}
void DiagramEllipseItem::setMySecondId(int id)
{
    mySecondId = id;
}
qreal DiagramEllipseItem::getRadius()
{
    return myRadius;
}
void DiagramEllipseItem::setRadius(qreal radius)
{
    myRadius = radius;
    myRect.setWidth(myRadius*2);
    myRect.setHeight(myRadius*2);
    myItemSignal->radiusChangeSignalEmit();
}
void DiagramEllipseItem::setMyBelong_wid(int belong_wid)
{
    myBelong_wid = belong_wid;
}
void DiagramEllipseItem::setMyName(QString name)
{
    myName = name;
}
void DiagramEllipseItem::setWid_type(int wid_type)
{
    myWid_type = wid_type;
}
void DiagramEllipseItem::setMyFacade(int facade)
{
    myFacade = facade;
}
