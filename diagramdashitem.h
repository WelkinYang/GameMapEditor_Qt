#ifndef DIAGRAMDASHITEM_H
#define DIAGRAMDASHITEM_H

#include <QGraphicsPixmapItem>
#include <QList>

class DiagramDashItem : public QGraphicsEllipseItem
{

public:
    explicit DiagramDashItem(const QRectF& rect, int radius, int FirstStarId, int SecondStarId, QGraphicsItem *parent = 0);
    int myFirstStarId;
    int mySecondStarId;
    int myId;
    int myRadius;
    QRectF myRect;
    static int base_id;
    const int myType = 2;
    int getMyType() { return myType;}
};
#endif // DIAGRAMDASHITEM_H
