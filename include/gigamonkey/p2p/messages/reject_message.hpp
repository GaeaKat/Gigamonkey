// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_REJECT_MESSAGE_HPP
#define GIGAMONKEY_REJECT_MESSAGE_HPP
#include <gigamonkey/p2p/var_string.hpp>
#include <ostream>
#include "message_body.hpp"

namespace Gigamonkey::p2p::messages {

    class RejectMessage : public MessageBody {
    public:
        /**
         * Constructs a blank Reject Message Body
         * @param params Network Params for connection
         */
        explicit RejectMessage(NetworkParams& params);

        /**
         * Constructs a Reject Message from bytes
         * @param payload Bytes to construct message from
         * @param params Network Params for connection
         */
        explicit RejectMessage(data::bytes payload,NetworkParams& params);

        friend std::ostream &operator<<(std::ostream &os, const RejectMessage &message);
        /**
         * Gets the command name this message body is for.
         * @return string containing a 12 byte or less command name
         */
        inline std::string getCommandName() override {
            return "reject";
        }

    protected:
        void buildBody() override;

    private:
        VarString message;
        data::byte code;
        VarString reason;
        data::bytes extra;
    };
    typedef boost::shared_ptr<RejectMessage> RejectMessagePtr;
}
#endif //GIGAMONKEY_REJECT_MESSAGE_HPP
