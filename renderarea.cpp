#include "renderarea.h"

#include <QTimer>

RenderArea::RenderArea(RenderThread *renderer, QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    m_radius = 10;

    m_renderer = renderer;
    m_pDragCircle = NULL;

    FPS = 0;
    m_fps_cnt = 0;
    m_fps_timer.start();

    QTimer *timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));

    timer->start(20);
}

void RenderArea::resizeEvent(QResizeEvent * event)
{
    Circle::rect = QRect(0, 0, event->size().width(), event->size().height());
}

void RenderArea::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.save();

    // draw background
    //painter.fillRect(rect(), Qt::gray);

    // draw objects
    std::list<Circle*> *objects = m_renderer->lockData();
    int count = objects->size();
    for(obj_it iterator = objects->begin(); iterator != objects->end(); iterator++)
    {
        Circle* obj = *iterator;
        obj->draw(&painter);
    }
    m_renderer->unlockData();

    // detect FPS
    m_fps_cnt++;
    quint64 time = m_fps_timer.elapsed();
    if(time > 1000)
    {
        FPS = m_fps_cnt;
        m_fps_cnt = 0;
        m_fps_timer.restart();
    }
    painter.restore();

    // draw info
    painter.setFont(QFont("Courier", 14));
    painter.drawText(30, 30,
                     QString("Count: %1,  ").arg(count) +
                     QString("FPS: %1, ").arg(FPS) +
                     QString("Calculation: %1 ms").arg(m_renderer->getCalcTime()));

    painter.end();
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    if(m_pDragCircle)
    {
        // do dragging
        m_pDragCircle->dragMove(event->pos());
    }
}

void RenderArea::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_pDragCircle = m_renderer->hitTest(event->pos(), true);


        if( m_pDragCircle )
        {
            // start dragging
            m_pDragCircle->dragStart(event->pos());
            return;
        }
    }
    else if(event->button() == Qt::RightButton && m_pDragCircle == NULL)
    {
        Circle* obj = m_renderer->hitTest(event->pos());

        // todo : optimize by hittesting only grid cell elements
        if( obj )
        {
            m_renderer->delObject(obj);
            return;
        }


        m_renderer->addObject(new Circle(event->pos().x(), event->pos().y(), m_radius) );
    }

    //repaint();
}

void RenderArea::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_pDragCircle)
        {
            // stop dragging
            m_pDragCircle->dragStop(event->pos()); // todo : offset
            m_pDragCircle->lock(false);
            m_pDragCircle = NULL;
            //repaint();
        }
    }
}
