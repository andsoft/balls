#ifndef RENDERAREA_H
#define RENDERAREA_H

#include "shape.h"

#include <QWidget>
#include <QList>
#include <QPainter>
#include <QMouseEvent>
#include <QElapsedTimer>

#include "renderthread.h"

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(RenderThread *renderer, QWidget *parent = 0);

signals:

public slots:
    void animate();

protected:
    void resizeEvent(QResizeEvent * event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    RenderThread *m_renderer;
    QList<Shape*> m_shapes;
    int elapsed;

    Shape* m_pDragShape;

    int FPS;
    QElapsedTimer m_fps_timer;
    int m_fps_cnt;
};

#endif // RENDERAREA_H
