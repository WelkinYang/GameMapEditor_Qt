#include "diagramdashitem.h"
#include <QPen>
int DiagramDashItem::base_id = 0;
DiagramDashItem::DiagramDashItem(const QRectF& rect, int radius, int FirstStarId, int SecondStarId, QGraphicsItem *parent)
    :QGraphicsEllipseItem(parent)
{
    myId = ++base_id;
    myFirstStarId = FirstStarId;
    mySecondStarId = SecondStarId;
    myRect = rect;
    myRadius = radius;
    QPen Pen = pen();
    Pen.setColor(Qt::blue);
    Pen.setStyle(Qt::DashLine);
    setPen(Pen);
    setRect(myRect);
    this->setSelected(false);
}
