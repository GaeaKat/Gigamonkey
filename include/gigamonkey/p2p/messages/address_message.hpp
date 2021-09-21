// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_ADDRESS_MESSAGE_HPP
#define GIGAMONKEY_ADDRESS_MESSAGE_HPP

#include <gigamonkey/p2p/var_string.hpp>
#include <ostream>
#include "message_body.hpp"
#include "network_address.hpp"

namespace Gigamonkey::p2p::messages {

    class AddressMessage : public MessageBody {

    public:

        /**
         * Constructs a blank Version Message Body
         * @param params Network Params for connection
         */
        explicit AddressMessage(NetworkParams& params);


        /**
         * Constructs a Version Message from bytes
         * @param payload bytes to construct from
         * @param params Network Params for connection
         */
        explicit AddressMessage(data::bytes payload,NetworkParams& params);

        /**
         * Gets the vector of NetworkAddresses in this message
         * @return vector containing the network addresses
         */
        [[nodiscard]] std::vector<NetworkAddress>& getAddresses()   {return addresses; }

        std::string getCommandName() override {
            return "addr";
        }

    protected:
        void buildBody() override;

    private:
        std::vector<NetworkAddress> addresses;

    };

    typedef boost::shared_ptr<AddressMessage> AddressMessagePtr;
}
#endif //GIGAMONKEY_ADDRESS_MESSAGE_HPP
