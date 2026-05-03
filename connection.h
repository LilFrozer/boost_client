#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include "GroupData.h"

namespace CONNECTION
{
const uint16_t SERVER_HASH = 0x1337;
const uint16_t SERVER_PORT = 1122;

struct PacketHeader
{
    uint16_t server_hash{SERVER_HASH};

    uint32_t total_data_size{0x0};
    uint16_t total_cnt_packets{0x0};
    uint16_t cur_packet_number{0x0};
    uint16_t cur_packet_size{0x0};

    uint8_t isFirst : 1;
    uint8_t isLast : 1;
    uint8_t : 6;
};

struct Packet
{
    PacketHeader header{};
    TCP_DATA::DataTypes data_type{TCP_DATA::DataTypes::TestStruct};
    std::vector<uint8_t> data{};
};
}

class IConnection
{
public:
    virtual void connect(const std::string &host, const uint16_t port) = 0;
    virtual void disconnect() = 0;
    virtual void send(TCP_DATA::DataTypes data_type, std::vector<uint8_t> &raw_data) = 0;
    virtual ~IConnection() = default;
};

// class SysSockConnection : public Interface
// {
// public:
//     SysSockConnection();
//     ~SysSockConnection() = default;
//     void connect() override final;
//     void disconnect() override final;
// };

#include <boost/asio.hpp>

class BoostConnection : public IConnection, std::enable_shared_from_this<BoostConnection>
{
private:
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::resolver m_resolver;
private:
    static std::vector<uint8_t> packet_to_binary(CONNECTION::Packet &packet);
    void handle_error(const boost::system::error_code& ec, const std::string& context);
public:
    BoostConnection(boost::asio::io_context &io_context);
    ~BoostConnection();
    void connect(const std::string &host, const uint16_t port) override final;
    void disconnect() override final;
    void send(TCP_DATA::DataTypes data_type, std::vector<uint8_t> &raw_data) override final;
};

#endif // CONNECTION_H
