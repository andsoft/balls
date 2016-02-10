#include "shape.h"
#include <QtMath>

QRect Shape::rect = QRect();

Shape::Shape()
{
    m_locked = false;
}

Shape::~Shape()
{

}

void Shape::lock(bool bLock)
{
    m_locked = bLock;
}

bool Shape::isLocked()
{
    return m_locked;
}

Circle::Circle(int pt_x, int pt_y, QString name) :
    x(pt_x),
    y(pt_y),
    m_name(name)
{
    x_force = 0.0;
    y_force = 0.0;
}

Circle::~Circle()
{

}


bool Circle::hitTest(int hit_x, int hit_y)
{
    if (((hit_x - x) * (hit_x - x) + (hit_y - y) * (hit_y - y)) < r * r)
    {
        // Точка принадлежит окружности
        return true;
    }
    else if (((hit_x - x) * (hit_x - x) + (hit_y - y) * (hit_y - y)) == r * r)
    {
        // Точка лежит на окружности
        return true;
    }
    else
    {
        // Точка не принадлежит окружности
    }

    return false;
}

void Circle::setForce(double to_x, double to_y, int f)
{
    x_force = to_x;
    y_force = to_y;
    f_force = f;
}

void Circle::applyForce()
{
    x += x_force;
    y += y_force;

    x_force = 0.0;
    y_force = 0.0;

    adjustPos();
}

void Circle::draw(QPainter * painter)
{
    QPen g_pen(QColor(0, 0, 0, 0));
        g_pen.setWidth(0);
        QRadialGradient grad(x, y, 40/2, x+5, y+5); // Create Gradient
        grad.setColorAt(1, QColor(200, 200, 0)); // Black, varying alpha
        grad.setColorAt(0, QColor(255, 255, 0)); // Black, completely transparent
        QBrush g_brush(grad); // Gradient QBrush
        painter->setPen(g_pen);
        painter->setBrush(g_brush);

    painter->drawEllipse(x-r, y-r, r*2, r*2);
    //painter->drawText(x-r/2, y+r/2, m_name);
    //painter->drawEllipse(x-r/2+x_force, y-r/2+y_force, r, r); // next step
}

void Circle::dragStart(QPoint pos)
{
    m_dragOffset = pos - QPoint(x, y);
}
// TODO : adjust by screen rect
void Circle::dragMove(QPoint pos)
{
    x = pos.x() - m_dragOffset.x();
    y = pos.y() - m_dragOffset.y();

    adjustPos();
}

void Circle::dragStop(QPoint pos)
{
    dragMove(pos);
}

void Circle::adjustPos()
{
    if(x < r) x = r;
    if(y < r) y = r;
    if(x > rect.width() - r) x = rect.width() - r;
    if(y > rect.height() - r) y = rect.height() - r;
}
