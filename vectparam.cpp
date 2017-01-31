#include "vectparam.h"
#include "ui_vectparam.h"
#include "main.h"
#include "QtWidgets"

VectParam::VectParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VectParam)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(deltax.cols());
    ui->tableWidget->setRowCount(deltax.rows());
    for(int i=0; i<deltax.rows(); i++){
        for(int j=0; j<deltax.cols(); j++){
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(QString::number(deltax(i,j), 'f')));
        }
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

VectParam::~VectParam()
{
    delete ui;
}

void VectParam::on_pushButton_clicked()
{
    this->close();
}

void VectParam::on_pushButton_2_clicked()
{
    write_vector(deltax, QFileDialog::getSaveFileName(this, tr("Write file"), "~/", tr("Out Files (*.txt)")));
}
