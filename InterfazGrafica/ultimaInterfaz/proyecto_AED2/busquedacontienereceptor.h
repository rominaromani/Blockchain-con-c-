#ifndef BUSQUEDACONTIENERECEPTOR_H
#define BUSQUEDACONTIENERECEPTOR_H

#include <QWidget>
#include "busquedacontiene.h"

namespace Ui {
class busquedaContieneReceptor;
}

class busquedaContieneReceptor : public QWidget
{
    Blockchain<Block*>* bc;
    Q_OBJECT

public:
    explicit busquedaContieneReceptor(QWidget *parent = nullptr);
    void setBC(Blockchain<Block*>*bc);

    ~busquedaContieneReceptor();

private slots:
    void on_btn_atras_clicked();

    void on_btn_buscar_clicked();

private:
    Ui::busquedaContieneReceptor *ui;
};

#endif // BUSQUEDACONTIENERECEPTOR_H
