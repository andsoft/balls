#include "renderarea.h"

#include <QTimer>

RenderArea::RenderArea(RenderThread *renderer, QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    m_renderer = renderer;
    elapsed = 0;
    m_pDragShape = NULL;

    FPS = 0;
    m_fps_cnt = 0;
    m_fps_timer.start();
}

void RenderArea::animate()
{
    elapsed = 5; //(elapsed + 5);//qobject_cast<QTimer*>(sender())->interval()) % 1000;

//    for(int i=0; i<m_circles.size(); i++)
//    {
//        QRect rect = this->rect();
//        int x = m_circles[i].x()+elapsed;
//        if(x > rect.right())
//            x = rect.right();

//        m_shapes[i].setX( x );
//    }
    repaint();
}

void RenderArea::resizeEvent(QResizeEvent * event)
{
    Shape::rect = QRect(0, 0, event->size().width(), event->size().height());
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.save();
    //painter.fillRect(rect(), Qt::gray);


    std::list<Shape*> *objects = m_renderer->lockData();
    int count = objects->size();
    for(obj_it iterator = objects->begin(); iterator != objects->end(); iterator++)
    {
        Shape* obj = *iterator;
        obj->draw(&painter);
    }
    m_renderer->unlockData();

    m_fps_cnt++;
    quint64 time = m_fps_timer.elapsed();

    if(time > 1000)
    {
        FPS = m_fps_cnt;
        m_fps_cnt = 0;
        m_fps_timer.restart();
    }
    painter.restore();
    painter.setFont(QFont("Courier", 14));
    painter.drawText(30, 30,
                     QString("Count: %1,  ").arg(count) +
                     QString("FPS: %1, ").arg(FPS) +
                     QString("Calculation: %1 ms").arg(m_renderer->getCalcTime()));

    painter.end();
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    if(m_pDragShape)
    {
        // do dragging
        m_pDragShape->dragMove(event->pos()); // todo : offset
        //repaint();
    }
}

void RenderArea::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_pDragShape = m_renderer->hitTest(event->pos(), true);

        // todo : optimize by hittesting only grid cell elements
        if( m_pDragShape )
        {
            // start dragging
            m_pDragShape->dragStart(event->pos());
            return;
        }
    }
    else if(event->button() == Qt::RightButton && m_pDragShape == NULL)
    {
        Shape* obj = m_renderer->hitTest(event->pos());

        // todo : optimize by hittesting only grid cell elements
        if( obj )
        {
            m_renderer->delObject(obj);
            return;
        }


        m_renderer->addObject(new Circle(event->pos().x(), event->pos().y()) );
    }

    //repaint();
}

void RenderArea::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_pDragShape)
        {
            // stop dragging
            m_pDragShape->dragStop(event->pos()); // todo : offset
            m_pDragShape->lock(false);
            m_pDragShape = NULL;
            //repaint();
        }
    }
}
