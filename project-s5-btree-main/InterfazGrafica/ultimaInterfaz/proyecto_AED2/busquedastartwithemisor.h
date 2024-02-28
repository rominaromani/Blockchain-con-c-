#ifndef BUSQUEDASTARTWITHEMISOR_H
#define BUSQUEDASTARTWITHEMISOR_H

#include <QWidget>
#include "busquedastartwith.h"

namespace Ui {
class busquedaStartWithEmisor;
}

class busquedaStartWithEmisor : public QWidget
{
    Blockchain<Block*>* bc;
    Q_OBJECT

public:
    explicit busquedaStartWithEmisor(QWidget *parent = nullptr);
    void setBC(Blockchain<Block*>*bc);

    ~busquedaStartWithEmisor();

private slots:
    void on_btn_atras_clicked();

    void on_btn_buscar_clicked();

private:
    Ui::busquedaStartWithEmisor *ui;
};

#endif // BUSQUEDASTARTWITHEMISOR_H
