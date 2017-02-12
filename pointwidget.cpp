#include "pointwidget.h"
#include "main.h"
#include "ui_pointwidget.h"
#include "QTableWidget"

PointWidget::PointWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointWidget)
{
    ui->setupUi(this);
    QTableWidget *coordTable = ui->CoordTable;
    QTableWidget *radiansTable = ui->RadiansTable;
    coordTable->setColumnCount(7);
    coordTable->setRowCount(dots.size());
    coordTable->setHorizontalHeaderLabels(QString("B,,,L,,,H").split(","));
    coordTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    coordTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    radiansTable->setColumnCount(6);
    radiansTable->setRowCount(dots.size());
    radiansTable->setHorizontalHeaderLabels(QString("B,L,H,X,Y,Z").split(","));
    radiansTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    radiansTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for(int i=0; i < (int) dots.size(); i++){
        Dot dot = dots.at(i);
        coordTable->setItem(i, 0, new QTableWidgetItem(QString::number(dot.values.at(0),'f',0)));
        coordTable->setItem(i, 1, new QTableWidgetItem(QString::number(dot.values.at(1),'f',0)));
        coordTable->setItem(i, 2, new QTableWidgetItem(QString::number(dot.values.at(2),'f',6)));
        coordTable->setItem(i, 3, new QTableWidgetItem(QString::number(dot.values.at(3),'f',0)));
        coordTable->setItem(i, 4, new QTableWidgetItem(QString::number(dot.values.at(4),'f',0)));
        coordTable->setItem(i, 5, new QTableWidgetItem(QString::number(dot.values.at(5),'f',6)));
        coordTable->setItem(i, 6, new QTableWidgetItem(QString::number(dot.values.at(6),'f',0)));

        radiansTable->setItem(i, 0, new QTableWidgetItem(QString::number(dot.Q.b,'f',6)));
        radiansTable->setItem(i, 1, new QTableWidgetItem(QString::number(dot.Q.l,'f',6)));
        radiansTable->setItem(i, 2, new QTableWidgetItem(QString::number(dot.Q.h,'f',0)));
        radiansTable->setItem(i, 3, new QTableWidgetItem(QString::number(dot.G.x,'f',3)));
        radiansTable->setItem(i, 4, new QTableWidgetItem(QString::number(dot.G.y,'f',3)));
        radiansTable->setItem(i, 5, new QTableWidgetItem(QString::number(dot.G.z,'f',3)));
    }
}

PointWidget::~PointWidget()
{
    delete ui;
}
