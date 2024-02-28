#ifndef BUSQUEDARANGO_H
#define BUSQUEDARANGO_H

#include <QWidget>
#include "menubusqueda.h"
#include "busquedarangomonto.h"
#include "busquedarangofecha.h"

namespace Ui {
class busquedaRango;
}

class busquedaRango : public QWidget
{
    Blockchain<Block*>*bc;
    Q_OBJECT

public:
    explicit busquedaRango(QWidget *parent = nullptr);
    ~busquedaRango();

    void setBC(Blockchain<Block*>*bc);

private slots:
    void on_pushButton_clicked();

    void on_btn_monto_clicked();

    void on_btn_fecha_clicked();

private:
    Ui::busquedaRango *ui;
};

#endif // BUSQUEDARANGO_H
