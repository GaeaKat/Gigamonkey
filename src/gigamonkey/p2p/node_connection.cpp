// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/node_connection.hpp>
#include <gigamonkey/p2p/messages/version_message.hpp>
#include <utility>
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#include <data/log/log.hpp>
#include <gigamonkey/p2p/messages/message_header.hpp>
#include <gigamonkey/p2p/messages/message.hpp>
#include <gigamonkey/p2p/messages/reject_message.hpp>


namespace Gigamonkey::p2p {


    NodeConnection::NodeConnection(boost::asio::io_context &ioContext, boost::asio::ip::tcp::endpoint mEp, Node &nod)
            : io_context(ioContext), m_ep(std::move(mEp)), m_sock(io_context), node(nod), incoming_message(){
        recv_version = 209;
        send_version = 70015;
    }

    void NodeConnection::connect(boost::function<void(boost::system::error_code)> connected) {
        boost::system::error_code ec;
        m_sock.open(boost::asio::ip::tcp::v4());
        if (ec.value() != 0) {
            // Failed to open the socket.
            std::cout
                    << "Failed to open the socket! Error code = "
                    << ec.value() << ". Message: " << ec.message();
        }
        m_sock.async_connect(m_ep, [this, connected](boost::system::error_code ec) { connect_handler(ec, connected); });
    }

    void NodeConnection::connect_handler(const boost::system::error_code &ec,
                                         boost::function<void(boost::system::error_code)> connected) {
        std::cout << "Connected." << std::endl;
        if (ec.failed()) {
            std::cout
                    << "Failed to connect! Error code = "
                    << ec.value() << ". Message: " << ec.message();
            connected(ec);
            return;
        }
        connected(ec);
        messages::VersionMessagePtr outgoingVersion=boost::make_shared<messages::VersionMessage>((node.getParams()));
        outgoingVersion->setProtocolVersion(send_version);
        std::time_t result = std::time(nullptr);
        outgoingVersion->setTimestamp(result);
        outgoingVersion->setUserAgent("Cosmos Wallet");
        messages::MessageHeadPtr header=boost::make_shared<messages::MessageHeader>((node.getParams()));
        header->setCommandName("version");
        messages::Message message(header, outgoingVersion);
        message.buildMessage();
        auto bytes = (data::bytes) message;
        boost::asio::async_write(m_sock, boost::asio::buffer(bytes.data(), bytes.size()),
                                 [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                                     network_loop();
                                 });

    }


    Node &NodeConnection::getNode() {
        return node;
    }



    void NodeConnection::header_handler(const boost::system::error_code &ec) {
        if (ec.failed()) {
            std::cout
                    << "Failed to recieve version header! Error code = "
                    << ec.value() << ". Message: " << ec.message();
            return;
        }


        incoming_message = messages::Message(boost::make_shared<messages::MessageHeader>(incoming_header, (node.getParams())), boost::make_shared<messages::EmptyBody>(node.getParams()));
        incoming_body = data::bytes(incoming_message.getHeader()->getPayloadSize());
        boost::asio::async_read(m_sock, boost::asio::mutable_buffer(incoming_body.data(),
                                                                    incoming_body.size()),
                                [this](boost::system::error_code ec,
                                       std::size_t bytes_transferred) {
                                    body_handler(ec);
                                });
    }

    void NodeConnection::body_handler(const boost::system::error_code &ec) {
        if (!ec.failed()) {
            messages::MessageBodyPtr message=makeBody();
            incoming_message.setBody(message);
            if(incoming_message.getBody()) {
                std::cout << "Recieved: " << *(incoming_message.getBody()) << std::endl;
            }
            incoming_queue.emplace(incoming_message);
            sendMessage();
            network_loop();

        }
        else {
            std::cout
                    << "Failed to recieve version body! Error code = "
                    << ec.value() << ". Message: " << ec.message();
        }
    }

    messages::MessageBodyPtr  NodeConnection::makeBody() {
        std::string messageType=incoming_message.getHeader()->getCommandName();
        if(messageType=="reject")
        {
            return  boost::make_shared<messages::RejectMessage>(incoming_body,node.getParams());
        }
        else if(messageType == "version")
        {
            return  boost::make_shared<messages::VersionMessage>(incoming_body,node.getParams());
        }
        else {
            std::cerr << "Recieved unknown message: " << messageType << std::endl;
            return boost::make_shared<messages::EmptyBody>(node.getParams());
        }
    }

    void NodeConnection::network_loop() {
        incoming_header=data::bytes(24);
        boost::asio::async_read(m_sock, boost::asio::mutable_buffer(incoming_header.data(), 24),
                                [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                                    header_handler(ec);
                                });
    }

    void NodeConnection::sendMessage() {
        if(outgoing_queue.empty())
            return;
        messages::Message curMessage=outgoing_queue.front();
        outgoing_queue.pop();
        curMessage.buildMessage();
        auto bytes = (data::bytes) curMessage;
        boost::asio::async_write(m_sock, boost::asio::buffer(bytes.data(), bytes.size()),
                                 [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                                     if(ec.failed())
                                     {
                                         active= false;
                                     }
                                 });

    }
}