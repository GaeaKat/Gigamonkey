// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "gtest/gtest.h"
#include <gigamonkey/p2p/node.hpp>
#include <gigamonkey/p2p/network_params.hpp>
#include <gmock/gmock.h>
#include <gigamonkey/p2p/node_connection.hpp>
#include <data/log/log.hpp>
#include <boost/bind/bind.hpp>
#include <gigamonkey/p2p/messages/message_header.hpp>
#include <gigamonkey/p2p/messages/message_body.hpp>
#include <gigamonkey/p2p/messages/version_message.hpp>
#include <gigamonkey/p2p/messages/message.hpp>
#include <gigamonkey/work/string.hpp>

namespace Gigamonkey::p2p {

    void tmp(boost::system::error_code ec) {
        std::cout << "Got here too" << std::endl;
        if(ec.failed()) {
            std::cout << "Failed to Connect" << std::endl;
        }
    }
    TEST(NodeTest, QuerySeeds) {
        data::log::init_logging("node_%0.log");
        Networks::TestNet network;
        Node node(network);
        auto output=node.QuerySeeds();
        std::cout << output[0] << std::endl;
        boost::asio::io_context io_context;
        NodeConnection test(io_context,boost::asio::ip::tcp::endpoint(output[0],network.port()),node);
        test.connect([](boost::system::error_code ec){tmp(ec);});
        io_context.run();

    }
}