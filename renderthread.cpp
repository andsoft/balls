#include "renderthread.h"

#include <QDebug>

#define R_SCALE 20  // scale
#define SPEED   1.0  // speed

RenderThread::RenderThread(QObject *parent) : QThread(parent)
{
    m_abort = false;
    m_calcTime = 0;

    m_scale = R_SCALE;
    m_speed = SPEED;

    startProcess();
}

RenderThread::~RenderThread()
{
    stopProcess();
    removeAllObjects();
}

void RenderThread::startProcess()
{
    qDebug("execution starting ");
    m_abort.store(false);
    start();
}

void RenderThread::stopProcess()
{
    m_abort.store(true);
    wait();
    qDebug("execution stoped ");
}

void RenderThread::removeAllObjects()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for(Circle *obj : m_objects)
        delete obj;
    m_objects.clear();
}

void RenderThread::addObject(Circle* obj)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_objects.push_back(obj);
}

void RenderThread::delObject(Circle* obj)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_objects.remove(obj);
    delete obj;
}

std::list<Circle *> *RenderThread::lockData()
{
    m_mutex.lock();
    return &m_objects;
}

void RenderThread::unlockData()
{
    m_mutex.unlock();
}
// todo : optimize by hittesting only grid cell elements
Circle *RenderThread::hitTest(QPoint point, bool bLock)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for(obj_it iterator = m_objects.begin(); iterator != m_objects.end(); iterator++)
    {
        Circle* obj = *iterator;
        if(obj->hitTest(point.x(), point.y()))
        {
            if(bLock)
                obj->lock(true);
            return obj;
        }
    }
    return NULL;
}
// No Qt in run function, only QThread wrapper
void RenderThread::run()
{
    while(!m_abort.load())
    {
        auto t0 = getTickCount();

        m_mutex.lock();
        applyForces();
        m_mutex.unlock();

        m_calcTime = getTickCount() - t0;

        msleep(1);
    }
}

void RenderThread::applyForces()
{
    // calculate forces
    for(obj_it iterator = m_objects.begin(); iterator != m_objects.end(); iterator++)
    {
        Circle* obj = *iterator;
        if(!obj->isLocked())     // check if object captured by mouse
            calculateForce(obj);
    }

    // apply forces
    for(Circle* obj : m_objects)
    {
        obj->applyForce();
    }
}

void RenderThread::calculateForce(Circle* obj)
{
    double Fx = 0.0;
    double Fy = 0.0;

    for(obj_it iterator = m_objects.begin(); iterator != m_objects.end(); iterator++)
    {
        Circle* obj2 = *iterator;
        if(obj == obj2)
            continue;

        // get distance between objects
        int dx = obj2->centerX() - obj->centerX();
        int dy = obj2->centerY() - obj->centerY();
        double r_c = std::sqrt(dx*dx+dy*dy);

        double r = (r_c - obj2->radius() - obj->radius()) / m_scale;

        // limit negative force
        if(r < 0.2)
            r = 0.2;

        // get "force"
        double F = 1.0/r - 1.0/(r*r);

        // get offsets by proportional triangles rule
        if(r_c!=0)
        {
            double factor = ( m_speed * F / obj->mass() ) / r_c;
            Fx += factor * dx;
            Fy += factor * dy;
            //qDebug() << "Force: " << obj << r << F << factor << Fx << Fy ;
        }

    }
    obj->setForce(Fx, Fy);
}

// no Qt in the thread, so use this
unsigned long long RenderThread::getTickCount()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

void RenderThread::setScale(int scale)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(scale < 1) m_scale = 1;
    else if(scale > 400) m_scale = 400;
    else m_scale = scale;
}

void RenderThread::setSpeed(double speed)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_speed = speed;
}
