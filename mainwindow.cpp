#include "mainwindow.h"
#include "pointwidget.h"
#include "topocoord.h"
#include "ui_mainwindow.h"
#include "normalmatrixform.h"
#include "korellmatrixofvector.h"
#include "vectparam.h"
#include "QtWidgets"
#include "main.h"

// a - большая полуось эллипсоида
// alpha - сжатие эллипсоида
// N - количество пунктов сети
// r'' - ро
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Подготовка интерфейса, блокируем кнопки
    ui->statusLabel->setText("Готов к работе");
    ui->actionPoints->setEnabled(false);
    ui->actionTopoCoord->setEnabled(false);
    ui->actionKorellMatrixOfVector->setEnabled(false);
    ui->actionMatrixNormUrPopr->setEnabled(false);
    ui->mapSelectButton->setEnabled(false);
    ui->calculateButton->setEnabled(false);
    //DEBUG Тестовое автозаполнение
    ui->AEditText->appendPlainText(QString::number(6378137, 'f', 0));
    ui->AlphaEditText->appendPlainText(QString::number(298.2578, 'f', 4));
    ui->REditText->appendPlainText(QString::number(206265));
    ui->m_AEditText->appendPlainText(QString::number(0.05));
    ui->m_ZEditText->appendPlainText(QString::number(0.05));
    ui->m_SEditText->appendPlainText(QString::number(0.05));
    //DEBUG
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileSelectButton_clicked()
{
    // Проверяем заполненность полей
    if(ui->AEditText->toPlainText().isEmpty() ||
            ui->AlphaEditText->toPlainText().isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Не заполнены обязательные поля");
        msgBox.exec();
    }
    // Получаем путь до файла с координатами
    dots_filename = QFileDialog::getOpenFileName(this,
        tr("Open File"), "~/", tr("Dots Files (*.txt)"));
    if(!dots_filename.isEmpty()){
        // Читаем поля А и Альфы
        CONST_A = ui->AEditText->toPlainText().toDouble();
        CONST_ALPHA = 1/ui->AlphaEditText->toPlainText().toDouble();
        // Запускаем чтение
        read_dots();
        // Обновляем блокировку кнопок
        statusBar()->showMessage(dots_filename);
        ui->statusLabel->setText("Координаты загружены");
        ui->actionPoints->setEnabled(true);
        ui->mapSelectButton->setEnabled(true);
        // Заполняем поле количества точек
        ui->NEditText->appendPlainText(QString::number(CONST_SIZE));
        // Выводим окно с исходными данными
        //PointWidget *pw = new PointWidget();
        //pw->show();
    }
}

void MainWindow::on_mapSelectButton_clicked()
{
    // Получаем карту
    link_map_filename = QFileDialog::getOpenFileName(this,
        tr("Open File"), "~/", tr("Dots Files (*.txt)"));
    if(!link_map_filename.isEmpty()){
        read_link_map();
        statusBar()->showMessage(link_map_filename);
        ui->statusLabel->setText("Карта связей загружена");
        ui->calculateButton->setEnabled(true);  
    }
}

void MainWindow::on_calculateButton_clicked()
{
    // Пробрасываем значения из полей в main.cpp
    R = ui->REditText->toPlainText().toDouble();
    m_A = ui->m_AEditText->toPlainText().toDouble();
    m_Z = ui->m_ZEditText->toPlainText().toDouble();
    m_S = ui->m_SEditText->toPlainText().toDouble();
    // Запуск вычислений
    process();
    // Обновляем интерфейс
    ui->actionTopoCoord->setEnabled(true);
    ui->actionTopoCoord->setEnabled(true);
    ui->actionKorellMatrixOfVector->setEnabled(true);
    ui->actionMatrixNormUrPopr->setEnabled(true);
    // Перезаполнение результата вычислений в случае перезапуска
    ui->MuEditText->clear();
    ui->MuEditText->appendPlainText(QString::number(MU, 'f', 16));
    // Выводим окно с результатами
    TopoCoord *tc = new TopoCoord();
    tc->show();
}

// Триггеры на меню
void MainWindow::on_actionPoints_triggered()
{
    PointWidget *pw = new PointWidget();
    pw->show();
}

void MainWindow::on_actionTopoCoord_triggered()
{
    TopoCoord *tc = new TopoCoord();
    tc->show();
}

void MainWindow::on_actionKorellMatrixOfVector_triggered()
{
    KorellMatrixOfVector *kmov = new KorellMatrixOfVector();
    kmov->show();
}

void MainWindow::on_actionMatrixNormUrPopr_triggered()
{
    NormalMatrixForm *nmf = new NormalMatrixForm();
    nmf->show();
}

void MainWindow::on_actionVectParam_triggered()
{
    VectParam *vp = new VectParam();
    vp->show();
}
