#ifndef MAIN_H
#define MAIN_H

#include "QString"
#include "Eigen/Dense"
using namespace Eigen;

extern int CONST_SIZE;
extern double CONST_A, CONST_ALPHA, R, MU, m_A, m_Z, m_S;
extern QString dots_filename, link_map_filename;
extern MatrixXd link_map;
class Angle{
public:
    int g, m;
    double s, value;
    Angle(int grad, int min, double sec);
    Angle(double val);
};
class Dot{
public:
    class Ellips_coord{
    public:
        double b, l, h;
        Ellips_coord();
        Ellips_coord(std::vector<double> v);
    };
    class Geocentr_coord{
    public:
        double x, y, z;
        Geocentr_coord();
        Geocentr_coord(double x1, double y1, double z1);
        Geocentr_coord(Ellips_coord in );
        Geocentr_coord operator-(Geocentr_coord g);
    };
    Ellips_coord Q;
    Geocentr_coord G;
    std::vector<double> values;
    Dot(std::vector<double> v);
};
class Start_Point{
public:
    int number;
    std::vector<Angle> a, z;
    std::vector<double> xp, yp, zp, dp, sf;
    Dot* dot;
    Matrix3d aMatrix;
    std::vector<Matrix3d> bMatrix;

    Start_Point(Dot* indot, int index);
    void calculate(std::vector<Dot> dots);
    void zagr();
};
extern std::vector<Start_Point> start_points_calc, start_points_zagr;
extern std::vector<Dot> dots;

extern MatrixXd matrixB, matrixNormal, deltaX;
extern VectorXd deltax, vectorV, vectorW, vectorL;

void write_matrix(MatrixXd& matrix, QString path);
void write_vector(VectorXd& vector, QString path);
void matrixB_calc();
void read_dots();
void read_link_map();
void process();

#endif // MAIN_H
