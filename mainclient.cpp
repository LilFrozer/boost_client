#include "mainclient.h"
#include "ui_mainclient.h"

MainClient::MainClient(QWidget *parent, boost::asio::io_context &io)
    : QMainWindow(parent)
    , ui(new Ui::MainClient)
{
    ui->setupUi(this);

    QObject::connect(ui->qpsh_connect, &QPushButton::clicked, this, &MainClient::slot_connect);
    QObject::connect(ui->qpsh_disconnect, &QPushButton::clicked, this, &MainClient::slot_disconnect);
    QObject::connect(ui->qpsh_send, &QPushButton::clicked, this, &MainClient::slot_send);

    if( !Iserver_ ) {
        Iserver_ = std::make_shared<BoostConnection>(io);
    }
}

MainClient::~MainClient()
{
    delete ui;
}

void MainClient::slot_connect()
{
    QString host = ui->qlineedit_host->text();
    std::cout << "trying connect. addr:" << host.toStdString() << std::endl;
    Iserver_->connect(host.toStdString(), CONNECTION::SERVER_PORT);
}

void MainClient::slot_disconnect()
{
    Iserver_->disconnect();
}

void MainClient::slot_send()
{
    TCP_DATA::TestStruct _struct;
    _struct.a = "test";
    _struct.b = 1;
    _struct.c = 0.75;
    _struct.d.reserve(5);
    for( size_t i=0;i<5;++i )
        _struct.d.push_back(i+i*i);

    std::vector<uint8_t> raw_data = _struct.serilize();

    Iserver_->send(TCP_DATA::DataTypes::TestStruct, raw_data);
}
