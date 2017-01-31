#include "normalmatrixform.h"
#include "ui_normalmatrixform.h"
#include "main.h"
#include "QtWidgets"

NormalMatrixForm::NormalMatrixForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NormalMatrixForm)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(matrixNormal.cols());
    ui->tableWidget->setRowCount(matrixNormal.rows());
    for(int i=0; i<matrixNormal.rows(); i++){
        for(int j=0; j<matrixNormal.cols(); j++){
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(QString::number(matrixNormal(i,j), 'f')));
        }
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

NormalMatrixForm::~NormalMatrixForm()
{
    delete ui;
}

void NormalMatrixForm::on_pushButton_clicked()
{
    delete ui;
    this->close();
}

void NormalMatrixForm::on_pushButton_2_clicked()
{
    write_matrix(matrixNormal, QFileDialog::getSaveFileName(this, tr("Write file"), "~/", tr("Out Files (*.txt)")));
}
