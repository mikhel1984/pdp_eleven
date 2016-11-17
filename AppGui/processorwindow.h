#ifndef PROCESSORWINDOW_H
#define PROCESSORWINDOW_H

#include <QMainWindow>
#include <stdint.h>
#include <QLineEdit>
#include <QKeyEvent>
#include <QTimer>
#include "processorthread.h"

class QGraphicsScene;
class QImage;

namespace Ui {
class ProcessorWindow;
}

class ProcessorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProcessorWindow(QWidget *parent = 0);
    ~ProcessorWindow();

protected:
    void keyPressEvent( QKeyEvent *k );

private slots:
    void on_assemblyButton_clicked();

    void on_runButton_clicked();

    void on_loadButton_clicked();

    void on_intBaseComboBox_activated(const QString &arg1);

    void on_loadFromFileButton_clicked();

    void on_resetButton_clicked();

    void on_stepButton_clicked();

    void on_refreshMemoryButton_clicked();

    void update_picture();

private:
    Ui::ProcessorWindow *ui;

    const uint16_t monitorHeight;
    const uint16_t monitorWidth;
    void setRegister(int number, QLineEdit* edit);
    void setRegisters();

    ProcessorThread* pthr;
    QTimer* timer;
};

#endif // PROCESSORWINDOW_H
