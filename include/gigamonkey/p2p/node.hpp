// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_NODE_H
#define GIGAMONKEY_NODE_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>
#include "network_params.hpp"



namespace Gigamonkey::p2p {

    /**
     * Base Node connecting to the network
     */
    class Node {
    public:
        /**
         * Constructs a node
         * @param net_params network to connect to
         */
        explicit Node(NetworkParams& net_params): io_context(),resolver(io_context),network(net_params) {};

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
    private:
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver;
        NetworkParams& network;
    };
}
#endif //GIGAMONKEY_NODE_H
