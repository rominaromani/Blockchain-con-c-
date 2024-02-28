#ifndef BUSQUEDAEMISOR_H
#define BUSQUEDAEMISOR_H

#include <QWidget>
#include "busqueda.h"

namespace Ui {
class busquedaEmisor;
}

class busquedaEmisor : public QWidget
{
    Blockchain<Block*>* bc;
    Q_OBJECT

public:
    explicit busquedaEmisor(QWidget *parent = nullptr);
    ~busquedaEmisor();
    void setBC(Blockchain<Block*>*bc);
private slots:
    void on_btn_atras_clicked();

    void on_btn_buscar_clicked();

private:
    Ui::busquedaEmisor *ui;
};

#endif // BUSQUEDAEMISOR_H
