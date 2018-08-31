#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H
#include <QGraphicsView>
#include <QWidget>

class DiagramView : public QGraphicsView
{
    Q_OBJECT
public:
    DiagramView(QWidget *parent = 0);
    DiagramView(QGraphicsScene *scene, QWidget *parent = 0);
    void itemMoved();

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void scaleView(qreal scaleFactor);
};


#endif // DIAGRAMVIEW_H
