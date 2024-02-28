#ifndef MAX_MIN_H
#define MAX_MIN_H

#include <QWidget>
#include "menubusqueda.h"

namespace Ui {
class Max_Min;
}

class Max_Min : public QWidget
{
    Blockchain<Block*>*bc;
    Q_OBJECT

public:
    explicit Max_Min(QWidget *parent = nullptr);
    ~Max_Min();
    void setBC(Blockchain<Block*>*bc);
private slots:
    void on_btn_atras_clicked();

    void on_btn_maximo_clicked();

    void on_btn_minimo_clicked();

private:
    Ui::Max_Min *ui;
};

#endif // MAX_MIN_H
