#include "./ui_mainwindow.h"
#include "busqueda.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bc = new Blockchain<Block*>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setBC(Blockchain<Block*>* bc){
    this->bc = bc;
}


void MainWindow::on_btn_buscar_clicked()
{
    //Cambio de página
    menuBusqueda* siguiente = new menuBusqueda();
    siguiente->show();
    //Le paso el blockchain a la siguiente página
    siguiente->setBC(bc);
    //Cierro la página actual
    close();
}

