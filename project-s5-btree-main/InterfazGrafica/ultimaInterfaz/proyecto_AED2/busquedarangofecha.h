#ifndef BUSQUEDARANGOFECHA_H
#define BUSQUEDARANGOFECHA_H

#include <QWidget>
#include "busquedarango.h"

namespace Ui {
class busquedaRangoFecha;
}

class busquedaRangoFecha : public QWidget
{
    Blockchain<Block*>*bc;
    Q_OBJECT

public:
    explicit busquedaRangoFecha(QWidget *parent = nullptr);
    void setBC(Blockchain<Block*>*bc);
    ~busquedaRangoFecha();

private slots:
    void on_btn_atras_clicked();

    void on_btn_buscar_clicked();

private:
    Ui::busquedaRangoFecha *ui;
};

#endif // BUSQUEDARANGOFECHA_H
