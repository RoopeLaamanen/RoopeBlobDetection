#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _bgBlobDetector = new BGBlobDetector(this);

    _timerId = startTimer(500);
}

MainWindow::~MainWindow()
{
    if(_timerId >= 0)
    {
        killTimer(_timerId);
        _timerId = -1;
    }


    if(_bgBlobDetector != nullptr)
    {
        delete _bgBlobDetector;
        _bgBlobDetector = nullptr;
    }
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(_bgBlobDetector != nullptr)
    {
        ui->horizontalSliderminTreshold->setValue(_bgBlobDetector->MinThreshold);
        ui->horizontalSlidermaxTreshold->setValue(_bgBlobDetector->MaxThreshold);
        ui->labelMinThresholdValue->setText(QString::number(_bgBlobDetector->MinThreshold));
        ui->labelmaxThresholdValue->setText(QString::number(_bgBlobDetector->MaxThreshold));

        ui->checkBoxFilterByArea->setChecked(_bgBlobDetector->FilterbyArea);
        ui->spinBoxAreaMin->setValue(_bgBlobDetector->MinArea);
        ui->spinBoxAreaMax->setValue(_bgBlobDetector->MaxArea);

        ui->checkBoxFilterByCircularity->setChecked(_bgBlobDetector->FilterByCircularity);
        ui->doubleSpinBoxCircularityMin->setValue(_bgBlobDetector->MinCircularity);
        ui->doubleSpinBoxCircularityMax->setValue(_bgBlobDetector->MaxCircularity);

        ui->checkBoxFilterByInertia->setChecked(_bgBlobDetector->FilterByInertia);
        ui->doubleSpinBoxInertiaMin->setValue(_bgBlobDetector->MinInertia);
        ui->doubleSpinBoxInertiaMax->setValue(_bgBlobDetector->MaxInertia);

        ui->plainTextEditContentText->setPlainText(_bgBlobDetector->ContentText);
        ui->plainTextEditContentText->appendPlainText(QString("Confidence=%1").arg(_bgBlobDetector->Confidence));
        ui->plainTextEditContentText->appendPlainText(QString("Angle=%1").arg(_bgBlobDetector->Angle));
        ui->plainTextEditContentText->appendPlainText(QString("PositionX=%1").arg(_bgBlobDetector->PositionX));
        ui->plainTextEditContentText->appendPlainText(QString("PositionY=%1").arg(_bgBlobDetector->PositionY));
    }
}


void MainWindow::on_pushButtonStart_clicked()
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->Start();
    }
}

void MainWindow::on_pushButtonStop_clicked()
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->Stop();
    }
}

void MainWindow::on_horizontalSliderminTreshold_valueChanged(int value)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->MinThreshold = (uchar)value;
    }

}

void MainWindow::on_horizontalSlidermaxTreshold_valueChanged(int value)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->MaxThreshold = (uchar)value;
    }
}

void MainWindow::on_checkBoxFilterByArea_stateChanged(int arg1)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->FilterbyArea = ui->checkBoxFilterByArea->isChecked();
    }
}

void MainWindow::on_spinBoxAreaMin_valueChanged(int arg1)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->MinArea = ui->spinBoxAreaMin->value();
    }
}

void MainWindow::on_spinBoxAreaMax_valueChanged(int arg1)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->MaxArea = ui->spinBoxAreaMax->value();
    }
}

void MainWindow::on_checkBoxFilterByCircularity_stateChanged(int arg1)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->FilterByCircularity = ui->checkBoxFilterByCircularity->isChecked();
    }
}

void MainWindow::on_doubleSpinBoxCircularityMin_valueChanged(double arg1)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->MinCircularity = ui->doubleSpinBoxCircularityMin->value();
    }
}

void MainWindow::on_doubleSpinBoxCircularityMax_valueChanged(double arg1)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->MaxCircularity = ui->doubleSpinBoxCircularityMax->value();
    }
}

void MainWindow::on_checkBoxFilterByInertia_stateChanged(int arg1)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->FilterByInertia = ui->checkBoxFilterByInertia->isChecked();
    }
}

void MainWindow::on_doubleSpinBoxInertiaMin_valueChanged(double arg1)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->MinInertia = ui->doubleSpinBoxInertiaMin->value();
    }
}

void MainWindow::on_doubleSpinBoxInertiaMax_valueChanged(double arg1)
{
    if(_bgBlobDetector != nullptr)
    {
        _bgBlobDetector->MaxInertia = ui->doubleSpinBoxInertiaMax->value();
    }
}
