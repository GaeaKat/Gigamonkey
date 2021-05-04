// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_PROTOCONF_MESSAGE_H
#define GIGAMONKEY_PROTOCONF_MESSAGE_H

#include <gigamonkey/p2p/var_int.hpp>
#include <gigamonkey/p2p/var_string.hpp>
#include "message_body.hpp"

namespace Gigamonkey::p2p::messages {
    class ProtoconfMessage : public MessageBody {
    public:
        /**
         * Constructs a blank Protoconf Message Body
         * @param params Network Params for connection
         */
        explicit ProtoconfMessage(NetworkParams& params);

        /**
         * Constructs a Protoconf Message from bytes
         * @param payload Bytes to construct message from
         * @param params Network Params for connection
         */
        explicit ProtoconfMessage(data::bytes payload,NetworkParams& params);

        friend std::ostream &operator<<(std::ostream &os, const ProtoconfMessage &message);

        /**
         * Gets the command name this message body is for.
         * @return string containing a 12 byte or less command name
         */
        inline std::string getCommandName() override {
            return "protoconf";
        }

        /**
         * Gets the number of fields
         * @return number of fields the Protoconf message contains
         */
        [[nodiscard]] uint64_t getNumberOfFields() const {
            return (uint64_t)numberOfFields;
        }

        /**
         * Sets the number of fields the protocon contains
         * @param nof Number of fields
         */
        void setNumberOfFields(uint64_t nof) {
            numberOfFields=VarInt(nof);
            buildBody();
        }

        /**
         * Gets the Max Recv Payload Length
         * @return max Recieve Payload Length
         */
        [[nodiscard]] uint32_t getMaxRecvPayloadLength() const {
            return (uint32_t)maxRecvPayloadLength;
        }

        /**
         * Sets the Max Recv Payload Length
         * @param mrpl max Recieve Payload Length
         */
        void setMaxRecvPayloadLength(uint32_t mrpl) {
            maxRecvPayloadLength=mrpl;
            buildBody();
        }

        /**
         * Gets the stream Policies
         * @return Stream policies
         */
        [[nodiscard]] std::string getStreamPolicies() const {
            return (std::string)streamPolicies;
        }

        /**
         * Sets the stream policies
         * @param policies Stream Policies
         */
        void setStreamPolicies(std::string policies) {
            streamPolicies=VarString(policies);
            buildBody();
        }



    protected:
        void buildBody() override;

    private:
        VarInt numberOfFields;
        data::uint32_little maxRecvPayloadLength;
        VarString streamPolicies;
    };

    typedef boost::shared_ptr<ProtoconfMessage> ProtoconfMessagePtr;
}
#endif //GIGAMONKEY_PROTOCONF_MESSAGE_H
