#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "circle.h"

#include <QThread>
#include <map>
#include <mutex>
#include <atomic>


typedef std::list<Circle*>::iterator obj_it;

class RenderThread : public QThread
{
    Q_OBJECT
public:
    explicit RenderThread(QObject* parent = 0);
    ~RenderThread();

    void startProcess();
    void stopProcess();

    unsigned long long getCalcTime() { return m_calcTime.load(); }

    // public interface for interacting with the "world"
    void removeAllObjects();
    void addObject(Circle* obj);
    void delObject(Circle* obj);

    std::list<Circle*> * lockData();
    void unlockData();

    Circle* hitTest(QPoint point, bool bLock = false);

    // customization
    void setScale(int scale);       // r-axis resolution in pixels
    void setSpeed(double speed);    // speed coeff

signals:

public slots:

protected:
    virtual void run();

    void applyForces();
    void calculateForce(Circle* obj);
    void collision(Circle* obj);

    unsigned long long getTickCount();

private:
    std::list<Circle*> m_objects; // TODO : use unique_ptr list instead of raw pointers !!!

    std::atomic<bool>  m_abort;
    std::mutex m_mutex;

    std::atomic<unsigned long long> m_calcTime;

    int m_scale;
    double m_speed;
};

#endif // RENDERTHREAD_H
