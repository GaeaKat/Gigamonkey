// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_PINGPONGMESSAGE_H
#define GIGAMONKEY_PINGPONGMESSAGE_H

#include "message_body.hpp"

namespace Gigamonkey::p2p::messages {
    class PingPongMessage : public MessageBody {
    public:
        /**
         * Constructs a blank Ping or Pong Message Body
         * @param params Network Params for connection
         */
        explicit PingPongMessage(NetworkParams& params,bool ping);

        /**
         * Constructs a Ping or Pong Message from bytes
         * @param payload Bytes to construct message from
         * @param params Network Params for connection
         */
        explicit PingPongMessage(data::bytes payload,NetworkParams& params,bool ping);

        friend std::ostream &operator<<(std::ostream &os, const PingPongMessage &message);
        /**
         * Gets the command name this message body is for.
         * @return string containing a 12 byte or less command name
         */
        inline std::string getCommandName() override {
            return ping? "ping" : "pong";
        }

        /**
         * Gets the nonce the ping/pong is using
         * @return uint 64 nonce
         */
        uint64_t getNonce() {
            return nonce;
        }

        /**
         * Sets the nonce the ping/pong is using
         * @param nce Nonce to set
         */
        void setNonce(uint64_t nce) {
            nonce=nce;
            buildBody();
        }

    protected:
        void buildBody() override;

    private:
        bool ping;
        data::uint64_little nonce;
    };

    typedef boost::shared_ptr<PingPongMessage> PingPongMessagePtr;
}
#endif //GIGAMONKEY_PINGPONGMESSAGE_H
