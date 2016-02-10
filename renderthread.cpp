#include "renderthread.h"

#include <QDebug>

RenderThread::RenderThread(QObject *parent) : QThread(parent)
{
    m_nextId = 1;
    m_abort = false;
}

RenderThread::~RenderThread()
{
    stopProcess();
}

void RenderThread::startProcess()
{
    qDebug("execution starting ");

    m_mutex.lock();
    m_abort = false;
    m_mutex.unlock();
    start();
}

void RenderThread::stopProcess()
{
    qDebug("execution stoping ");

    m_mutex.lock();
    m_abort = true;
    m_mutex.unlock();
    wait();

    qDebug("execution stoped ");
}

void RenderThread::addObject(Shape* obj)
{
    qDebug("Add object");
    m_mutex.lock();
    m_objects.insert(std::pair<quint64, Shape*>(m_nextId++, obj));
    m_mutex.unlock();
}

void RenderThread::delObject(quint64 objId)
{
    m_mutex.lock();
    m_objects.erase(objId); // TODO : free Shape* ??????
    m_mutex.unlock();
}

Shape* RenderThread::getObject(quint64 objId)
{
    //m_mutex.lock();
    return m_objects.at(objId);
    //m_mutex.unlock();
}

std::map<quint64, Shape*> * RenderThread::lockData()
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

quint64 RenderThread::hitTest(QPoint point, bool bLock)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for(obj_it iterator = m_objects.begin(); iterator != m_objects.end(); iterator++)
    {
        quint64 key = iterator->first;
        Shape* obj = iterator->second;
        if(obj->hitTest(point.x(), point.y()))
        {
            if(bLock)
                obj->lock(true);
            return key;
        }
    }
    return BAD_ID;
}

void RenderThread::run()
{
    qDebug("thread runing");

    while(!m_abort)
    {
        //qDebug() << "wait" ;
        //m_mutex.lock();
       // qDebug() << "Calculate start" ;
        applyForces();
        //qDebug() << "Calculate stop" ;
        //m_mutex.unlock();
        //qDebug() << "Calculate stop2" ;
        msleep(10);

    }
    qDebug("thread exit");
}

void RenderThread::applyForces()
{
    // calculate forces
    for(obj_it iterator = m_objects.begin(); iterator != m_objects.end(); iterator++)
    {
        // iterator->first = key
        m_mutex.lock();
        Shape* obj = iterator->second;
        if(!obj->isLocked())
            calculateForce(obj);

        //collision(obj);
        m_mutex.unlock();
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
        Shape* obj = iterator->second;

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
                m_mutex.lock();
                obj->applyForce();
                m_mutex.unlock();
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
        Shape* obj2 = iterator->second;
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
        double factor = (Fcur*0.5 / (r_c));//
        Fx += factor * dx;
        Fy += factor * dy;
        //qDebug() << "Force: " << obj << r << Fcur << factor << Fx << Fy ;
        }

    }

    obj->setForce(Fx, Fy, 0);
}

void RenderThread::collision(Shape* obj)
{
    // collision
    for(obj_it iterator2 = m_objects.begin(); iterator2 != m_objects.end(); iterator2++)
    {
        // iterator->first = key
        Shape* obj2 = iterator2->second;
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
