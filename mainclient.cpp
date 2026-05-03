#include "mainclient.h"
#include "ui_mainclient.h"

MainClient::MainClient(QWidget *parent, std::unique_ptr<IConnection> &ptr_I)
    : QMainWindow(parent)
    , ui(new Ui::MainClient)
    , m_ptr_I{ptr_I}
{
    ui->setupUi(this);

    QObject::connect(ui->qpsh_connect, &QPushButton::clicked, this, &MainClient::slot_connect);
    QObject::connect(ui->qpsh_disconnect, &QPushButton::clicked, this, &MainClient::slot_disconnect);
    QObject::connect(ui->qpsh_send, &QPushButton::clicked, this, &MainClient::slot_send);

    std::cout << Q_FUNC_INFO << std::endl;
}

MainClient::~MainClient()
{
    delete ui;
}

void MainClient::slot_connect()
{
    QString host = ui->qlineedit_host->text();
    m_ptr_I->connect(host.toStdString(), CONNECTION::SERVER_PORT);
}

void MainClient::slot_disconnect()
{
    m_ptr_I->disconnect();
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

    m_ptr_I->send(TCP_DATA::DataTypes::TestStruct, raw_data);
}
