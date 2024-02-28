#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "blockchain.h"
#include "menubusqueda.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Blockchain<Block*>*bc;
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setBC(Blockchain<Block*>*bc);
private slots:
    void on_btn_buscar_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
