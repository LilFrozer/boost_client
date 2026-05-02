#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <string>
#include <cstdint>

namespace Connection
{
struct PacketHeader
{
    uint16_t server_hash{0x1337};

    uint32_t total_data_size{0x0};
    uint16_t total_cnt_packets{0x0};
    uint16_t cur_packet_number{0x0};
    uint16_t cur_packet_size{0x0};

    uint8_t isFirst : 1;
    uint8_t isLast : 1;
    uint8_t : 6;
};

enum class DataTypes : uint8_t
{
    TestStruct = 0x0
};

struct Packet
{
    PacketHeader    header;
    DataTypes       data_type{DataTypes::TestStruct};
    std::vector<uint8_t> data;
};
}

class Interface
{
public:
    virtual void connect(const std::string &host, const uint16_t port) = 0;
    virtual void disconnect() = 0;
    virtual void send(Connection::DataTypes data_type, std::vector<uint8_t> &raw_data) = 0;
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

class BoostConnection : public Interface, std::enable_shared_from_this<BoostConnection>
{
private:
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::resolver m_resolver;
public:
    BoostConnection(boost::asio::io_context &io_context);
    ~BoostConnection() = default;
    void connect(const std::string &host, const uint16_t port) override final;
    void disconnect() override final;
    void send(Connection::DataTypes data_type, std::vector<uint8_t> &raw_data) override final;
};

#endif // CONNECTION_H
