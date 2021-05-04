// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/messages/message_header.hpp>

#include <utility>

namespace Gigamonkey::p2p::messages {

    MessageHeader::MessageHeader(data::bytes head,NetworkParams& params) : header(std::move(head)), params(params),checksum(4) {
        std::copy(header.begin()+20,header.begin()+24,checksum.begin());
        char str[12];
        std::copy(header.begin()+4,header.begin()+16,std::begin(str));
        commandName=std::string(str);
        char size[4];
        std::copy(header.begin()+16,header.begin()+20,std::begin(size));
        //todo: Use endian type correctly
        auto* sizePointer= reinterpret_cast<uint32_t *>(&size);
        payloadSize=boost::endian::little_to_native(*sizePointer);
    }


    data::bytes MessageHeader::getChecksum() {
        return checksum;
    }

    uint32_t MessageHeader::getPayloadSize() {
        return payloadSize;
    }

    std::string MessageHeader::getCommandName() {
        return commandName;
    }

    MessageHeader::operator data::bytes() const {
        return header;
    }

    bool MessageHeader::valid() const {
        auto magic=params.magic_bytes();
        auto headerBytes=data::bytes(4);
        std::copy(header.begin(),header.begin()+4,headerBytes.begin());
        return magic==headerBytes;
    }

    void MessageHeader::setChecksum(data::bytes chk) {
        std::copy(chk.begin(),chk.begin()+4,checksum.begin());
        buildHeader();
    }

    void MessageHeader::setCommandName(std::string comm) {
        if(comm.size()>12)
            comm=comm.substr(0,12);
        commandName=comm;
        buildHeader();
    }

    void MessageHeader::setPayloadSize(uint32_t size) {
        payloadSize=size;
        buildHeader();
    }

    MessageHeader::MessageHeader(NetworkParams& params) : params(params),header(24),checksum(4) {
        buildHeader();
    }

    void MessageHeader::buildHeader() {
        data::bytes tmp(24);
        std::fill(tmp.begin(),tmp.end(),'\0');
        auto magic=params.magic_bytes();
        std::copy(magic.begin(),magic.end(),tmp.begin());
        std::copy(commandName.begin(),commandName.end(),tmp.begin()+4);
        std::copy(checksum.begin(), checksum.end(),tmp.begin()+20);
        std::copy(payloadSize.begin(),payloadSize.end(),tmp.begin()+16);
        std::copy(tmp.begin(), tmp.end(),header.begin());
    }

    std::ostream &operator<<(std::ostream &os, const MessageHeader &header) {
        os << " commandName: " << header.commandName << " payloadSize: " << header.payloadSize << " checksum: "
        << header.checksum;
        return os;
    }
}