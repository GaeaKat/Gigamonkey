// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/messages/message_body.hpp>
#include <data/types.hpp>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hmac.h>
#include <cryptopp/files.h>
#include <cryptopp/sha.h>


namespace Gigamonkey::p2p::messages {

    data::bytes MessageBody::getChecksum() {
        data::bytes checksum(4);
        byte abDigest[CryptoPP::SHA256::DIGESTSIZE];
        CryptoPP::SHA256().CalculateDigest(abDigest, body.data(), body.size());
        byte abDigest2[CryptoPP::SHA256::DIGESTSIZE];
        CryptoPP::SHA256().CalculateDigest(abDigest2, abDigest, CryptoPP::SHA256::DIGESTSIZE);
        std::copy(std::begin(abDigest2),std::begin(abDigest2)+4,checksum.begin());
        return checksum;
    }

    uint32_t MessageBody::getSize() {
        return body.size();
    }

    MessageBody::operator data::bytes() const {
        return body;
    }

    MessageBody::MessageBody(NetworkParams& params) :params(params){

    }

    std::ostream &operator<<(std::ostream &os, const MessageBody &body) {
        os << "body: " << body.body;
        return os;
    }

    MessageBody::~MessageBody() = default;

    void EmptyBody::buildBody() {

    }

    std::string EmptyBody::getCommandName() {
        return name;
    }

}
