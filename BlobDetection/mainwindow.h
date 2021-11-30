#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "bgblobdetector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void on_pushButtonStart_clicked();

    void on_pushButtonStop_clicked();



    void on_horizontalSliderminTreshold_valueChanged(int value);

    void on_horizontalSlidermaxTreshold_valueChanged(int value);

    void on_checkBoxFilterByArea_stateChanged(int arg1);

    void on_spinBoxAreaMin_valueChanged(int arg1);

    void on_spinBoxAreaMax_valueChanged(int arg1);

    void on_checkBoxFilterByCircularity_stateChanged(int arg1);

    void on_doubleSpinBoxCircularityMin_valueChanged(double arg1);

    void on_doubleSpinBoxCircularityMax_valueChanged(double arg1);

    void on_checkBoxFilterByInertia_stateChanged(int arg1);

    void on_doubleSpinBoxInertiaMin_valueChanged(double arg1);

    void on_doubleSpinBoxInertiaMax_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    int _timerId = -1;

    BGBlobDetector *_bgBlobDetector = nullptr;
};
#endif // MAINWINDOW_H
