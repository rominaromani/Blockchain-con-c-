#ifndef MENUBUSQUEDA_H
#define MENUBUSQUEDA_H

#include <QWidget>
#include "mainwindow.h"
#include "busqueda.h"
#include "busquedarango.h"
#include "max_min.h"
#include "busquedacontiene.h"
#include "busquedastartwith.h"

namespace Ui {
class menuBusqueda;
}

class menuBusqueda : public QWidget
{
    Blockchain<Block*>*bc;
    Q_OBJECT

public:
    explicit menuBusqueda(QWidget *parent = nullptr);
    ~menuBusqueda();
    void setBC(Blockchain<Block*>*bc);
private slots:
    void on_btn_atras_clicked();

    void on_btn_igualdad_clicked();

    void on_btn_max_min_clicked();

    void on_btn_rango_clicked();

    void on_btn_contiene_clicked();

    void on_btn_empiezaCon_clicked();

private:
    Ui::menuBusqueda *ui;
};

#endif // MENUBUSQUEDA_H
