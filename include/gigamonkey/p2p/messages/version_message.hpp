// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_VERSION_MESSAGE_H
#define GIGAMONKEY_VERSION_MESSAGE_H

#include <gigamonkey/p2p/var_string.hpp>
#include <ostream>
#include "message_body.hpp"

namespace Gigamonkey::p2p::messages {
    /**
     * Version Message Body for p2p
     */
    class VersionMessage : public MessageBody {
    public:
        /**
         * Constructs a blank Version Message Body
         * @param params Network Params for connection
         */
        explicit VersionMessage(NetworkParams& params);


        /**
         * Constructs a Version Message from bytes
         * @param payload bytes to construct from
         * @param params Network Params for connection
         */
        explicit VersionMessage(data::bytes payload,NetworkParams& params);

        /**
         * Gets the Protocol version set in the message
         * @return protocol version.
         */
        [[nodiscard]] int32_t getProtocolVersion() const { return version;}

        /**
         * sets the Protocol version for the message
         * @param ver version to set
         */
        void setProtocolVersion(const int32_t ver) { version=ver; buildBody();}

        /**
         * Gets the services advertised in this version message
         * @return services bitfield
         */
        [[nodiscard]] uint64_t getServices() const { return services;}

        /**
         * Sets the services bitfield advertised by this version message
         * @param serv services bitfield
         */
        void setServices(const uint64_t serv) { services=serv;  buildBody();}

        /**
         * Gets the timestamp of the version message.
         *
         * This needs to be in standard epoch time
         * @note Nodes will kick if this is more then 2 hours different
         * @return node timestamp
         */
        [[nodiscard]] uint64_t getTimestamp() const { return timestamp;}

        /**
         * Sets the timestamp of the version message
         *
         * This needs to be in standard epoch time
         * @note Nodes will kick if this is more then 2 hours different
         * @param time timestamp to set
         */
        void setTimestamp(const uint64_t  time) { timestamp = time;  buildBody();}

        /**
         * Gets the services supported by recieving node, as percieved bby transmitting node
         * @return services bitfield
         */
        [[nodiscard]] uint64_t getAddrRecvServices() const { return addr_recv_services;};
        /**
         * Sets the services supported by recieving node, as percieved by transmitting node
         * @param serv services bitfield
         */
        void setAddrRecvServices(const uint64_t serv) { addr_recv_services=serv; buildBody();}

        /**
         * Gets the recieving IP address as percieved by the transmitting node
         * @return byte vector containing the ip address in ipv6 form
         */
        [[nodiscard]] data::bytes getAddrRecvIpAddress() const { return addr_recv_ipaddress;}

        /**
         * Sets the recieving IP address as percieved by the transmitting node
         * @param aripAddress byte vector containing the ip address in ipv6 form
         */
        void setAddrRecvIpAddress(const data::bytes& aripAddress) { addr_recv_ipaddress = aripAddress; buildBody();}

        /**
         * Gets the recieving port number as percieved by the transmitting node
         * @return port number
         */
        [[nodiscard]] uint16_t getAddrRecvPort() const { return addr_recv_port;}

        /**
         * Sets the recieving port number, as percieved by the transmitting node
         * @param prt port number
         */
        void setAddrRecvPort(const uint16_t prt) { addr_recv_port=prt; buildBody();}


        /**
         * Gets the services supported by node
         *
         * Should be identical to @var services
         * @return services bitfield
         */
        [[nodiscard]] uint64_t getAddrTransServices() const { return addr_trans_services;};
        /**
         * Sets the services supported by node.
         *
         * Should be identical to @var services
         * @param serv services bitfield
         */
        void setAddrTransServices(const uint64_t serv) { addr_trans_services=serv; buildBody();}

        /**
         * Gets the transmitting node IP address
         * @return byte vector containing the ip address in ipv6 form
         */
        [[nodiscard]] data::bytes getAddrTransIpAddress() const { return addr_trans_ipaddress;}

        /**
         * Sets the transmitting node IP address
         * @param aripAddress byte vector containing the ip address in ipv6 form
         */
        void setAddrTransIpAddress(const data::bytes& aripAddress) { addr_trans_ipaddress = aripAddress; buildBody();}

        /**
         * Gets the transmitting node port number
         * @return port number
         */
        [[nodiscard]] uint16_t getAddrTransPort() const { return addr_trans_port;}

        /**
         * Sets the transmitting node port number
         * @param prt port number
         */
        void setAddrTransPort(const uint16_t prt) { addr_trans_port=prt; buildBody();}


        /**
         * Gets the Nonce value for this message.
         *
         * if nonce returned is non zero and one sent before, disconnect.
         * @return Nonce value
         */
        [[nodiscard]] uint64_t getNonce() const { return nonce;}

        /**
         * Sets the Nonce value for this message.
         * @param non Nonce set
         */
        void setNonce(const uint64_t non) { nonce=non; buildBody();}

        /**
         * Gets the user agent String for this message.
         * @return user agent for this message
         */
        [[nodiscard]] std::string getUserAgent() const { return (std::string)user_agent;}

        /**
         * Sets the user agent for this connection
         * @param agent user agent to set
         */
        void setUserAgent(const std::string agent) {user_agent=VarString(agent); buildBody();}

        /**
         * Gets the height of the node's best block chain
         * @return int of the height of the chain
         */
        [[nodiscard]] int32_t getStartHeight() const { return start_height;}

        /**
         * Sets the height of the node's best block chain
         * @param start height of the best block chain
         */
        void setStartHeight(const int32_t start) { start_height=start; buildBody();}

        /**
         * Gets the relay boolean
         * @return true for node being a relay node, false otherwise
         */
        [[nodiscard]] bool getRelay() const {return relay;}

        /**
         * Sets the relay boolean
         * @param rely true if node is a relay node, false otherwise
         */
        void setRelay(const bool rely) { relay=rely; buildBody();}


        /**
         * Outputs a VersionMessage to a stream
         * @param os Output stream
         * @param message message to output
         * @return Output stream
         */
        friend std::ostream &operator<<(std::ostream &os, const VersionMessage &message);

        /**
         * Gets the command name this message body is for.
         * @return string containing a 12 byte or less command name
         */
        inline std::string getCommandName() override {
            return "version";
        }

    protected:
        void buildBody() override;
        data::int32_little version;
        data::uint64_little services;
        data::uint64_little timestamp;
        data::uint64_little addr_recv_services;
        data::bytes addr_recv_ipaddress;
        data::uint16_big  addr_recv_port;
        data::uint64_little addr_trans_services;
        data::bytes addr_trans_ipaddress;
        data::uint16_big  addr_trans_port;
        data::uint64_little nonce;
        VarString user_agent;
        data::int32_little start_height;
        bool relay;
    };
    typedef boost::shared_ptr<VersionMessage> VersionMessagePtr;
}
#endif //GIGAMONKEY_VERSION_MESSAGE_H
