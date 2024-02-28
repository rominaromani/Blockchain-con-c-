#ifndef BUSQUEDARECEPTOR_H
#define BUSQUEDARECEPTOR_H

#include <QWidget>
#include "busqueda.h"

namespace Ui {
class busquedaReceptor;
}

class busquedaReceptor : public QWidget
{
    Blockchain<Block*>* bc;
    Q_OBJECT

public:
    explicit busquedaReceptor(QWidget *parent = nullptr);
    ~busquedaReceptor();
    void setBC(Blockchain<Block*>*bc);

private slots:
    void on_btn_atras_clicked();

    void on_btn_buscar_clicked();

private:
    Ui::busquedaReceptor *ui;
};

#endif // BUSQUEDARECEPTOR_H
