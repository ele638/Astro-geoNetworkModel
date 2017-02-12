#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include "mainwindow.h"
#include <QApplication>
#include "QFile"
#include "QTextStream"
#include "QString"
#include "QMessageBox"
#include "Eigen/Dense"
using namespace std;
using namespace Eigen;

int CONST_SIZE;
double CONST_A, CONST_ALPHA, R, MU, m_A, m_Z, m_S;
QString dots_filename, link_map_filename;
MatrixXd link_map;

/* N = 8
 * A = 6378137.0
 * Alpha = 0.003352804184836
 * R = 206265
 */

class Angle{
public:
    int g, m;
    double s, value;
    // Методы инициализации
    Angle(int grad, int min, double sec){
        g = grad;
        m = min;
        s = sec;
        value = to_rad(); // автоматическое вычисление значения в радианах
    }

    Angle(double val){
        value = val;
        to_grad();
    }

    Angle operator+(double val){
        return(Angle(this->value+val));
    }

    double to_rad(){
        return ((s/60.0 + m)/60.0 + g)*M_PI/180.0;
    }

    void to_grad(){
        g = (int) value;
        m = (int) ((value - g)*60.0);
        s = ((value - g)*60 - m)*60;
        if( s>60.0 ) {
            m+=1.0;
            s-=60.0;
        }
        if(m>60){
            g+=1;
            m-=60;
        }
    }

};

class Dot{
public:
    class Ellips_coord{
    public:
        double b, l, h;
        Ellips_coord(){
            b = 0; l = 0; h = 0;
        }

        Ellips_coord(std::vector<double> v){
            b = ((v.at(2)/60.0 + v.at(1))/60.0 + v.at(0))*M_PI/180.0;
            l = ((v.at(5)/60.0 + v.at(4))/60.0 + v.at(3))*M_PI/180.0;
            h = v.at(6);
        }
    };

    class Geocentr_coord{
    public:
        double x, y, z;
        Geocentr_coord(){
            x = 0; y = 0; z = 0;
        }

        Geocentr_coord(double x1, double y1, double z1){
            x = x1; y = y1; z = z1;
        }

        Geocentr_coord(Ellips_coord in ){
            double e2 = CONST_ALPHA * ( 2.0 - CONST_ALPHA );
            double n = CONST_A / sqrt( 1.0 - e2 * pow( sin(in.b), 2.0 ));
            x = ( n + in.h ) * cos(in.b) * cos(in.l);
            y = ( n + in.h ) * cos(in.b) * sin(in.l);
            z = ( n + in.h - e2 * n) * sin(in.b);
        }

        Geocentr_coord operator-(Geocentr_coord g){
            return Geocentr_coord(x - g.x, y - g.y, z - g.z);
        }

    };

    Ellips_coord Q;
    Geocentr_coord G;
    std::vector<double> values;

    Dot(std::vector<double> v){
        values = v;
        Q = Ellips_coord(v);
        G = Geocentr_coord(Q);
    }
};


class Start_Point{
public:
    int number;
    std::vector<Angle> a, z;
    std::vector<double> xp, yp, zp, dp, sf;
    Dot* dot;
    Matrix3d aMatrix;
    std::vector<Matrix3d> bMatrix;


    Matrix3d aM(double b, double l){
        Matrix3d matrix(3,3);
        matrix(0,0) = -sin(b)*cos(l);
        matrix(0,1) = -sin(l);
        matrix(0,2) = cos(b)*cos(l);
        matrix(1,0) = -sin(b)*sin(l);
        matrix(1,1) = cos(l);
        matrix(1,2) = cos(b)*sin(l);
        matrix(2,0) = cos(b);
        matrix(2,1) = 0;
        matrix(2,2) = sin(b);
        return matrix;
    }

    Matrix3d bM(double b1, double l1, double bi, double li){
        Matrix3d matrix(3,3);
        matrix(0,0) = cos(b1)*cos(bi) + sin(b1)*sin(bi)*cos(li-l1);
        matrix(0,1) = sin(b1)*sin(li-l1);
        matrix(0,2) = cos(b1)*sin(bi) - sin(b1)*cos(bi)*cos(li-l1);
        matrix(1,0) = -sin(b1)*sin(li-l1);
        matrix(1,1) = cos(li-l1);
        matrix(1,2) = cos(bi)*sin(li-l1);
        matrix(2,0) = sin(b1)*cos(bi) - cos(b1)*sin(bi)*cos(li-l1);
        matrix(2,1) = -cos(b1)*sin(li-l1);
        matrix(2,2) = sin(b1)*sin(bi) + cos(b1)*cos(bi)*cos(li-l1);
        return matrix;
    }

    Start_Point(Dot* indot, int index){
        dot = indot;
        aMatrix = aM(dot->Q.b, dot->Q.l).transpose();
        this->number = index;
    }

    // Деструктор класса
    ~Start_Point(){
        z.clear();
        a.clear();
        xp.clear();
        yp.clear();
        zp.clear();
        dp.clear();
        sf.clear();
        bMatrix.clear();
    }

    void calculate(std::vector<Dot> dots){
        // Проход по всем точкам
        for(int i=0; i<CONST_SIZE; i++){
            // Если связи между точками, забиваем значения нулями
            if (link_map(number, i) == 0){
                xp.push_back(0.0);
                yp.push_back(0.0);
                zp.push_back(0.0);
                dp.push_back(0.0);
                sf.push_back(0.0);
                a.push_back(Angle(0));
                z.push_back(Angle(0));
                bMatrix.push_back(Matrix3d());
            }else{
                // Нахождения координат X', Y', Z', D', s, A и матрицу B
                Dot::Geocentr_coord diff(dots.at(i).G - dot->G);
                xp.push_back(aMatrix(0,0) * diff.x +
                        aMatrix(0,1) * diff.y +
                        aMatrix(0,2) * diff.z);
                yp.push_back(aMatrix(1,0) * diff.x +
                        aMatrix(1,1) * diff.y +
                        aMatrix(1,2) * diff.z);
                zp.push_back(aMatrix(2,0) * diff.x +
                        aMatrix(2,1) * diff.y +
                        aMatrix(2,2) * diff.z);
                dp.push_back(sqrt(pow(xp[i], 2.0) + pow(yp[i], 2.0)));
                sf.push_back(sqrt(pow(xp[i], 2.0) + pow(yp[i], 2.0) + pow(zp[i], 2.0)));
                a.push_back(Angle(180.0+(atan(yp[i]/xp[i]))*180.0/M_PI));
                z.push_back(Angle(180.0+(atan(dp[i]/zp[i]))*180.0/M_PI));
                bMatrix.push_back(bM(dot->Q.b, dot->Q.l, dots.at(i).Q.b, dots.at(i).Q.l));
            }
        }

    }

    // Метод загрубления координат
    void zagr(){
        // Создаем новые массивы s, A, z, чтобы не портить старые значения
        std::vector<double> sftemp;
        std::vector<Angle> atemp, ztemp;
        for(int i=0; i<CONST_SIZE; i++){
            if (link_map(number, i) == 0){
                atemp.push_back(Angle(0));
                ztemp.push_back(Angle(0));
                sftemp.push_back(0.0);
            }else{
                atemp.push_back(Angle(a.at(i).value + ((m_A/3600)*sqrt(-2.0*log((double)rand() / RAND_MAX)) * sin( 2* M_PI* ((double)rand() / RAND_MAX)))));
                ztemp.push_back(Angle(z.at(i).value + ((m_Z/3600)*sqrt(-2.0*log((double)rand() / RAND_MAX)) * sin( 2* M_PI* ((double)rand() / RAND_MAX)))));
                sftemp.push_back(sf.at(i) + m_S*sqrt(-2.0*log((double)rand() / RAND_MAX)) * sin( 2*M_PI* ((double)rand() / RAND_MAX) ));
            }
        }
        // Перепривязываем указатели на объекте
        this->sf = sftemp;
        this->a = atemp;
        this->z = ztemp;
    }

};

std::vector<Start_Point> start_points_calc, start_points_zagr;
std::vector<Dot> dots;

MatrixXd matrixB, matrixNormal, deltaX;
VectorXd deltax, vectorV, vectorW, vectorL;

void write_matrix(MatrixXd& matrix, QString path ){
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    if(file.isOpen()){
        QTextStream out(&file);
        out.setCodec("UTF-8");
        QString temp;
        for(int i=0; i<matrix.rows(); i++){
            for(int j=0; j<matrix.cols(); j++){
                temp.append(QString::number(matrix(i,j), 'f', 5) + "\t");
            }
            temp.append('\n');
        }
        out << temp;
    }
    file.close();
}

void write_vector(VectorXd& vector, QString path ){
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    if(file.isOpen()){
        QTextStream out(&file);
        out.setCodec("UTF-8");
        QString temp;
        for(int i=0; i<vector.rows(); i++){
            temp.append(QString::number(vector(i), 'f', 5) + "\t");
        }
        out << temp;
    }
    file.close();
}


// Генерация матрицы коэффициентов уравнений поправок
void matrixB_calc(){
    // Задаем переменную (matixB - ошибочное наименование переменной, но лень рефакторить)
    matrixB = MatrixXd((CONST_SIZE-1)*CONST_SIZE*3, CONST_SIZE*3);
    // Сектор под Z
    int zPlace = (CONST_SIZE-1)*CONST_SIZE;
    // Сектор под S
    int sPlace = (CONST_SIZE-1)*CONST_SIZE*2;
    // Обнуляем матрицу
    matrixB.setZero((CONST_SIZE-1)*CONST_SIZE*3, CONST_SIZE*3);
    // Вспомогательные переменные и массивы ошибок
    int k=0, filled1=0, filled2=0;
    std::vector<double> errorsA, errorsZ, errorsS, errors;
    // Проходим по всем загрубленным пунктам
    for(int m=0; m<start_points_zagr.size(); m++){
       // Ловим текущие пункты в цикле (загрубленные и вычисленные значения)
       Start_Point current_point = start_points_zagr.at(m), current_point_calc = start_points_calc.at(m);
       // Идем по всем точкам данного пункта
       for(int i=0; i<CONST_SIZE; i++){
           // Если не замыкаемся на себя или если есть связь
           if(link_map(i, current_point.number) == 1.0){
               // Идем по всем соседним точкам
               for(int j=0; j<CONST_SIZE; j++){
                   // Если совпал индекс левой связи и не было заполнено раньше
                   if(current_point.number == j && filled1==0){
                       //A
                       matrixB(k, j*3) = sin(current_point.a.at(i).to_rad())/current_point.dp.at(i)*R;
                       matrixB(k, (j*3)+1) = -cos(current_point.a.at(i).to_rad())/current_point.dp.at(i)*R;
                       matrixB(k, (j*3)+2) = 0;
                       //Z
                       matrixB(zPlace+k, j*3) = cos(current_point.a.at(i).to_rad())*cos(current_point.z.at(i).to_rad())/(-current_point.sf.at(i))*R;
                       matrixB(zPlace+k, (j*3)+1) = sin(current_point.a.at(i).to_rad())*cos(current_point.z.at(i).to_rad())/(-current_point.sf.at(i))*R;
                       matrixB(zPlace+k, (j*3)+2) = sin(current_point.z.at(i).to_rad())/current_point.sf.at(i)*R;
                       //S
                       matrixB(sPlace+k, j*3) = -cos(current_point.a.at(i).to_rad())*sin(current_point.z.at(i).to_rad());
                       matrixB(sPlace+k, (j*3)+1) = -sin(current_point.a.at(i).to_rad())*sin(current_point.z.at(i).to_rad());
                       matrixB(sPlace+k, (j*3)+2) = -cos(current_point.z.at(i).to_rad());

                       filled1=1;
                   }
                   // Если совпал индекс правой связи
                   if(i == j){
                       //A
                       matrixB(k, j*3) = (current_point.bMatrix.at(i)(0,0)*sin(current_point.a.at(i).to_rad()) -
                                              current_point.bMatrix.at(i)(1,0)*cos(current_point.a.at(i).to_rad()))/
                                              (-current_point.dp.at(i))*R;
                       matrixB(k, (j*3)+1) = (current_point.bMatrix.at(i)(0,1)*sin(current_point.a.at(i).to_rad()) -
                                                  current_point.bMatrix.at(i)(1,1)*cos(current_point.a.at(i).to_rad()))/
                                                  (-current_point.dp.at(i))*R;
                       matrixB(k, (j*3)+2) = (current_point.bMatrix.at(i)(0,2)*sin(current_point.a.at(i).to_rad()) -
                                                  current_point.bMatrix.at(i)(1,2)*cos(current_point.a.at(i).to_rad()))/
                                                  (-current_point.dp.at(i))*R;
                       //Z
                       matrixB(zPlace+k, j*3) = (current_point.bMatrix.at(i)(0,0)*cos(current_point.a.at(i).to_rad())*cos(current_point.z.at(i).to_rad()) +
                                                     current_point.bMatrix.at(i)(1,0)*sin(current_point.a.at(i).to_rad())*cos(current_point.z.at(i).to_rad()) -
                                                     current_point.bMatrix.at(i)(2,0)*sin(current_point.z.at(i).to_rad()))/
                                                    (current_point.sf.at(i))*R;
                       matrixB(zPlace+k, (j*3)+1) = (current_point.bMatrix.at(i)(0,1)*cos(current_point.a.at(i).to_rad())*cos(current_point.z.at(i).to_rad()) +
                                                         current_point.bMatrix.at(i)(1,1)*sin(current_point.a.at(i).to_rad())*cos(current_point.z.at(i).to_rad()) -
                                                         current_point.bMatrix.at(i)(2,1)*sin(current_point.z.at(i).to_rad()))/
                                                        (current_point.sf.at(i))*R;
                       matrixB(zPlace+k, (j*3)+2) = (current_point.bMatrix.at(i)(0,2)*cos(current_point.a.at(i).to_rad())*cos(current_point.z.at(i).to_rad()) +
                                                         current_point.bMatrix.at(i)(1,2)*sin(current_point.a.at(i).to_rad())*cos(current_point.z.at(i).to_rad()) -
                                                         current_point.bMatrix.at(i)(2,2)*sin(current_point.z.at(i).to_rad()))/
                                                        (current_point.sf.at(i))*R;
                       //S
                       matrixB(sPlace+k, j*3) = (current_point.bMatrix.at(i)(0,0)*cos(current_point.a.at(i).to_rad())*sin(current_point.z.at(i).to_rad()) +
                                                     current_point.bMatrix.at(i)(1,0)*sin(current_point.a.at(i).to_rad())*sin(current_point.z.at(i).to_rad()) +
                                                     current_point.bMatrix.at(i)(2,0)*cos(current_point.z.at(i).to_rad()));
                       matrixB(sPlace+k, (j*3)+1) = (current_point.bMatrix.at(i)(0,1)*cos(current_point.a.at(i).to_rad())*sin(current_point.z.at(i).to_rad()) +
                                                         current_point.bMatrix.at(i)(1,1)*sin(current_point.a.at(i).to_rad())*sin(current_point.z.at(i).to_rad()) +
                                                         current_point.bMatrix.at(i)(2,1)*cos(current_point.z.at(i).to_rad()));
                       matrixB(sPlace+k, (j*3)+2) = (current_point.bMatrix.at(i)(0,2)*cos(current_point.a.at(i).to_rad())*sin(current_point.z.at(i).to_rad()) +
                                                         current_point.bMatrix.at(i)(1,2)*sin(current_point.a.at(i).to_rad())*sin(current_point.z.at(i).to_rad()) +
                                                         current_point.bMatrix.at(i)(2,2)*cos(current_point.z.at(i).to_rad()));

                       filled2=1;
                   }
                   // Если две стороны просмотрены, заканчиваем работу
                   if(filled1==1 && filled2==1) break;
               }
               // Итерация
               k++;
               filled1=0;
               filled2=0;
               // Считаем разницу между загрубленными и вычисленными значениями
               errorsA.push_back(current_point.a.at(i).value - current_point_calc.a.at(i).value);
               errorsZ.push_back(current_point.z.at(i).value - current_point_calc.z.at(i).value);
               errorsS.push_back(current_point.sf.at(i) - current_point_calc.sf.at(i));
           }
       }
    }
    // Формируем один массив ошибок по порядку A, Z, s
    errors.insert(errors.end(), errorsA.begin(), errorsA.end());
    errors.insert(errors.end(), errorsZ.begin(), errorsZ.end());
    errors.insert(errors.end(), errorsS.begin(), errorsS.end());
    matrixNormal = matrixB.transpose()*matrixB;
    deltaX = matrixNormal.inverse();
    vectorW = VectorXd::Map(errors.data(), errors.size());
    deltax = VectorXd(-deltaX*matrixB.transpose()*vectorW);
    vectorV = VectorXd(matrixB*deltax + vectorW);
    VectorXd temp(vectorV.transpose()*vectorV);
    MU = sqrt(temp(0,0)/(pow(CONST_SIZE,2.0)*3 - k*3));
}




void read_dots(){
    std::vector<std::vector<double> > out;
    QFile myfile (dots_filename);
    myfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (myfile.isOpen())
    {
        QTextStream stream(&myfile);
        while ( !stream.atEnd() )
        {
            std::vector<double> v;
            foreach(QString str, stream.readLine().split(",")){
                v.push_back(str.toDouble());
            }
            out.push_back(v);
        }
        myfile.close();
    }
    for (int i = 0; i < (int) out.size(); ++i)
    {
        dots.push_back(Dot(out.at(i)));
    }
    CONST_SIZE = (int) out.size();
    myfile.close();
}

void read_link_map(){
    QFile myfile (link_map_filename);
    link_map = MatrixXd(CONST_SIZE,CONST_SIZE);
    myfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(myfile.isOpen()){
        QTextStream stream(&myfile);
        for(int i=0; i<CONST_SIZE; i++){
            QStringList list = stream.readLine().split(" ");
            for(int j=0; j<CONST_SIZE; j++){
                link_map(i,j) = list.at(j).toInt();
            }
        }
    }
    myfile.close();
}


void process(){
    start_points_calc.clear();
    start_points_zagr.clear();
    for (int i = 0; i < CONST_SIZE; ++i){
        start_points_calc.push_back(Start_Point(&dots.at(i), i));
        start_points_calc.at(i).calculate(dots);
        start_points_zagr.push_back(start_points_calc.at(i));
        start_points_zagr.at(i).zagr();

    }
    matrixB_calc();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

