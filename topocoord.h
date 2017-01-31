#ifndef TOPOCOORD_H
#define TOPOCOORD_H

#include <QWidget>

namespace Ui {
class TopoCoord;
}

class TopoCoord : public QWidget
{
    Q_OBJECT

public:
    explicit TopoCoord(QWidget *parent = 0);
    ~TopoCoord();

private:
    Ui::TopoCoord *ui;
};

#endif // TOPOCOORD_H
