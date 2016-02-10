#ifndef SHAPE_H
#define SHAPE_H

#include <QPainter>

class Shape
{
public:
    Shape();
    virtual ~Shape();

    void lock(bool b);
    bool isLocked();

    virtual bool hitTest(int x, int y) = 0;
    virtual void setForce(double x, double y, int f) = 0;
    virtual void applyForce() = 0;

    virtual void draw(QPainter * painter) = 0;

    virtual void dragStart(QPoint pos) = 0;
    virtual void dragMove(QPoint pos) = 0;
    virtual void dragStop(QPoint pos) = 0;

    virtual int centerX() = 0;
    virtual int centerY() = 0;

    virtual int forceX() = 0;
    virtual int forceY() = 0;
    virtual int forceF() = 0;

    static QRect rect; // bounding rect

private:
    bool m_locked;
};

class Circle : public Shape
{
public:
    Circle(int x, int y);
    virtual ~Circle();

    virtual bool hitTest(int hit_x, int hit_y);
    virtual void setForce(double to_x, double to_y, int f);
    virtual void applyForce();

    virtual void draw(QPainter * painter);

    virtual void dragStart(QPoint pos);
    virtual void dragMove(QPoint pos);
    virtual void dragStop(QPoint pos);

    virtual int centerX() {return x;}
    virtual int centerY() {return y;}

    virtual int forceX() {return x_force;}
    virtual int forceY() {return y_force;}
    virtual int forceF() {return f_force;}

protected:
    void adjustPos();

private:
    double x;
    double y;

    QString m_name;

    const int r = 20;

    QPoint m_dragOffset;
    double x_force;
    double y_force;
    int f_force;
};

#endif // SHAPE_H
