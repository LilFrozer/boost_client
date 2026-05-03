#include "mainclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::unique_ptr<IConnection> ptr_I;

    try {
        boost::asio::io_context io_context;
        ptr_I = std::make_unique<BoostConnection>(io_context);
        io_context.run();
    } catch ( const std::exception &e ) {
        std::cerr << e.what() << std::endl;
    }

    std::unique_ptr<MainClient> client = std::make_unique<MainClient>(nullptr, ptr_I);
    client->show();

    return a.exec();
}
