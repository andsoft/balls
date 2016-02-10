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
    connect(timer, SIGNAL(timeout()), wnd, SLOT(animate()));

    timer->start(10);


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
        for(int j=0; j<N; j++)
        m_renderer->addObject(new Circle(/*randInt(10,1000)*/200+40*j, 200+40*i, ""));
    }
}

int MainWindow::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}
