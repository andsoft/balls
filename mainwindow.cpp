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

    m_wnd = new RenderArea(m_renderer, this);
    setCentralWidget(m_wnd);

    on_slider_Speed_valueChanged(ui->slider_Speed->value());
    on_slider_Scale_valueChanged(ui->slider_Scale->value());
    on_slider_Radius_valueChanged(ui->slider_Radius->value());
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
        m_renderer->addObject(new Circle(randInt(20,rect.width()-20),
                                         randInt(20,rect.height()-20),
                                         m_wnd->m_radius));
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
    v /= 1000;
    ui->label_speed->setText(QString::number(v, 'f', 3));
    m_renderer->setSpeed(v);
}

void MainWindow::on_slider_Scale_valueChanged(int value)
{
    ui->label_scale->setText(QString::number(value));
    m_renderer->setScale(value);
}

void MainWindow::on_slider_Radius_valueChanged(int value)
{
    ui->label_radius->setText(QString::number(value));
    m_wnd->m_radius = value;
}
