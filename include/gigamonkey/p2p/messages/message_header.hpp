// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_MESSAGE_HEADER_HPP
#define GIGAMONKEY_MESSAGE_HEADER_HPP

#include <data/iterable.hpp>
#include <data/encoding/endian.hpp>
#include <gigamonkey/p2p/network_params.hpp>
#include <ostream>
#include <boost/shared_ptr.hpp>
class NodeConnection;

namespace Gigamonkey::p2p::messages {

    /**
     * Basic Message Header
     */
    class MessageHeader {
    public:
        /**
         * Constructs a message header from byte vector
         * @param head byte vector
         * @param params network Params
         */
        MessageHeader(data::bytes head,NetworkParams& params);

        /**
         * Constructs a blank network header
         * @param params Network Params
         */
        MessageHeader(NetworkParams& params);

        /**
         * Is this header valid
         *
         * Checks the magic bytes are appropriate to the network
         * @return true if valid false otherwise
         */
        bool valid() const;

        /**
         * Gets the checksum for the header
         * @return 4 byte byte vector
         */
        data::bytes getChecksum();

        /**
         * Gets the payload size
         * @return size of the message body
         */
        uint32_t getPayloadSize();

        /**
         * Gets the command name
         * @return name of the message type
         */
        std::string getCommandName();


        /**
         * Sets the checksum bytes
         * @param chk Checksum bytes
         */
        void setChecksum(data::bytes chk);

        /**
         * Sets the payload size
         * @param size Mesage body size
         */
        void setPayloadSize(uint32_t size);

        /**
         * Sets the command name
         * @param comm command name
         */
        void setCommandName(std::string comm);

        /**
         * Casts the header to a byte vector
         * @return byte vector
         */
        operator data::bytes() const;
    private:
        void buildHeader();
        NetworkParams& params;
        data::bytes header;
        std::string commandName;
        data::uint32_little payloadSize{};
        mutable data::bytes checksum;
    public:
        friend std::ostream &operator<<(std::ostream &os, const MessageHeader &header);
    };

    typedef boost::shared_ptr<MessageHeader> MessageHeadPtr;
}
#endif //GIGAMONKEY_MESSAGE_HEADER_HPP
