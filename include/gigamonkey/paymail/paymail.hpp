// Copyright (c) 2021 Katrina Knight
// Distributed under the Open BSV software license, see the accompanying file LICENSE.


#ifndef GIGAMONKEY_PAYMAIL_H
#define GIGAMONKEY_PAYMAIL_H
#include <data/cross.hpp>
#include <gigamonkey/types.hpp>
#include "capability.hpp"
#include <unbound.h>

namespace Gigamonkey::Bitcoin::Paymail {

    /**
     * Class handling Paymail operations
     */
    class Paymail {
    public:
        Paymail();

    protected:
        struct ub_ctx* ctx;
        struct ub_result* result;
        /**
         * Splits a paymail address into user and domain.tld
         * @param address Address to split
         * @return a Pair containing first the user, and second the domain and tld
         */
        std::pair<std::string,std::string> splitAddress(std::string address);

        /**
         * gets the capabilities, and associated urls for a domain
         * @param domain
         * @param ignoreCache
         * @return
         */
        std::map<Capability,std::string> getCapabilities(std::string domain,bool ignoreCache = false);

        std::string getCapabilityServer(std::string domain);
    public:
        /**
         * Checks if a paymail is valid
         * @note This only checks if it matches. It does not use any validation services currently.
         * @param address Address to check
         * @return true if valid, false otherwise
         */
        static bool isValid(std::string address);

        /**
         *
         * @param address
         * @param amount
         * @param purpose
         * @return
         */
        bytes getScript(std::string address,int amount,std::string purpose="");
    };
}
#endif //GIGAMONKEY_PAYMAIL_H
