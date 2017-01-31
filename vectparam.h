#ifndef VECTPARAM_H
#define VECTPARAM_H

#include <QDialog>

namespace Ui {
class VectParam;
}

class VectParam : public QDialog
{
    Q_OBJECT

public:
    explicit VectParam(QWidget *parent = 0);
    ~VectParam();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::VectParam *ui;
};

#endif // VECTPARAM_H
