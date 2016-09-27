#ifndef PROCESSORWINDOW_H
#define PROCESSORWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QImage;
class QTimer;

namespace Ui {
class ProcessorWindow;
}

class ProcessorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProcessorWindow(QWidget *parent = 0);
    ~ProcessorWindow();

private slots:
    void on_runButton_clicked();

    void on_stopButton_clicked();

    void on_resetButton_clicked();

    void update_monitor();

private:
    void redrawMonitor();
    void nextStep();

    QGraphicsScene *gScene;
    QImage *gImage;
    QTimer *timer;

    int step; // for testing

    Ui::ProcessorWindow *ui;



};

#endif // PROCESSORWINDOW_H
