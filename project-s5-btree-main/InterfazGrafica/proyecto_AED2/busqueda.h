#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include <QWidget>
#include "menubusqueda.h"
#include "busquedareceptor.h"
#include "busquedamonto.h"
#include "busquedafecha.h"

namespace Ui {
class Busqueda;
}

class Busqueda : public QWidget
{
    Blockchain<Block*>*bc;
    Q_OBJECT

public:
    explicit Busqueda(QWidget *parent = nullptr);
    ~Busqueda();
    void setBC(Blockchain<Block*>*bc);
private slots:
    void on_btn_atras_clicked();

    void on_btn_emisor_clicked();

    void on_btn_receptor_clicked();

    void on_btn_monto_clicked();

    void on_btn_fecha_clicked();

private:
    Ui::Busqueda *ui;
};

#endif // BUSQUEDA_H
