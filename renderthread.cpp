#include "renderthread.h"

#include <QDebug>

RenderThread::RenderThread(QObject *parent) : QThread(parent)
{
    m_abort = false;
    m_calcTime = 0;
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
    qDebug("execution stoping ");
    m_abort.store(true);
    wait();
    qDebug("execution stoped ");
}

void RenderThread::removeAllObjects()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for(Shape *obj : m_objects)
        delete obj;
    m_objects.clear();
}

void RenderThread::addObject(Shape* obj)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_objects.push_back(obj);
}

void RenderThread::delObject(Shape* obj)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_objects.remove(obj);
    delete obj;
}

std::list<Shape *> *RenderThread::lockData()
{
    //qDebug("lock data");
    m_mutex.lock();
    return &m_objects;
}

void RenderThread::unlockData()
{
    //qDebug("unlock data");
    m_mutex.unlock();
}

Shape *RenderThread::hitTest(QPoint point, bool bLock)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for(obj_it iterator = m_objects.begin(); iterator != m_objects.end(); iterator++)
    {
        Shape* obj = *iterator;
        if(obj->hitTest(point.x(), point.y()))
        {
            if(bLock)
                obj->lock(true);
            return obj;
        }
    }
    return BAD_ID;
}

void RenderThread::run()
{
    qDebug("thread runing");

    while(!m_abort.load())
    {
        //qDebug() << "wait" ;
        auto t0 = getTickCount();

        m_mutex.lock();

       // qDebug() << "Calculate start" ;
        applyForces();
        //qDebug() << "Calculate stop" ;

        m_mutex.unlock();
        //qDebug() << "Calculate stop2" ;
        m_calcTime = getTickCount() - t0;

        msleep(1);

    }
    qDebug("thread exit");
}

void RenderThread::applyForces()
{
    // calculate forces

    for(obj_it iterator = m_objects.begin(); iterator != m_objects.end(); iterator++)
    {
        // iterator->first = key
        //m_mutex.lock();
        Shape* obj = *iterator;
        if(!obj->isLocked())
            calculateForce(obj);

        //collision(obj);
        //m_mutex.unlock();
 //       msleep(500);

//        m_mutex.lock();
//        obj->applyForce();
//        m_mutex.unlock();
//        msleep(500);
    }

    // apply forces
    for(obj_it iterator = m_objects.begin(); iterator != m_objects.end(); iterator++)
    {
        // iterator->first = key
        Shape* obj = *iterator;

/*
        // collision
        for(obj_it iterator2 = m_objects.begin(); iterator2 != m_objects.end(); iterator2++)
        {
            // iterator->first = key
            Shape* obj2 = iterator2->second;
            if(obj == obj2)
                continue;

            double Fx = obj->forceX();
            double Fy = obj->forceY();

            int dx = obj2->centerX() - obj->centerX()+Fx;
            int dy = obj2->centerY() - obj->centerY()+Fy ;
            int r_c = std::sqrt(dx*dx+dy*dy);

            if( r_c < 2*20 )
            {
                double factor = (r - 2*20)/r;
                Fx *= factor;
                Fy *= factor;
            }
        }
*/
                //m_mutex.lock();
                obj->applyForce();
                //m_mutex.unlock();
            //    msleep(500);
    }
}

void RenderThread::calculateForce(Shape* obj)
{
    int F = 0; // resulting force
    double Fx = 0;
    double Fy = 0;
    int A = 0; // resulting angle

    for(obj_it iterator = m_objects.begin(); iterator != m_objects.end(); iterator++)
    {
        // iterator->first = key
        Shape* obj2 = *iterator;
        if(obj == obj2)
            continue;

        // get distance between objects
        int dx = obj2->centerX() - obj->centerX();
        int dy = obj2->centerY() - obj->centerY();
        double r_c = std::sqrt(dx*dx+dy*dy);

        double r = (r_c - 2*20)/20;

        if(r < 0.2) r = 0.2;
        double Fcur = 0.1;
        //if(r>0.2)
        {
            //qDebug() << "dx,dy,r: " << dx << dy << r ;

            // get force
            Fcur = 1.0/r - 1.0/(r*r);


        }
        //else
        {
            //obj->lock(true);
            //obj2->lock(true);
        }
        double X = 0.0;
        double Y = 0.0;

        if((dx!=0)||(dy!=0))
        {


        // get offset
        double factor = (Fcur*1 / (r_c));//
        Fx += factor * dx;
        Fy += factor * dy;
        //qDebug() << "Force: " << obj << r << Fcur << factor << Fx << Fy ;
        }

    }
//qDebug() << "Force: " << obj << Fx << Fy ;
    obj->setForce(Fx, Fy, 0);
}

void RenderThread::collision(Shape* obj)
{
    // collision
    for(obj_it iterator2 = m_objects.begin(); iterator2 != m_objects.end(); iterator2++)
    {
        // iterator->first = key
        Shape* obj2 = *iterator2;
        if(obj == obj2)
            continue;

        double Fx = obj->forceX();
        double Fy = obj->forceY();
        double Fr = std::sqrt(Fx*Fx+Fy*Fy);

        int dx = obj2->centerX() - obj->centerX();
        int dy = obj2->centerY() - obj->centerY();
        int r_c = std::sqrt(dx*dx+dy*dy);

        if( Fr > r_c - 2*20 )
        {
            Fr = r_c - 2*20;
            double factor = Fr/r_c;
            Fx = factor * dx;
            Fy = factor * dy;
            obj->setForce(Fx, Fy, 0);
        }
    }
}

// no Qt in the thread, so use this
unsigned long long RenderThread::getTickCount()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}
