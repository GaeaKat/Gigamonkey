// Copyright (c) 2021 Katrina Knight
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#include <gigamonkey/paymail/paymail.hpp>
#include <ctre.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <arpa/inet.h>

namespace Gigamonkey::Bitcoin::Paymail {



    bytes Paymail::getScript(std::string address, int amount, std::string purpose) {
        return bytes();
    }


    bool Paymail::isValid(const std::string address) {
        static constexpr auto pattern = ctll::fixed_string{ "^[a-zA-Z0-9_.+\\-]+@[a-zA-Z0-9\\-]+\\.[a-zA-Z0-9\\-.]+$" };
        return ctre::match<pattern>(address );
    }

    std::pair<std::string, std::string> Paymail::splitAddress(std::string address) {
        if(isValid(address))
        {
            std::vector<string> split;
            boost::split(split, address,boost::is_any_of("@"));
            if(split.size()!=2)
                return {};
            else
                return {split[0],split[1]};
        }
        return {};
    }

    std::map<Capability, std::string> Paymail::getCapabilities(std::string domain, bool ignoreCache) {
        return std::map<Capability, std::string>();
    }

    std::string Paymail::getCapabilityServer(std::string domain) {
        std::string full_domain="_bsvalias._tcp.";
        full_domain.append(domain);
        full_domain.append(".");
        int retval=ub_resolve(ctx,full_domain.c_str(),3,1,&result
                   );
        if(retval!=0) {
            printf("resolve error: %s\n", ub_strerror(retval));
        }
        else {
            printf("qname: %s\n", result->qname);
            printf("qtype: %d\n", result->qtype);
            printf("qclass: %d\n", result->qclass);
            int num = 0;
            for(int i=0; result->data[i]; i++) {
                printf("result data element %d has length %d\n",
                       i, result->len[i]);
                printf("result data element %d is: %s\n",
                       i, result->data[i]);
                num++;
            }
            printf("result has %d data element(s)\n", num);
        }
        return std::string();
    }

    Paymail::Paymail(){
        ctx=ub_ctx_create();
        ub_ctx_resolvconf(ctx,nullptr);
        ub_ctx_hosts(ctx, nullptr);
    }


}