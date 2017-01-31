#include "korellmatrixofvector.h"
#include "ui_korellmatrixofvector.h"
#include "main.h"
#include "QtWidgets"

KorellMatrixOfVector::KorellMatrixOfVector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KorellMatrixOfVector)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(deltaX.cols());
    ui->tableWidget->setRowCount(deltaX.rows());
    for(int i=0; i<deltaX.rows(); i++){
        for(int j=0; j<deltaX.cols(); j++){
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(QString::number(deltaX(i,j), 'f', 5)));
        }
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

KorellMatrixOfVector::~KorellMatrixOfVector()
{
    delete ui;
}

void KorellMatrixOfVector::on_pushButton_clicked()
{
    delete ui;
    this->close();
}

void KorellMatrixOfVector::on_pushButton_2_clicked()
{
    write_matrix(deltaX, QFileDialog::getSaveFileName(this, tr("Write file"), "~/", tr("Out Files (*.txt)")));
}
