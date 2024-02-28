#ifndef BUSQUEDARANGOMONTO_H
#define BUSQUEDARANGOMONTO_H

#include <QWidget>
#include "busquedarango.h"

namespace Ui {
class busquedaRangoMonto;
}

class busquedaRangoMonto : public QWidget
{
    Blockchain<Block*>*bc;
    Q_OBJECT

public:
    explicit busquedaRangoMonto(QWidget *parent = nullptr);
    void setBC(Blockchain<Block*>*bc);
    ~busquedaRangoMonto();

private slots:
    void on_btn_atras_clicked();

    void on_btn_buscar_clicked();

private:
    Ui::busquedaRangoMonto *ui;
};

#endif // BUSQUEDARANGOMONTO_H
