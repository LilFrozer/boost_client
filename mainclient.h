#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#include <QMainWindow>
#include <memory>
#include <boost/asio.hpp>
#include <iostream>
#include "connection.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainClient;
}
QT_END_NAMESPACE

class MainClient : public QMainWindow
{
    Q_OBJECT

public:
    MainClient(QWidget *parent, std::unique_ptr<IConnection> &ptr_I);
    ~MainClient();

private:
    Ui::MainClient *ui;
    std::unique_ptr<IConnection> &m_ptr_I;

private slots:
    void slot_connect();
    void slot_disconnect();
    void slot_send();
};
#endif // MAINCLIENT_H
