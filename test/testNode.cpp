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
#include <boost/log/utility/setup/console.hpp>
namespace Gigamonkey::p2p {

    void tmp(boost::system::error_code ec) {
        DATA_LOG(normal) << "Got here too";
        if(ec.failed()) {
            DATA_LOG(error) << "Failed to Connect";
        }
    }
    TEST(NodeTest, QuerySeeds) {
        data::log::init_logging("node_%0.log");
        boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%] [%Channel%] [%Severity%]: %Message%", boost::log::keywords::auto_flush = true);
        Networks::TestNet network;
        DATA_LOG(normal) << "Test logging";
        Node node(network);
        auto output=node.QuerySeeds();
        if(output.size()>0) {
            std::cout << output[0] << std::endl;
            node.connect(output[0]);
            node.run();
        }
    }
}