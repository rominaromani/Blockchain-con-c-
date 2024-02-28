#ifndef BUSQUEDASTARTWITHRECEPTOR_H
#define BUSQUEDASTARTWITHRECEPTOR_H

#include <QWidget>
#include "busquedastartwith.h"

namespace Ui {
class busquedaStartWithReceptor;
}

class busquedaStartWithReceptor : public QWidget
{
    Blockchain<Block*>* bc;
    Q_OBJECT

public:
    explicit busquedaStartWithReceptor(QWidget *parent = nullptr);
    ~busquedaStartWithReceptor();
    void setBC(Blockchain<Block*>*bc);


private slots:
    void on_btn_atras_clicked();

    void on_btn_buscar_clicked();

private:
    Ui::busquedaStartWithReceptor *ui;
};

#endif // BUSQUEDASTARTWITHRECEPTOR_H
