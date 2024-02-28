#ifndef BUSQUEDAFECHA_H
#define BUSQUEDAFECHA_H

#include <QWidget>
#include "busqueda.h"

namespace Ui {
class busquedaFecha;
}

class busquedaFecha : public QWidget
{
    Blockchain<Block*>*bc;
    Q_OBJECT

public:
    explicit busquedaFecha(QWidget *parent = nullptr);
    ~busquedaFecha();
    void setBC(Blockchain<Block*>*bc);
private slots:
    void on_btn_atras_clicked();

    void on_btn_buscar_clicked();

private:
    Ui::busquedaFecha *ui;
};

#endif // BUSQUEDAFECHA_H
