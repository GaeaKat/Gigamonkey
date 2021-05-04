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
        /**
         * Gets the message type being rejected
         * @return name of the message not padded to 12 bytes
         */
        [[nodiscard]] std::string getMessage() const {
            return (std::string) message;
        }

        /**
         * Sets the message type being rejected
         * @param msg name of the message not padded to 12 bytes
         */
        void setMessage(std::string msg) {
            message=VarString(msg);
            buildBody();
        }

        /**
         * Gets the rejection code
         * @return rejection code
         */
        [[nodiscard]] data::byte getCode() const {
            return code;
        }

        /**
         * Sets the rejection code
         * @param cde rejection code
         */
        void setCode(data::byte cde) {
            code=cde;
            buildBody();
        }

        /**
         * Gets the rejection reason
         * @return reason the messasge is rejected
         */
        std::string getReason() const {
            return (std::string) reason;
        }

        /**
         * Sets the rejection reason
         * @param rson reason the messasge is rejected
         */
        void setReason(std::string rson) {
            reason=VarString(rson);
            buildBody();
        }

        /**
         * Gets the extra data
         * @return extra data
         */
        data::bytes getExtra() const {
            return extra;
        }

        /**
         * Sets the extra data
         * @param xtra extra data
         */
        void setExtra(data::bytes xtra) {
            extra=xtra;
            buildBody();
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
