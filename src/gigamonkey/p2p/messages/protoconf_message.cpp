// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/messages/protoconf_message.hpp>
#include <data/log/log.hpp>

namespace Gigamonkey::p2p::messages {

    ProtoconfMessage::ProtoconfMessage(NetworkParams &params): MessageBody(params), numberOfFields(0), maxRecvPayloadLength(0),
                                                               streamPolicies("") {}
    ProtoconfMessage::ProtoconfMessage(data::bytes payload, NetworkParams &params): MessageBody(params), numberOfFields(0), maxRecvPayloadLength(0),
                                                                                    streamPolicies("") {
        auto it=payload.begin();
        DATA_LOG(critical) << "Tryign to uinderstand " << payload;
        numberOfFields=VarInt(it);
        it+=numberOfFields.size();
        if(numberOfFields > VarInt(0)) {
            maxRecvPayloadLength=data::uint32_little();
            std::copy(it,it+4,maxRecvPayloadLength.begin());
            it+=4;
            if(numberOfFields > VarInt(1))
            {
                streamPolicies=VarString(it);
                it+=streamPolicies.getSize();
            }
        }
    }

    std::ostream &operator<<(std::ostream &os, const ProtoconfMessage &message) {
        os << "ProtoconfMessage<" << "NumberOfFields: " << message.numberOfFields << ", MaxRecvPayloadLength: " << message.maxRecvPayloadLength <<", streamPolicies: " << message.streamPolicies << ">";
        return os;
    }

    void ProtoconfMessage::buildBody() {
        body=data::bytes(numberOfFields.size()+4+streamPolicies.getSize());
        auto it=body.begin();
        auto nofBytes=(data::bytes)numberOfFields;
        auto mrplBytes=(data::bytes)maxRecvPayloadLength;
        auto spBytes=(data::bytes)streamPolicies;
        std::copy(nofBytes.begin(), nofBytes.end(),it);
        it+=numberOfFields.size();
        std::copy(mrplBytes.begin(), mrplBytes.end(),it);
        it+=4;
        std::copy(spBytes.begin(), spBytes.end(),it);
    }
}
