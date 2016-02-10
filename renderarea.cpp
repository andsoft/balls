#include "renderarea.h"

#include <QTimer>

RenderArea::RenderArea(RenderThread *renderer, QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    m_renderer = renderer;
    elapsed = 0;
    m_pDragShape = NULL;
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

    painter.fillRect(rect(), Qt::darkGray);

    std::map<quint64, Shape*> *objects = m_renderer->lockData();
    for(obj_it iterator = objects->begin(); iterator != objects->end(); iterator++)
    {
        //quint64 key = iterator->first;
        Shape* obj = iterator->second;
        obj->draw(&painter);
    }
    m_renderer->unlockData();
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
        quint64 obj_id = m_renderer->hitTest(event->pos(), true);

        // todo : optimize by hittesting only grid cell elements
        if( obj_id != BAD_ID )
        {
            // start dragging
            m_pDragShape = m_renderer->getObject(obj_id);
            m_pDragShape->dragStart(event->pos());
            return;
        }
    }
    else if(event->button() == Qt::RightButton && m_pDragShape == NULL)
    {
        quint64 obj_id = m_renderer->hitTest(event->pos());

        // todo : optimize by hittesting only grid cell elements
        if( obj_id != BAD_ID )
        {
            m_renderer->delObject(obj_id);
            //repaint();
            return;
        }


        m_renderer->addObject(new Circle(event->pos().x(), event->pos().y(), QString("%1").arg(1)) );
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
