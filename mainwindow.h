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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
