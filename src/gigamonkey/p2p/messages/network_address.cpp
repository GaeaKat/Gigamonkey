// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <gigamonkey/p2p/messages/network_address.hpp>

namespace Gigamonkey::p2p::messages {

    void NetworkAddress::buildBody() {
        body=data::bytes(30);
        auto it=body.begin();
        std::copy(time.begin(), time.end(),it);
        it+=4;
        std::copy(services.begin(), services.end(),it);
        it+=8;

        auto ipBytes=ipAddress.to_bytes();

        std::copy(ipBytes.begin(), ipBytes.end(),it);
        it+=ipBytes.size();
        std::copy(port.begin(), port.end(),it);
    }

    NetworkAddress::NetworkAddress() = default;


    NetworkAddress::operator data::bytes() const {
        return body;
    }

    NetworkAddress::NetworkAddress(data::bytes::iterator itr) {
        std::copy(itr,itr+4,time.begin());
        itr+=4;
        std::copy(itr,itr+8,services.begin());
        itr+=8;
        uint32_t ipBytes[4];
        boost::asio::ip::address_v6::bytes_type addrBytes;
        std::copy(itr,itr+16,addrBytes.begin());
        itr+=16;
        ipAddress=boost::asio::ip::address_v6(addrBytes,0);
        std::copy(itr,itr+2,port.begin());
        itr+=2;
    }

    std::ostream &operator<<(std::ostream &os, const NetworkAddress &address) {
        os << "NetworkAddress<"<< "time: " << address.time << ", services: " << address.services
           << ", ipAddress: " << address.ipAddress << ", port: " << address.port << ">";
        return os;
    }
}


