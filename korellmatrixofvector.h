#ifndef KORELLMATRIXOFVECTOR_H
#define KORELLMATRIXOFVECTOR_H

#include <QDialog>

namespace Ui {
class KorellMatrixOfVector;
}

class KorellMatrixOfVector : public QDialog
{
    Q_OBJECT

public:
    explicit KorellMatrixOfVector(QWidget *parent = 0);
    ~KorellMatrixOfVector();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::KorellMatrixOfVector *ui;
};

#endif // KORELLMATRIXOFVECTOR_H
