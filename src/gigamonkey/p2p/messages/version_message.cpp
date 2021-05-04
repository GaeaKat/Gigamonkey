// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/messages/version_message.hpp>
namespace Gigamonkey::p2p::messages {

    void VersionMessage::buildBody() {
        data::bytes retVal=data::bytes(84+user_agent.getSize()+(version>=70001?1:0));
        auto it=retVal.begin();
        std::copy(version.begin(), version.end(),it);
        it+=4;
        std::copy(services.begin(), services.end(),it);
        it+=8;
        std::copy(timestamp.begin(), timestamp.end(),it);
        it+=8;
        std::copy(addr_recv_services.begin(),addr_recv_services.end(),it);
        it+=8;
        std::copy(addr_recv_ipaddress.begin(),addr_recv_ipaddress.end(),it);
        it+=16;
        std::copy(addr_recv_port.begin(), addr_recv_port.end(),it);
        it+=2;
        std::copy(addr_trans_services.begin(),addr_trans_services.end(),it);
        it+=8;
        std::copy(addr_trans_ipaddress.begin(),addr_trans_ipaddress.end(),it);
        it+=16;
        std::copy(addr_trans_port.begin(), addr_trans_port.end(),it);
        it+=2;
        std::copy(nonce.begin(), nonce.end(),it);
        it+=8;
        data::bytes user=(data::bytes)user_agent;
        std::copy(user.begin(),user.end(),it);
        int siz=user.size();
        it+=siz;
        std::copy(start_height.begin(), start_height.end(),it);
        it+=4;
        if(version>=70001)
        {
            retVal[retVal.size()-1]=(relay?0x01:0x00);
        }
        body=data::bytes(retVal.size());
        std::copy(retVal.begin(),retVal.end(),body.begin());
    }

    VersionMessage::VersionMessage(NetworkParams& params) : MessageBody(params), user_agent("") {}

    std::ostream &operator<<(std::ostream &os, const VersionMessage &message) {
        os << "Message <" << "version: " << message.version << ", services: "
           << message.services << ", timestamp: " << message.timestamp << ", addr_recv_services: "
           << message.addr_recv_services << ", addr_recv_ipaddress: " << message.addr_recv_ipaddress
           << ", addr_recv_port: " << message.addr_recv_port << ", addr_trans_services: " << message.addr_trans_services
           << ", addr_trans_ipaddress: " << message.addr_trans_ipaddress << ", addr_trans_port: "
           << message.addr_trans_port << ", nonce: " << message.nonce << ", user_agent: " << message.user_agent
           << ", start_height: " << message.start_height << ", relay: " << message.relay << ">";
        return os;
    }

    VersionMessage::VersionMessage(data::bytes bytes, NetworkParams &params) : MessageBody(params), user_agent(""){

        auto it=bytes.begin();
        std::copy(it,it+4,version.begin());
        it+=4;
        std::copy(it,it+8,services.begin());
        it+=8;
        std::copy(it,it+8,timestamp.begin());
        it+=8;
        std::copy(it,it+8,addr_recv_services.begin());
        it+=8;
        addr_recv_ipaddress=data::bytes(16);
        std::copy(it,it+16,addr_recv_ipaddress.begin());
        it+=16;
        std::copy(it,it+2,addr_recv_port.begin());
        it+=2;

        std::copy(it,it+8,addr_trans_services.begin());
        it+=8;
        addr_trans_ipaddress=data::bytes(16);
        std::copy(it,it+16,addr_trans_ipaddress.begin());
        it+=16;
        std::copy(it,it+2,addr_trans_port.begin());
        it+=2;
        std::copy(it,it+8,nonce.begin());
        it+=8;
        user_agent=VarString(it);
        it+=user_agent.getSize();
        std::copy(it,it+4,start_height.begin());
        it+=4;
        if(version>=70001 && it!=bytes.end()) {
            data::byte rel = *it;
            relay = rel == 0x01;
        }
        body=data::bytes(bytes.size());
        std::copy(bytes.begin(),bytes.end(),body.begin());
    }
}

