#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPainter>


class Circle
{
public:
    Circle(int x, int y, int radius = 20);
    virtual ~Circle();

    virtual bool hitTest(double hit_x, double hit_y);
    virtual void setForce(double to_x, double to_y);
    virtual void applyForce();

    virtual void draw(QPainter * painter);

    virtual void dragStart(QPoint pos);
    virtual void dragMove(QPoint pos);
    virtual void dragStop(QPoint pos);

    double centerX() const {return x;}
    double centerY() const {return y;}

    double speedX() const {return x_speed;}
    double speedY() const {return y_speed;}

    int radius() const { return r; }
    double mass() const { return 0.0003 * 4.0 * 3.14 * pow(r, 3)/3.0; } // use volume as mass criteria

    void lock(bool b);
    bool isLocked() const ;

    static QRect rect; // bounding rect

protected:
    void adjustPos();

private:
    double x; // center x
    double y; // center y

    const int r; // radius

    QPoint m_dragOffset;

    double x_speed;
    double y_speed;

    bool m_locked;
};

#endif // CIRCLE_H
