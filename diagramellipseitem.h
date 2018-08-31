#ifndef DIAGRAMELLIPSEITEM_H
#define DIAGRAMELLIPSEITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QMap>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
class ItemSignal;
class DiagramDashItem;
QT_END_NAMESPACE

class Arrow;
class MainWindow;

//! [0]
class DiagramEllipseItem : public QGraphicsEllipseItem
{

public:
    explicit DiagramEllipseItem(QMenu *contextMenu, MainWindow *mainWindow, const QRectF& rect, QGraphicsItem *parent = 0);
    explicit DiagramEllipseItem(QMenu *contextMenu, MainWindow *mainWindow, QGraphicsItem *parent = 0);
    explicit DiagramEllipseItem(QMenu *contextMenu, MainWindow *mainWindow, int id, const QRectF& rect, int belong_wid, QString &name, int wid_type, int radius, int facade, QGraphicsItem *parent = 0);
    explicit DiagramEllipseItem(QMenu *contextMenu, QGraphicsItem *parent = 0);
    static const int myEllipseType = 4;
    const int myType = 1;
    void removeArrow(Arrow *arrow);
    void removeArrows();
    void addArrow(Arrow *arrow);
    QPixmap image() const;
    int type() const override { return Type;}
    int getMyType() { return myType;}
    QRectF getMyRectF();
    QString getMyName();
    int getMyWid_tpye();
    int getMyRank();
    int getMyFacade();
    int getMyBelong_wid();
    int getMyId();
    void setMyId(int id);
    qreal getRadius();
    void setRadius(qreal radius);
    int getMySecondId();
    void setMySecondId(int id);
    void setMyDashItem(DiagramDashItem* dashItem);
    void setMyBelong_wid(int belong_wid);
    void setMyName(QString name);
    void setMyFacade(int facade);
    void setWid_type(int wid_type);
    DiagramDashItem* getMyDashItem();
    static int base_id;
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QString myName;
    int myId;
    int myWid_type;
    int myRank;
    int myFacade;
    int myBelong_wid;
    int mySecondId = -1;
    qreal myRadius;
    DiagramDashItem* myDashItem;
    QRectF myRect;
    QMenu *myContextMenu;
    QList<Arrow *> arrows;
    MainWindow *myMainWindow;
    ItemSignal *myItemSignal;

};
//! [0]



#endif // DIAGRAMELLIPSEITEM_H
