// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_NODE_CONNECTION_H
#define GIGAMONKEY_NODE_CONNECTION_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>
#include <gigamonkey/p2p/node.hpp>
#include <gigamonkey/p2p/messages/message.hpp>
#include <data/tools.hpp>
#include <queue>

namespace Gigamonkey::p2p {
    /**
     * Connection to an external node
     */
    class NodeConnection {
    public:
        /**
         * Constructs a node connection
         *
         * @note this does not connect to the node yet
         * @param ioContext iocontext for the node
         * @param mEp Endpoint to connect to.
         * @param nod Parent node
         */
        NodeConnection(boost::asio::io_context &ioContext, boost::asio::ip::tcp::endpoint mEp, Node& nod);

        /**
         * connect to the specified node, starts the handshake
         * @param connected function to call when finished connecting
         */
        void connect(boost::function<void (boost::system::error_code)> connected);

        /**
         * Handles the connection finishing
         * @param ec error code
         * @param connected  connected function to call when finished connecting
         */
        void connect_handler(const boost::system::error_code& ec,boost::function<void(boost::system::error_code)> connected);

        /**
         * Main network handing function.
         */
        void network_loop();

        /**
         * Handles the messages
         */
        void handler_loop();

        /**
         * Handles pasrsing header from the stream.
         * @param ec Error code
         */
        void header_handler(const boost::system::error_code& ec);

        /**
         * Handles reading body from stream.
         * @param ec Error Code
         */
        void body_handler(const boost::system::error_code& ec);

        /**
         * Gets the node this connection is attached to.
         * @return Node
         */
        Node& getNode();

        /**
         * Is this node active.
         * @return true if node is active false if errored or disconnected.
         */
        [[nodiscard]] bool isActive() const { return active;};

    protected:
        /**
         * Instantiates the appropriate body type as per the header and returns it
         * @return Instantiated Message body
         */

         messages::MessageBodyPtr makeBody();

         void sendMessage();
    private:
        boost::asio::io_context & io_context;
        boost::asio::ip::tcp::endpoint m_ep;
        boost::asio::ip::tcp::socket m_sock;
        Node&  node;
        data::bytes incoming_header{24};
        data::bytes incoming_body;
        messages::Message incoming_message;
        std::queue<messages::Message> incoming_queue;
        std::queue<messages::Message> outgoing_queue;
        int recv_version;
        int send_version;
        bool active = false;

    };
}
#endif //GIGAMONKEY_NODE_CONNECTION_H
