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

    RenderArea* wnd = new RenderArea(m_renderer, this);
    setCentralWidget(wnd);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addObjects(int N)
{
    for(int i=0; i<N; i++)
    {
        QRect rect = centralWidget()->rect();
        m_renderer->addObject(new Circle(randInt(20,rect.width()-20)/*200+50*i*/,
                                         randInt(20,rect.height()-20)/*200+50*i*/));
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

void MainWindow::on_slider_Speed_valueChanged(int value)
{
    double v = value;
    m_renderer->setSpeed(v/50);
}

void MainWindow::on_slider_Scale_valueChanged(int value)
{
    m_renderer->setScale(value);
}
