#ifndef BUSQUEDASTARTWITH_H
#define BUSQUEDASTARTWITH_H

#include <QWidget>
#include "menubusqueda.h"
#include "busquedastartwithemisor.h"
#include "busquedastartwithreceptor.h"

namespace Ui {
class busquedaStartWith;
}

class busquedaStartWith : public QWidget
{
    Blockchain<Block*>* bc;
    Q_OBJECT

public:
    explicit busquedaStartWith(QWidget *parent = nullptr);
    void setBC(Blockchain<Block*>*bc);
    ~busquedaStartWith();

private slots:
    void on_btn_atras_clicked();

    void on_btn_emisor_clicked();

    void on_btn_receptor_clicked();

private:
    Ui::busquedaStartWith *ui;
};

#endif // BUSQUEDASTARTWITH_H
