#include "connection.h"

BoostConnection::BoostConnection(boost::asio::io_context &io_context) :
    m_socket{io_context},
    m_resolver{io_context}
{

}

BoostConnection::~BoostConnection()
{
    this->disconnect();
}

void BoostConnection::connect(const std::string &host, const uint16_t port)
{
    auto ptr_self = shared_from_this();

    m_resolver.async_resolve(host, std::to_string(port), [this, ptr_self](boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type endpoints)
    {
        if( ec )
        {
            this->handle_error(ec, "resolve");
            return;
        }
        boost::asio::async_connect(m_socket, endpoints, [this, ptr_self](boost::system::error_code ec, boost::asio::ip::tcp::endpoint)
        {
            if( ec )
            {
                this->handle_error(ec, "connect");
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
    if( m_socket.is_open() )
    {
        boost::system::error_code ec;
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        m_socket.close(ec);
    }

    std::cout << "Connection`s disconnect" << std::endl;
}

std::vector<uint8_t> BoostConnection::packet_to_binary(CONNECTION::Packet &packet)
{
    std::vector<uint8_t> buffer;

    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&packet.header.server_hash),
                  reinterpret_cast<const uint8_t*>(&packet.header.server_hash) + 2);

    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&packet.header.total_data_size),
                  reinterpret_cast<const uint8_t*>(&packet.header.total_data_size) + 4);

    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&packet.header.total_cnt_packets),
                  reinterpret_cast<const uint8_t*>(&packet.header.total_cnt_packets) + 2);

    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&packet.header.cur_packet_number),
                  reinterpret_cast<const uint8_t*>(&packet.header.cur_packet_number) + 2);

    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&packet.header.cur_packet_size),
                  reinterpret_cast<const uint8_t*>(&packet.header.cur_packet_size) + 2);

    uint8_t flags = (packet.header.isFirst ? 0x01 : 0x00) |
                    (packet.header.isLast ? 0x02 : 0x00);
    buffer.push_back(flags);
    buffer.push_back(0);

    // !!!cast<>(DataTypes) -> int16_t
    int16_t data_type = static_cast<uint16_t>(packet.data_type);
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&data_type),
                  reinterpret_cast<const uint8_t*>(&data_type) + 2);

    buffer.insert(buffer.end(), packet.data.begin(), packet.data.end());

    return buffer;
}

/**
 * @brief Отправка данных по tcp соединению
 */
void BoostConnection::send(TCP_DATA::DataTypes data_type, std::vector<uint8_t> &raw_data)
{
    if( !m_socket.is_open() )
    {
        std::cout << "Connect to server for send!" << std::endl;
        return;
    }

    CONNECTION::Packet pkt{};

    pkt.data_type = data_type;
    pkt.data = raw_data;

    pkt.header.total_data_size = raw_data.size();
    pkt.header.total_cnt_packets = 1;
    pkt.header.cur_packet_number = 1;
    pkt.header.cur_packet_size = raw_data.size() + sizeof(CONNECTION::PacketHeader) + sizeof(uint8_t);

    pkt.header.isFirst = 1;
    pkt.header.isLast = 1;

    std::vector<uint8_t> buffer = packet_to_binary(pkt);

    uint32_t packet_size = htonl(buffer.size());
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(&packet_size, 4));
    buffers.push_back(boost::asio::buffer(buffer));

    auto self = shared_from_this();

    boost::asio::async_write(m_socket, buffers, [this, self](boost::system::error_code ec, size_t length)
    {
        if( ec )
        {
            handle_error(ec, "send");
        }
        else
        {
            std::cout << "Отправлено " << length << " байт" << std::endl;
        }
    });

    // std::cout << "Отправлен пакет #" << pkt.header.cur_packet_number
    //           << "/" << pkt.header.total_cnt_packets
    //           << ", размер: " << buffer.size() << " байт"
    //           << ", isFirst=" << (int)pkt.header.isFirst
    //           << ", isLast=" << (int)pkt.header.isLast << std::endl;
}

void BoostConnection::handle_error(const boost::system::error_code& ec, const std::string& context)
{
    if( ec == boost::asio::error::eof )
    {
        std::cout << "Connection closed by server" << std::endl;
        this->disconnect();
    }
    else if( ec == boost::asio::error::connection_reset)
    {
        std::cout << "Connection reset by server" << std::endl;
        this->disconnect();
    }
    else if( ec == boost::asio::error::connection_aborted)
    {
        std::cout << "Connection aborted" << std::endl;
        this->disconnect();
    }
    else if( ec == boost::asio::error::broken_pipe)
    {
        std::cout << "Connection (broken pipe)" << std::endl;
        this->disconnect();
    }
    else
    {
        std::cout << "Error in: " << context << " ;ec = " << ec.message() << std::endl;
    }
}
