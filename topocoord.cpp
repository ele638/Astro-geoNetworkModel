#include "topocoord.h"
#include "main.h"
#include "ui_topocoord.h"
#include "QtWidgets"

TopoCoord::TopoCoord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopoCoord)
{
    ui->setupUi(this);
    QTabWidget *tabWidget = new QTabWidget(parent);
    ui->verticalLayout->addWidget(tabWidget);
    for(int i=0; i < (int) start_points_calc.size(); i++){
        QWidget *tab = new QWidget(tabWidget);
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);
        QScrollArea *area = new QScrollArea(tab);
        QVBoxLayout *layout = new QVBoxLayout(area);
        QFrame *inner = new QFrame(area);

        inner->setLayout(layout);
        tabLayout->addWidget(area);
        area->setWidgetResizable(true);
        area->setWidget(inner);
        inner->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        tabWidget->addTab(tab, QString("Пункт №%1").arg(QString::number(i+1)));

        Start_Point current_point = start_points_calc.at(i);
        //Заголовок
        QLabel *pointName = new QLabel();
        pointName->setText(QString("Начальный пункт №%1:").arg(QString::number(i+1)));
        pointName->setAlignment(Qt::AlignCenter);
        QFont font = pointName->font();
        font.setBold(true);
        font.setPointSize(18);
        pointName->setFont(font);
        layout->addWidget(pointName);
        QLabel *horRectCoordLabel = new QLabel();
        horRectCoordLabel->setText("Пространственные топоцентрические горизонтные прямоугольные координаты");
        layout->addWidget(horRectCoordLabel);

        Start_Point current_zagr = start_points_zagr.at(i);
        //Матрица координат x, y, z
        QTableWidget *horRectCoordTable = new QTableWidget();
        horRectCoordTable->setRowCount(4);
        int column_count=0;
        for(int j=0; j < (int) start_points_calc.size();j++) {
            if(link_map(i,j) == 1) column_count++;
        }
        horRectCoordTable->setColumnCount(column_count);
        QStringList *header = new QStringList();
        for(int j=0, k=0; j < (int) start_points_calc.size(); j++){
            if(link_map(i,j) == 1){
                header->push_back(QString::number(j+1));
                horRectCoordTable->setItem(0,k,new QTableWidgetItem(QString::number(current_point.xp.at(j))));
                horRectCoordTable->setItem(1,k,new QTableWidgetItem(QString::number(current_point.yp.at(j))));
                horRectCoordTable->setItem(2,k,new QTableWidgetItem(QString::number(current_point.zp.at(j))));
                horRectCoordTable->setItem(3,k,new QTableWidgetItem(QString::number(current_point.dp.at(j))));
                k++;
            }
        }
        horRectCoordTable->setHorizontalHeaderLabels(*header);
        horRectCoordTable->setVerticalHeaderLabels(QString("X\',Y\',Z\',D\'").split(','));
        horRectCoordTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        horRectCoordTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        layout->addWidget(horRectCoordTable);

        //Пространственные топоцентрические горизонтные сферические координаты
        QLabel *horSphereCoordLabel = new QLabel();
        horSphereCoordLabel->setText("Пространственные топоцентрические горизонтные сферические координаты");
        horSphereCoordLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(horSphereCoordLabel);

        QTableWidget *horSpereCoordTable = new QTableWidget();
        horSpereCoordTable->setRowCount(9);
        horSpereCoordTable->setColumnCount(column_count*3);
        QStringList *header2 = new QStringList();
        for(int j=0, k=0; j < (int) start_points_calc.size(); j++){
            if(link_map(i,j) == 1){
                header2->push_back(QString::number(j+1));
                header2->push_back("");
                header2->push_back("");
                //S_clean
                horSpereCoordTable->setItem(0,k,new QTableWidgetItem(QString::number(current_point.sf.at(j),'f',8)));
                horSpereCoordTable->setSpan(0,k,1,3);

                //S_error
                horSpereCoordTable->setItem(1,k,new QTableWidgetItem(QString::number(current_point.sf.at(j) - current_zagr.sf.at(j),'f',8)));
                horSpereCoordTable->setSpan(1,k,1,3);

                //S_dirty
                horSpereCoordTable->setItem(2,k,new QTableWidgetItem(QString::number(current_zagr.sf.at(j),'f',8)));
                horSpereCoordTable->setSpan(2,k,1,3);

                //Z_clean
                horSpereCoordTable->setItem(3,k,new QTableWidgetItem(QString::number(current_point.z.at(j).g,'f',0)));
                horSpereCoordTable->setItem(3,k+1,new QTableWidgetItem(QString::number(current_point.z.at(j).m,'f',0)));
                horSpereCoordTable->setItem(3,k+2,new QTableWidgetItem(QString::number(current_point.z.at(j).s,'f',8)));

                //Z_error
                horSpereCoordTable->setItem(4,k,new QTableWidgetItem(QString::number(current_point.z.at(j).value - current_zagr.z.at(j).value,'f',8)));
                horSpereCoordTable->setSpan(4,k,1,3);

                //Z_dirty
                horSpereCoordTable->setItem(5,k,new QTableWidgetItem(QString::number(current_zagr.z.at(j).g,'f',0)));
                horSpereCoordTable->setItem(5,k+1,new QTableWidgetItem(QString::number(current_zagr.z.at(j).m,'f',0)));
                horSpereCoordTable->setItem(5,k+2,new QTableWidgetItem(QString::number(current_zagr.z.at(j).s,'f',8)));

                //A_clean
                horSpereCoordTable->setItem(6,k,new QTableWidgetItem(QString::number(current_point.a.at(j).g,'f',0)));
                horSpereCoordTable->setItem(6,k+1,new QTableWidgetItem(QString::number(current_point.a.at(j).m,'f',0)));
                horSpereCoordTable->setItem(6,k+2,new QTableWidgetItem(QString::number(current_point.a.at(j).s,'f',8)));

                //A_error
                horSpereCoordTable->setItem(7,k,new QTableWidgetItem(QString::number(current_point.a.at(j).value - current_zagr.a.at(j).value,'f',8)));
                horSpereCoordTable->setSpan(7,k,1,3);

                //A_dirty
                horSpereCoordTable->setItem(8,k,new QTableWidgetItem(QString::number(current_zagr.a.at(j).g,'f',0)));
                horSpereCoordTable->setItem(8,k+1,new QTableWidgetItem(QString::number(current_zagr.a.at(j).m,'f',0)));
                horSpereCoordTable->setItem(8,k+2,new QTableWidgetItem(QString::number(current_zagr.a.at(j).s,'f',8)));

                k+=3;
            }
        }
        horSpereCoordTable->setHorizontalHeaderLabels(*header2);
        horSpereCoordTable->setVerticalHeaderLabels(QString("s_clean,s_error,s,z_clean,z_error,z,A_clean,A_error,A").split(','));
        //horSpereCoordTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        horSpereCoordTable->resizeColumnsToContents();
        horSpereCoordTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        layout->addWidget(horSpereCoordTable);
    }
}

TopoCoord::~TopoCoord()
{
    delete ui;
}
