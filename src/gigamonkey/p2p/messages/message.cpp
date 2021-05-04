// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/messages/message.hpp>

namespace Gigamonkey::p2p::messages {

    Message::operator data::bytes() const {
        data::bytes headerBytes=(data::bytes)*header;
        data::bytes bodyBytes=(data::bytes)*body;
        data::bytes whole=data::bytes(headerBytes.size()+bodyBytes.size());
        std::copy(headerBytes.begin(),headerBytes.end(),whole.begin());
        std::copy(bodyBytes.begin(), bodyBytes.end(),whole.begin()+headerBytes.size());
        return whole;
    }

    Message::Message(MessageHeadPtr head, MessageBodyPtr bdy) :header(head),body(bdy){

    }

    bool Message::valid() const {
        return header->valid() && header->getChecksum()==body->getChecksum();
    }

    void Message::buildMessage() {
        header->setCommandName(body->getCommandName());
        header->setPayloadSize(body->getSize());
        header->setChecksum(body->getChecksum());
    }

    std::ostream &operator<<(std::ostream &os, const Message &message) {
        os << "header: " << message.header << " body: " << message.body;
        return os;
    }
}