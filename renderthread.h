#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "shape.h"

#include <QThread>
#include <map>
#include <mutex>
#include <atomic>

#define BAD_ID 0

typedef std::list<Shape*>::iterator obj_it;

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
    void addObject(Shape* obj);
    void delObject(Shape* obj);

    std::list<Shape*> * lockData();
    void unlockData();

    Shape* hitTest(QPoint point, bool bLock = false);

signals:

public slots:

protected:
    virtual void run();

    void applyForces();
    void calculateForce(Shape* obj);
    void collision(Shape* obj);

    unsigned long long getTickCount();

private:
    std::list<Shape*> m_objects;

    std::atomic<bool>  m_abort;
    std::mutex m_mutex;

    std::atomic<unsigned long long> m_calcTime;
};

#endif // RENDERTHREAD_H
