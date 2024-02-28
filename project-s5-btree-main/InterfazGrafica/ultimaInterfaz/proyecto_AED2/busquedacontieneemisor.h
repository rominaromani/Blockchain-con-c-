#ifndef BUSQUEDACONTIENEEMISOR_H
#define BUSQUEDACONTIENEEMISOR_H

#include <QWidget>
#include "busquedacontiene.h"

namespace Ui {
class busquedaContieneEmisor;
}

class busquedaContieneEmisor : public QWidget
{
    Blockchain<Block*>* bc;
    Q_OBJECT

public:
    explicit busquedaContieneEmisor(QWidget *parent = nullptr);
    void setBC(Blockchain<Block*>*bc);
    ~busquedaContieneEmisor();

private slots:
    void on_btn_atras_clicked();

    void on_btn_buscar_clicked();

private:
    Ui::busquedaContieneEmisor *ui;
};

#endif // BUSQUEDACONTIENEEMISOR_H
