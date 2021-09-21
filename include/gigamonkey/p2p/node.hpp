// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_NODE_H
#define GIGAMONKEY_NODE_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>
#include "network_params.hpp"
#include <list>
#include <gigamonkey/p2p/node_connection.hpp>
#include <gigamonkey/p2p/messages/network_address.hpp>
#include <gigamonkey/p2p/p2p.hpp>
#include <set>

namespace Gigamonkey::p2p {


    /**
     * Base Node connecting to the network
     */
    class Node {
    public:
        /**
         * Constructs a node
         * @param net_params network to connect to
         * @param wished_connections Amount of connections to aim for in general
         */
        explicit Node(NetworkParams& net_params,int wished_connections=3): io_context(),resolver(io_context),network(net_params), event_timer(io_context,boost::asio::chrono::seconds(1)),desired_connections(wished_connections) {
            event_timer.async_wait([this](const boost::system::error_code& ec){event_loop();});
        };

        /**
         * Gets a list of ip addresses for the network first connection
         * @return vector of ip addresses
         */
        std::vector<boost::asio::ip::address> QuerySeeds();

        /**
         * Gets the network Parameters
         * @return
         */
        NetworkParams& getParams();

        /**
         * Starts the nodes asking for more nodes to connect to.
         */
        void startScanForNodes();


        /**
         * Main event loop for the nodes
         */
        void event_loop();

        /**
         * Runs the node loop
         */
        void run();

        /**
         * Connects a new connection
         * @param connection connection to make
         */
        void connect(const boost::asio::ip::address& connection);
    private:

        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver;
        boost::asio::steady_timer event_timer;
        NetworkParams& network;
        std::list<NodeConnectionPtr> active_connections;
        std::list<NodeConnectionPtr> inactive_connections;
        std::list<NodeConnectionPtr> starting_connections;
        std::set<messages::NetworkAddress> knownNodes;
        void sortConnections();
        bool temp=false;
        int desired_connections;
    };
}
#endif //GIGAMONKEY_NODE_H
