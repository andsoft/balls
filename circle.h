#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPainter>


class Circle
{
public:
    Circle(int x, int y);
    virtual ~Circle();

    virtual bool hitTest(int hit_x, int hit_y);
    virtual void setForce(double to_x, double to_y);
    virtual void applyForce();

    virtual void draw(QPainter * painter);

    virtual void dragStart(QPoint pos);
    virtual void dragMove(QPoint pos);
    virtual void dragStop(QPoint pos);

    int centerX() {return x;}
    int centerY() {return y;}

    int forceX() {return x_force;}
    int forceY() {return y_force;}

    int radius() { return r; }

    void lock(bool b);
    bool isLocked();

    static QRect rect; // bounding rect

protected:
    void adjustPos();

private:
    double x; // center x
    double y; // center y

    const int r = 20; // hardcoded radius

    QPoint m_dragOffset;
    double x_force;
    double y_force;

    bool m_locked;
};

#endif // CIRCLE_H
