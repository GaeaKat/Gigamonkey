// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/messages/address_message.hpp>

namespace Gigamonkey::p2p::messages {

    void AddressMessage::buildBody() {
        auto countBytes=(data::bytes)VarInt(addresses.size());
        body=data::bytes(countBytes.size()+(30*countBytes.size()));
        auto it=body.begin();
        std::copy(countBytes.begin(), countBytes.end(),it);
        it+=countBytes.size();
        for(const NetworkAddress& addr:addresses){
            auto addrBytes=(data::bytes)addr;
            std::copy(addrBytes.begin(), addrBytes.end(),it);
            it+=addrBytes.size();
        }
    }

    AddressMessage::AddressMessage(NetworkParams &params) : MessageBody(params) {

    }

    AddressMessage::AddressMessage(data::bytes payload, NetworkParams &params): MessageBody(params) {
        auto it=payload.begin();
        VarInt count=VarInt(it);
        it+=count.size();
        for(int i=0;i<(uint64_t)count;i++) {
            NetworkAddress address(it);
            it+=30;
            addresses.push_back(address);
        }
    }
}