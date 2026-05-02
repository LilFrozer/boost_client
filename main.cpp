#include "mainclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::unique_ptr<MainClient> client = std::make_unique<MainClient>();
    client->show();

    try {
        // boost::asio::io_context io_context;

        // io_context.run();
    } catch ( const std::exception &e ) {
        std::cerr << e.what() << std::endl;
    }
    return a.exec();
}
