// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/node.hpp>
#include <script/script.h>
#include <data/log/log.hpp>
#include <gigamonkey/p2p/node_connection.hpp>
#include <gigamonkey/p2p/messages/address_message.hpp>
#include <gigamonkey/p2p/messages/addressv2_message.hpp>

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

    void Node::startScanForNodes() {
        messages::EmptyBodyPtr body=boost::make_shared<messages::EmptyBody>(getParams(),"getaddr");
        messages::MessageHeadPtr  header=boost::make_shared<messages::MessageHeader>(getParams());
        messages::Message addrMsg(header,body);
        addrMsg.buildMessage();
        for (const NodeConnectionPtr &conn:active_connections) {
            conn->sendMessage(addrMsg);
        }

    }

    void Node::event_loop() {
        sortConnections();

        if(!active_connections.empty() && !temp) {
            startScanForNodes();
            temp=true;
        }

        int conns=active_connections.size()+starting_connections.size();
        if(conns < desired_connections) {
            int count=desired_connections-conns;
            count=std::min(count,(int)knownNodes.size());
            for(int i=0;i<count;i++) {
                messages::NetworkAddress addr=*knownNodes.begin();
                if(addr.getIpAddress().is_v4_mapped()) {
                    connect(addr.getIpAddress().to_v4());
                    DATA_LOG_CHANNEL("network",normal) << "Connecting to: " << addr;

                }
                knownNodes.erase(knownNodes.begin());
            }
        }

        for(const NodeConnectionPtr& conn:starting_connections)
        {
            conn->handler_loop();
        }

        for(const NodeConnectionPtr& conn:active_connections)
        {
            conn->handler_loop();
            while(!conn->getUndealtMessages().empty()) {
                messages::Message msg=conn->getUndealtMessages().front();
                conn->getUndealtMessages().pop();
                DATA_LOG_CHANNEL("network",normal) << "Message command " << msg.getHeader()->getCommandName();
                if(msg.getHeader()->getCommandName()=="addr") {
                    messages::AddressMessagePtr ptr=boost::dynamic_pointer_cast<messages::AddressMessage>(msg.getBody());
                    std::vector<messages::NetworkAddress> addrs=ptr->getAddresses();
                    for(const messages::NetworkAddress& addr:addrs) {
                        knownNodes.insert(addr);
                    }
                    DATA_LOG_CHANNEL("network",normal) << "We now know this many nodes " << knownNodes.size();

                }
            }

        }


        event_timer.async_wait([this](const boost::system::error_code& ec){event_loop();});
    }

    void Node::sortConnections() {
        std::list<NodeConnectionPtr> moving;
        for(NodeConnectionPtr conn:starting_connections) {
            if(!conn->isConnecting() && conn->isActive())
            {
                moving.push_back(conn);
                active_connections.push_back(conn);
            }
            else if(!conn->isConnecting() && !conn->isActive())
            {
                moving.push_back(conn);
                inactive_connections.push_back(conn);
            }
        }
        for(const NodeConnectionPtr& conn:moving) {
            auto findItr=std::find(starting_connections.begin(), starting_connections.end(), conn);
            if(findItr != starting_connections.end())
                starting_connections.erase(findItr);
        }
        moving.clear();


        for(NodeConnectionPtr conn:active_connections) {
            if(!conn->isActive()) {
                moving.push_back(conn);
                inactive_connections.push_back(conn);
            }

        }

        for(const NodeConnectionPtr& conn:moving) {
            auto findItr=std::find(active_connections.begin(), active_connections.end(), conn);
            if(findItr != active_connections.end())
                active_connections.erase(findItr);
        }
        moving.clear();
    }

    void Node::connect(const boost::asio::ip::address& connection) {
        NodeConnectionPtr  node_connection=boost::make_shared<NodeConnection>(io_context,boost::asio::ip::tcp::endpoint(connection,getParams().port()),this);
        starting_connections.push_back(node_connection);
        node_connection->connect([](boost::system::error_code ec){});
    }

    void Node::run() {
        io_context.run();
    }
}