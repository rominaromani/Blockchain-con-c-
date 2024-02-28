#ifndef BUSQUEDAMONTO_H
#define BUSQUEDAMONTO_H

#include <QWidget>
#include "busqueda.h"

namespace Ui {
class busquedaMonto;
}

class busquedaMonto : public QWidget
{
    Blockchain<Block*>*bc;
    Q_OBJECT

public:
    explicit busquedaMonto(QWidget *parent = nullptr);
    ~busquedaMonto();
    void setBC(Blockchain<Block*>*bc);
private slots:
    void on_bton_atras_clicked();

    void on_btn_buscar_clicked();

private:
    Ui::busquedaMonto *ui;
};

#endif // BUSQUEDAMONTO_H
