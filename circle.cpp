#include "circle.h"
#include <QtMath>

QRect Circle::rect = QRect();

Circle::Circle(int pt_x, int pt_y, int radius) :
    x(pt_x),
    y(pt_y),
    r(radius)
{
    x_speed = 0.0;
    y_speed = 0.0;

    m_locked = false;
}

Circle::~Circle()
{

}

void Circle::lock(bool bLock)
{
    m_locked = bLock;
}

bool Circle::isLocked() const
{
    return m_locked;
}

bool Circle::hitTest(double hit_x, double hit_y)
{
    if (((hit_x - x) * (hit_x - x) + (hit_y - y) * (hit_y - y)) < r * r)
    {
        return true; // point inside
    }
    else if (((hit_x - x) * (hit_x - x) + (hit_y - y) * (hit_y - y)) == r * r)
    {
        return true;// point on the border
    }

    return false; //point outside
}

void Circle::setForce(double to_x, double to_y)
{
    x_speed += to_x;
    y_speed += to_y;
}

void Circle::applyForce()
{
    x += x_speed;
    y += y_speed;

    adjustPos();
}

void Circle::draw(QPainter * painter)
{
    QPen pen(QColor(0, 0, 0, 0));
    pen.setWidth(0);

    QRadialGradient grad(x, y, r, x+r/4, y+r/4); // create gradient
    grad.setColorAt(1, QColor(30, 30, 30));
    grad.setColorAt(0, QColor(255, 255, 250));

    QBrush brush(grad); // gradient QBrush

    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawEllipse(x-r, y-r, r*2, r*2);
    //painter->drawEllipse(x-r/2+x_force, y-r/2+y_force, r, r); // next step
}

void Circle::dragStart(QPoint pos)
{
    x_speed = 0;
    y_speed = 0;
    m_dragOffset = pos - QPoint(x, y);
}

void Circle::dragMove(QPoint pos)
{
    x = pos.x() - m_dragOffset.x();
    y = pos.y() - m_dragOffset.y();
    // adjust by screen rect
    adjustPos();
}

void Circle::dragStop(QPoint pos)
{
    dragMove(pos);
}

void Circle::adjustPos()
{
    return;
    if(!rect.adjusted(20,20,20,20).contains(x,y))
    {
    x_speed = 0;
    y_speed = 0;
    }
    if(x < r) x = r;
    if(y < r) y = r;
    if(x > rect.width() - r) x = rect.width() - r;
    if(y > rect.height() - r) y = rect.height() - r;
}
