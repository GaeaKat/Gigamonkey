// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_NETWORK_ADDRESS_HPP
#define GIGAMONKEY_NETWORK_ADDRESS_HPP
#include <utility>
#include <gigamonkey/p2p/var_string.hpp>
#include <gigamonkey/p2p/var_int.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <ostream>

namespace Gigamonkey::p2p::messages {

    /**
     * Network address as sent by server
     */
    class NetworkAddress{
    protected:
        void buildBody();
    public:

        /**
         * Constructs a network address
         */
        NetworkAddress();

        /**
         * Constructs a network address from bytes.
         * @param bytes bytes to construct address from.
         */
        explicit NetworkAddress(data::bytes bytes): NetworkAddress(bytes.begin()) {};

        /**
         * Constructs a network address from bytes iterator.
         * @param itr bytes iterator to construct address from.
         */
        explicit NetworkAddress(data::bytes::iterator itr);
        /**
         * Gets the time last seen this ip
         * @return time in unix epoch time format
         */
        [[nodiscard]] uint32_t getTime() const { return time;}

        /**
         * Sets the time last seen this ip
         * @param tme time in unix epoch time format
         */
        void setTime(uint32_t tme) { time=tme; buildBody();}

        /**
         * Gets the services offered by this ip
         * @return services offered
         */
        [[nodiscard]] uint64_t getServices() const { return services;}

        /**
         * Sets the services offered by this ip
         * @param srv services offered
         */
         void setServices(uint64_t srv) { services=srv; buildBody();}

         /**
          * Gets the IP address of this node
          *
          * @note This will be in IPv6 any IPv4 Nodes will be mapped
          * @return ip address of the node
          */
         [[nodiscard]] boost::asio::ip::address_v6 getIpAddress() const { return ipAddress;}

         /**
          * Sets the IP address of the node
          *
          * @note This will be in IPv6 any IPv4 Nodes will be mapped
          * @param addr ip address of the node
          */
         void setIpAddress(boost::asio::ip::address_v6 addr) {ipAddress=std::move(addr); buildBody(); }

         /**
          * Gets the port of the node
          * @return port node runs on
          */
        [[nodiscard]] uint16_t getPort() const { return port;}

        /**
         * Sets the port of the node
         * @param prt port node runs on.
         */
        void setPort(uint16_t prt) { port=prt; buildBody();}

        /**
         * Casts the Network Address to a byte vector
         * @return byte vector
         */
        operator data::bytes() const;

        friend std::ostream &operator<<(std::ostream &os, const NetworkAddress &address);

        bool operator<(const NetworkAddress &rhs) const {
            return time < rhs.time;
        }

        bool operator>(const NetworkAddress &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const NetworkAddress &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const NetworkAddress &rhs) const {
            return !(*this < rhs);
        }

        bool operator==(const NetworkAddress &rhs) const {
            return std::tie(time, services, ipAddress, port) ==
                   std::tie(rhs.time, rhs.services, rhs.ipAddress, rhs.port);
        }

        bool operator!=(const NetworkAddress &rhs) const {
            return !(rhs == *this);
        }

    private:
        data::bytes body;
        data::uint32_little time{};
        data::uint64_little services{};
        boost::asio::ip::address_v6 ipAddress;
        data::uint16_big port{};
    };
}
#endif //GIGAMONKEY_NETWORK_ADDRESS_HPP
