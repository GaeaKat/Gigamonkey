// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_P2P_MESSAGES_BLANKPAYLOAD_HPP_
#define GIGAMONKEY_P2P_MESSAGES_BLANKPAYLOAD_HPP_

#include <utility>

#include "messagePayload.hpp"
namespace Gigamonkey::Bitcoin::P2P::Messages {
	class BlankPayload: public MessagePayload {
	  public:
		explicit BlankPayload(Networks network): MessagePayload(network){};
		BlankPayload(data::bytes data,Networks network): MessagePayload(std::move(data),network){};
		explicit operator data::bytes() override {
			return {};
		}
	};
}
#endif //GIGAMONKEY_P2P_MESSAGES_BLANKPAYLOAD_HPP_