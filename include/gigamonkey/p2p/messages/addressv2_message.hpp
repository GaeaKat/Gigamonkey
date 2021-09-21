// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_ADDRESSV2_MESSAGE_H
#define GIGAMONKEY_ADDRESSV2_MESSAGE_H
#include <gigamonkey/p2p/var_string.hpp>
#include <ostream>
#include "message_body.hpp"

namespace Gigamonkey::p2p::messages {

    class AddressV2Message : public MessageBody {

    };
    typedef boost::shared_ptr<AddressV2Message> Addrv2MessagePtr;
}
#endif //GIGAMONKEY_ADDRESSV2_MESSAGE_H
