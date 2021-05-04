// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/messages/reject_message.hpp>

namespace Gigamonkey::p2p::messages {

    void RejectMessage::buildBody() {
        int size=message.getSize()+reason.getSize()+1+extra.size();
        body=data::bytes(size);
        auto messageBytes=(data::bytes)message;
        auto reasonBytes=(data::bytes)reason;
        auto it=body.begin();
        std::copy(messageBytes.begin(), messageBytes.end(),it);
        it+=messageBytes.size();
        *it=code;
        it++;
        std::copy(reasonBytes.begin(), reasonBytes.end(),it);
        it+=reasonBytes.size();
        std::copy(extra.begin(), extra.end(),it);
    }

    RejectMessage::RejectMessage(NetworkParams &params) : MessageBody(params),code(0), message(""),reason("") {

    }

    RejectMessage::RejectMessage(data::bytes payload, NetworkParams &params):MessageBody(params),code(0), message(""),reason("") {
        auto it=payload.begin();
        message=VarString(it);
        it+=message.getSize();
        code=*it;
        it++;
        reason=VarString(it);
        it+=reason.getSize();
        int remaining=payload.size()-(message.getSize()+1+reason.getSize());
        extra=data::bytes(remaining);
        std::copy(it,payload.end(),extra.begin());

    }

    std::ostream &operator<<(std::ostream &os, const RejectMessage &message) {
        os << "RejectMessage<" << "message: " << message.message << ", code: 0x" << std::hex << (int)message.code
           << ", reason: " << message.reason << ", extra: " << message.extra << ">";
        return os;
    }
}