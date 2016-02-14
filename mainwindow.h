#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "renderthread.h"
#include "renderarea.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addObjects(int N);

protected:
    int randInt(int low, int high);

protected:
    RenderThread *m_renderer;
    RenderArea *m_wnd;

private slots:
    void on_actionClear_triggered();

    void on_actionAdd_10_triggered();

    void on_actionAdd_100_triggered();

    void on_slider_Speed_valueChanged(int value);

    void on_slider_Scale_valueChanged(int value);

    void on_slider_Radius_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
