#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    void on_fileSelectButton_clicked();

    void on_mapSelectButton_clicked();

    void on_calculateButton_clicked();

    void on_actionPoints_triggered();

    void on_actionTopoCoord_triggered();

    void on_actionKorellMatrixOfVector_triggered();

    void on_actionMatrixNormUrPopr_triggered();

    void on_actionVectParam_triggered();

    void on_actionOutAll_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
