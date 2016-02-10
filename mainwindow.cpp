#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    m_renderer = new RenderThread(this);
    m_renderer->startProcess();

    RenderArea* wnd = new RenderArea(m_renderer, this);
    setCentralWidget(wnd);

    QTimer *timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), wnd, SLOT(animate()));

    timer->start(40);


}

MainWindow::~MainWindow()
{
    m_renderer->stopProcess();

    delete ui;
}

void MainWindow::addObjects(int N)
{
    for(int i=0; i<N; i++)
    {
        m_renderer->addObject(new Circle(randInt(300,1000)/*200+50*i*/, 200+50*i, ""));
    }
}

int MainWindow::randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

void MainWindow::on_actionClear_triggered()
{
    m_renderer->removeAllObjects();
}

void MainWindow::on_actionAdd_10_triggered()
{
    addObjects(10);
}

void MainWindow::on_actionAdd_100_triggered()
{
    addObjects(100);
}
