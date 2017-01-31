#ifndef NORMALMATRIXFORM_H
#define NORMALMATRIXFORM_H

#include <QDialog>

namespace Ui {
class NormalMatrixForm;
}

class NormalMatrixForm : public QDialog
{
    Q_OBJECT

public:
    explicit NormalMatrixForm(QWidget *parent = 0);
    ~NormalMatrixForm();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::NormalMatrixForm *ui;
};

#endif // NORMALMATRIXFORM_H
