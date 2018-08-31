#include "diagramview.h"


#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>

//! [0]
DiagramView::DiagramView(QWidget *parent)
    : QGraphicsView(parent)
{

}
DiagramView::DiagramView(QGraphicsScene *scene, QWidget *parent)
    :QGraphicsView(scene, parent)
{

}
void DiagramView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Equal:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void DiagramView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
//! [7]


void DiagramView::zoomIn()
{
    scaleView(qreal(1.2));
}

void DiagramView::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
