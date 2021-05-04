// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <gigamonkey/p2p/messages/ping_pong_message.hpp>

namespace Gigamonkey::p2p::messages {

    PingPongMessage::PingPongMessage(NetworkParams &params, bool ping): MessageBody(params),ping(ping)
    {}

    PingPongMessage::PingPongMessage(data::bytes payload, NetworkParams &params, bool ping): MessageBody(params),ping(ping) {
        nonce=data::uint64_little();
        std::copy(payload.begin(),payload.end(),nonce.begin());
    }

    void PingPongMessage::buildBody() {
        body=data::bytes(8);
        std::copy(nonce.begin(), nonce.end(),body.begin());

    }

    std::ostream &operator<<(std::ostream &os, const PingPongMessage &message) {
        os << "PingPongMessage<" << "ping: " << message.ping << ", nonce: 0x" << std::hex << (uint64_t)message.nonce << ">";
        return os;
    }


}

