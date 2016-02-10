#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "shape.h"

#include <QThread>
#include <map>
#include <mutex>

#define BAD_ID 0

typedef std::map<quint64, Shape*>::iterator obj_it;

class RenderThread : public QThread
{
    Q_OBJECT
public:
    explicit RenderThread(QObject* parent = 0);
    ~RenderThread();

    void startProcess();
    void stopProcess();

    // public interface for interacting with the "world"
    void removeAllObjects();
    void addObject(Shape* obj);
    void delObject(quint64 objId);
    Shape* getObject(quint64 objId);

    std::map<quint64, Shape*> * lockData();
    void unlockData();

    quint64 hitTest(QPoint point, bool bLock = false);

signals:

public slots:

protected:
    virtual void run();

    void applyForces();
    void calculateForce(Shape* obj);
    void collision(Shape* obj);

private:
    std::map<quint64, Shape*> m_objects;
    quint64 m_nextId;
    bool m_abort;

    std::mutex m_mutex;
};

#endif // RENDERTHREAD_H
