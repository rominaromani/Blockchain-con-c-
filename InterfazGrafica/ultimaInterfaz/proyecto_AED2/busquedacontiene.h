#ifndef BUSQUEDACONTIENE_H
#define BUSQUEDACONTIENE_H

#include <QWidget>
#include "menubusqueda.h"
#include "busquedacontieneemisor.h"
#include "busquedacontienereceptor.h"

namespace Ui {
class busquedaContiene;
}

class busquedaContiene : public QWidget
{
    Blockchain<Block*>* bc;
    Q_OBJECT

public:
    explicit busquedaContiene(QWidget *parent = nullptr);
    void setBC(Blockchain<Block*>*bc);
    ~busquedaContiene();

private slots:
    void on_btn_atras_clicked();

    void on_btn_emisor_clicked();

    void on_btn_receptor_clicked();

private:
    Ui::busquedaContiene *ui;
};

#endif // BUSQUEDACONTIENE_H
