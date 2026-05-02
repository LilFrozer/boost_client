#include "mainclient.h"
#include "ui_mainclient.h"

MainClient::MainClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainClient)
{
    ui->setupUi(this);
    std::cout << Q_FUNC_INFO << std::endl;
}

MainClient::~MainClient()
{
    delete ui;
}
