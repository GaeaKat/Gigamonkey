// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_MESSAGE_BODY_HPP
#define GIGAMONKEY_MESSAGE_BODY_HPP

#include <data/iterable.hpp>
#include <gigamonkey/p2p/network_params.hpp>
#include <ostream>
#include <boost/shared_ptr.hpp>
#include <utility>

namespace Gigamonkey::p2p::messages {
    /**
     * Body for a message
     *
     * Abstract class
     */
    class MessageBody {
    public:

        /**
         * Constructs a message body storing the Network Params
         * @param params Network Parameters
         */
        explicit MessageBody(NetworkParams& params);

        /**
         * Destructor for the base Message Body
         */
        virtual ~MessageBody();

        /**
         * Constructs the checksum from the built body
         * @return 4 byte checksum
         */
        data::bytes getChecksum();

        /**
         * Gets the size of the message body
         * @return Size of the message body
         */
        uint32_t getSize();

        /**
         * Gets the command name this message body is for.
         * @return string containing a 12 byte or less command name
         */
        virtual std::string getCommandName() = 0;

        /**
         * Casts the body to a byte vector
         * @return vector of bytes
         */
        explicit operator data::bytes() const;

        /**
         * Outputs the body to a stream
         * @param os  Output Stream
         * @param body Message body to output
         * @return Output Stream
         */
        friend std::ostream &operator<<(std::ostream &os, const MessageBody &body);

    protected:
        virtual void buildBody() = 0;
        data::bytes body;
        NetworkParams& params;
    };

    /**
     * Empty Message Body
     */
    class EmptyBody : public MessageBody {
    public:
        /**
         * Constructs an empty messasge body
         * @param params Network Params
         */
        EmptyBody(NetworkParams& params) : MessageBody(params) {
            body=data::bytes(0);
        }

        /**
         * Constructs an empty messasge body with a specific command name
         * @param params Network Params
         * @param name Name of the command
         */
        EmptyBody(NetworkParams& params,std::string name) : MessageBody(params),name(std::move(name)) {
            body=data::bytes(0);
        }
        /**
         * Gets the command name this message body is for.
         * @return string containing a 12 byte or less command name
         */
        std::string getCommandName() override;

    protected:
        void buildBody() override;
        std::string name;
    };

    typedef boost::shared_ptr<MessageBody> MessageBodyPtr;
}
#endif //GIGAMONKEY_MESSAGE_BODY_HPP
