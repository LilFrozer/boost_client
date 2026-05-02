#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#include <QMainWindow>
#include <memory>
#include <boost/asio.hpp>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainClient;
}
QT_END_NAMESPACE

class MainClient : public QMainWindow
{
    Q_OBJECT

public:
    MainClient(QWidget *parent = nullptr);
    ~MainClient();

private:
    Ui::MainClient *ui;
};
#endif // MAINCLIENT_H
