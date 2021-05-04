// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_NETWORK_PARAMS_H
#define GIGAMONKEY_NETWORK_PARAMS_H

#include <vector>
#include <data/iterable.hpp>
#include <iostream>
#include <string>

namespace Gigamonkey::p2p {
    /**
     * Network Params
     *
     * Abstract Class
     */
    class NetworkParams {
    public:
        /**
         * Gets the seed domain list for the network
         * @return vector of strings
         */
        virtual inline std::vector<std::string> seeds()  = 0;

        /**
         * Gets the magic bytes for this network
         * @return vector of bytes
         */
        virtual inline data::bytes magic_bytes()  = 0;

        /**
         * Gets the port number for this network
         * @return Port number
         */
        virtual inline int port() = 0;
    };

    namespace Networks {

        /**
         * Main network
         */
        class MainNet : public NetworkParams {
        public:

            /**
            * Gets the seed domain list for the network
            * @return vector of strings
            */
            inline std::vector<std::string> seeds() override {
                return {"seed.bitcoinsv.io", "seed.cascharia.com", "seed.satoshisvision.network"};
            };

            /**
            * Gets the port number for this network
            * @return Port number
            */
            int port() override {
                return 8333;
            }

            /**
            * Gets the magic bytes for this network
            * @return vector of bytes
            */
            data::bytes magic_bytes() override {
                return {0xe3,0xe1,0xf3,0xe8};
            }
        };

        /**
         * Test Network
         */
        class TestNet : public NetworkParams {
        public:

            /**
            * Gets the seed domain list for the network
            * @return vector of strings
            */
            inline std::vector<std::string> seeds() override {
                return {"testnet-seed.bitcoincloud.net","testnet-seed.bitcoinsv.io", "testnet-seed.cascharia.com"};
            };

            /**
            * Gets the port number for this network
            * @return Port number
            */
            int port() override {
                return 18333;
            }

            /**
            * Gets the magic bytes for this network
            * @return vector of bytes
            */
            data::bytes magic_bytes() override {
                return {0xf4,0xe5,0xf3,0xf4};
            }
        };
    }
}
#endif //GIGAMONKEY_NETWORK_PARAMS_H
