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
#include <gigamonkey/p2p/messages/ping_pong_message.hpp>
#include <gigamonkey/p2p/messages/protoconf_message.hpp>
#include <gigamonkey/p2p/node.hpp>
#include <gigamonkey/p2p/messages/address_message.hpp>


namespace Gigamonkey::p2p {


    NodeConnection::NodeConnection(boost::asio::io_context &ioContext, boost::asio::ip::tcp::endpoint mEp, Node* nod)
            : io_context(ioContext), m_ep(std::move(mEp)), m_sock(io_context), node(nod), incoming_message(){
        recv_version = 209;
        send_version = 70015;
    }

    void NodeConnection::connect(boost::function<void(boost::system::error_code)> connected) {
        boost::system::error_code ec;
        m_sock.open(boost::asio::ip::tcp::v4());
        if (ec.value() != 0) {
            DATA_LOG_CHANNEL("network",error) << "Failed to open the socket! Error code = "
                                                 << ec.value() << ". Message: " << ec.message();
            active=false;
            connecting=true;
            connected(ec);
            return;
        }
        DATA_LOG_CHANNEL("network",normal) << "Connecting to " << m_ep;
        m_sock.async_connect(m_ep, [this, connected](boost::system::error_code ec) { connect_handler(ec, connected); });
    }

    void NodeConnection::connect_handler(const boost::system::error_code &ec,
                                         boost::function<void(boost::system::error_code)> connected) {
        if (ec.failed()) {
            DATA_LOG_CHANNEL("network",error)
                    << "Failed to connect to " << m_ep <<"! Error code = "
                    << ec.value() << ". Message: " << ec.message();
            active=false;
            connecting=false;
            connected(ec);
            return;
        }
        DATA_LOG_CHANNEL("network",normal) << "Connected to " << m_ep << " successfully";
        connected(ec);
        messages::VersionMessagePtr outgoingVersion=boost::make_shared<messages::VersionMessage>((node->getParams()));
        outgoingVersion->setProtocolVersion(send_version);
        std::time_t result = std::time(nullptr);
        outgoingVersion->setTimestamp(result);
        outgoingVersion->setUserAgent("Cosmos Wallet");
        messages::MessageHeadPtr header=boost::make_shared<messages::MessageHeader>((node->getParams()));
        header->setCommandName("version");
        messages::Message message(header, outgoingVersion);
        message.buildMessage();
        auto bytes = (data::bytes) message;
        boost::asio::async_write(m_sock, boost::asio::buffer(bytes.data(), bytes.size()),
                                 [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                                     network_loop();
                                 });

    }


    Node* NodeConnection::getNode() {
        return node;
    }



    void NodeConnection::header_handler(const boost::system::error_code &ec) {
        if (ec.failed()) {
            DATA_LOG_CHANNEL("network",error)
                    << "Failed to recieve version header! Error code = "
                    << ec.value() << ". Message: " << ec.message();
            active=false;
            connecting=false;
            return;
        }


        incoming_message = messages::Message(boost::make_shared<messages::MessageHeader>(incoming_header, (node->getParams())), boost::make_shared<messages::EmptyBody>(node->getParams()));
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
            messages::MessageBodyPtr message=incoming_message.makeBody(incoming_body,node->getParams());
            if(incoming_message.getBody()) {

                DATA_LOG_CHANNEL("network",normal) << "Recieved: " << *(incoming_message.getBody());
            }
            incoming_queue.emplace(incoming_message);
            //todo: temp
            network_loop();

        }
        else {
            DATA_LOG_CHANNEL("network",error)
                    << "Failed to recieve version body! Error code = "
                    << ec.value() << ". Message: " << ec.message();
            active=false;
            connecting=false;
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
        DATA_LOG_CHANNEL("network",normal) << "Sending: " << curMessage << "Bytes: " << bytes ;
        boost::asio::async_write(m_sock, boost::asio::buffer(bytes.data(), bytes.size()),
                                 [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                                     if(ec.failed())
                                     {
                                         DATA_LOG_CHANNEL("network",error)
                                             << "Failed to send message! Error code = "
                                             << ec.value() << ". Message: " << ec.message();
                                         active= false;
                                     }
                                 });

    }

    void NodeConnection::handler_loop() {

        while(!incoming_queue.empty()) {
            messages::Message msg=incoming_queue.front();
            incoming_queue.pop();
            if(msg.getHeader()->getCommandName()== "version") {
                messages::Message verack;
                boost::shared_ptr<messages::EmptyBody> verackBody=boost::make_shared<messages::EmptyBody>(node->getParams(),"verack");
                verack.setHeader(boost::make_shared<messages::MessageHeader>(node->getParams()));
                verack.setBody(verackBody);
                verack.buildMessage();
                outgoing_queue.push(verack);
                active= true;
                connecting=false;
            } else if(msg.getHeader()->getCommandName() == "ping") {
                messages::PingPongMessagePtr pingBody=boost::dynamic_pointer_cast<messages::PingPongMessage>(msg.getBody());
                messages::Message pong;
                messages::PingPongMessagePtr pongBody=boost::make_shared<messages::PingPongMessage>(node->getParams(),false);
                pongBody->setNonce(pingBody->getNonce());
                pong.setHeader(boost::make_shared<messages::MessageHeader>(node->getParams()));
                pong.setBody(pongBody);

                pong.buildMessage();

                outgoing_queue.push(pong);
            } else if(msg.getHeader()->getCommandName() == "protoconf") {
                messages::ProtoconfMessagePtr  protoconfBody=boost::dynamic_pointer_cast<messages::ProtoconfMessage>(msg.getBody());
                DATA_LOG_CHANNEL("network",normal) << "Recived protoconf: " << *protoconfBody;
            }
            else {
                undealt_queue.push(msg);
            }
        }
        sendMessage();

    }

    void NodeConnection::sendMessage(const messages::Message& messageToSend) {
        outgoing_queue.push(messageToSend);
    }
}