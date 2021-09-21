// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_MESSAGE_HPP
#define GIGAMONKEY_MESSAGE_HPP

#include <ostream>
#include <utility>
#include "message_header.hpp"
#include "message_body.hpp"

namespace Gigamonkey::p2p::messages {

    /**
     * Full network p2p message
     */
    class Message {
    public:
        /**
         * Constructs a message from an existing Header and Body
         * @param head Head to construct message with
         * @param bdy body to construct message with
         */
        Message(MessageHeadPtr head,MessageBodyPtr bdy);

        Message() {};

        /**
         * Casts the message to a byte vector.
         * @warning build the message before this every time.
         * @return byte vector containing the message
         */
        explicit operator data::bytes() const;

        /**
         * Builds a message, mostly consists of setting the checksum and size in the header.
         */
        void buildMessage();

        /**
         * Is the message valid.
         *
         * This checks if the message has the correct magic bytes for the current network, and the checksum in the header matches the body.
         * @return true if valid false otherwise
         */
        [[nodiscard]] bool valid() const;

        /***
         * Gets the attached Header
         * @return MessageHeader object
         */
        MessageHeadPtr getHeader() { return header;};

        /**
         * Gets the attached body
         * @return MessageBody object
         */
        MessageBodyPtr getBody() { return body;};

        void setHeader(MessageHeadPtr head) { header=std::move(head);}

        void setBody(MessageBodyPtr bdy) { body=std::move(bdy);}

        friend std::ostream &operator<<(std::ostream &os, const Message &message);

        /**
         * Instantiates the appropriate body type as per the header and returns it
         * @return Instantiated Message body
         */
        messages::MessageBodyPtr makeBody(data::bytes incoming_body,NetworkParams& params);

    private:
        MessageHeadPtr header;
        MessageBodyPtr body;
    };
}
#endif //GIGAMONKEY_MESSAGE_HPP
