// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/node.hpp>
#include <script/script.h>
#include <data/log/log.hpp>

namespace Gigamonkey::p2p {


    std::vector<boost::asio::ip::address> Node::QuerySeeds() {
        std::vector<boost::asio::ip::address> output;

        boost::system::error_code ec;
        for(std::string host: network.seeds())
        {
            boost::asio::ip::tcp::resolver::query resolver_query(host,std::to_string(network.port()), boost::asio::ip::tcp::resolver::query::numeric_service);
            boost::asio::ip::tcp::resolver::iterator it =resolver.resolve(resolver_query, ec);
            if(ec.failed()) {
                DATA_LOG_CHANNEL("network",error) << "Failed to resolve a DNS name."
                          << "Error code = " << ec.value()
                          << ". Message = " << ec.message();
            }
            boost::asio::ip::tcp::resolver::iterator it_end;

            for (; it != it_end; ++it) {
                // Here we can access the endpoint like this.
                boost::asio::ip::tcp::endpoint ep = it->endpoint();
                output.push_back(ep.address());
            }

        }
        return output;

    }

    NetworkParams &Node::getParams() {
        return network;
    }
}