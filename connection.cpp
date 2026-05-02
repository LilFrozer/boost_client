#include "connection.h"

BoostConnection::BoostConnection(boost::asio::io_context &io_context) :
    m_socket{io_context},
    m_resolver{io_context}
{

}

void BoostConnection::connect(const std::string &host, const uint16_t port)
{
    auto ptr_self = shared_from_this();

    m_resolver.async_resolve(host, std::to_string(port), [this, ptr_self](boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type endpoints)
    {
        if( ec )
        {
            return;
        }
        boost::asio::async_connect(m_socket, endpoints, [this, ptr_self](boost::system::error_code ec, boost::asio::ip::tcp::endpoint)
        {
            if( ec )
            {

            }
            else
            {
                std::cout << "Connected to server!" << std::endl;
            }
         });
    });
}

void BoostConnection::disconnect()
{

}

/**
 * @brief Отправка данных по tcp соединению
 */
void BoostConnection::send(Connection::DataTypes data_type, std::vector<uint8_t> &raw_data)
{
    Connection::Packet pkt{};

    pkt.data_type = data_type;
    pkt.data = raw_data;

    pkt.header.total_data_size = raw_data.size();
    pkt.header.total_cnt_packets = 1;
    pkt.header.cur_packet_number = 1;
    pkt.header.cur_packet_size = raw_data.size() + sizeof(Connection::PacketHeader) + sizeof(uint8_t);

    pkt.header.isFirst = 1;
    pkt.header.isLast = 1;

    // pack full packet into binary type
    std::vector<uint8_t> buffer;
}
