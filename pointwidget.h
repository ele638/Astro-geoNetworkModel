#ifndef POINTWIDGET_H
#define POINTWIDGET_H

#include <QWidget>

namespace Ui {
class PointWidget;
}

class PointWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PointWidget(QWidget *parent = 0);
    ~PointWidget();

private:
    Ui::PointWidget *ui;
};

#endif // POINTWIDGET_H
