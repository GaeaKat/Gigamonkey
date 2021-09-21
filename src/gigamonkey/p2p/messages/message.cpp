// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/messages/message.hpp>
#include <gigamonkey/p2p/messages/message_header.hpp>
#include <gigamonkey/p2p/messages/message.hpp>
#include <gigamonkey/p2p/messages/reject_message.hpp>
#include <gigamonkey/p2p/messages/ping_pong_message.hpp>
#include <gigamonkey/p2p/messages/protoconf_message.hpp>
#include <gigamonkey/p2p/messages/address_message.hpp>
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#include <data/log/log.hpp>
#include <gigamonkey/p2p/messages/version_message.hpp>

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
        os << "header: " << *(message.header) << " body: " << *(message.body);
        return os;
    }

    messages::MessageBodyPtr  Message::makeBody(data::bytes incoming_body,NetworkParams& params) {

        std::string messageType=getHeader()->getCommandName();
        if(messageType == "reject")
        {
            setBody(boost::make_shared<RejectMessage>(incoming_body,params));
        }
        else if(messageType == "version")
        {
            setBody(boost::make_shared<VersionMessage>(incoming_body,params));
        }
        else if(messageType == "verack") {
            setBody(boost::make_shared<EmptyBody>(params,"verack"));
        }
        else if(messageType == "ping") {
            setBody(boost::make_shared<PingPongMessage>(incoming_body,params,true));
        }
        else if(messageType == "pong") {
            setBody(boost::make_shared<PingPongMessage>(incoming_body,params,false));
        }
        else if(messageType == "protoconf") {
            setBody(boost::make_shared<ProtoconfMessage>(incoming_body,params));
        }
        else if(messageType == "addr") {
            setBody(boost::make_shared<AddressMessage>(incoming_body,params));
        }
        else {
            DATA_LOG_CHANNEL("network",error) << "Recieved unknown message: " << messageType;
            setBody(boost::make_shared<EmptyBody>(params));
        }
        return getBody();
    }
}