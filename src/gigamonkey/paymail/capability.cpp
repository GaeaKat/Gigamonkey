// Copyright (c) 2021 Katrina Knight
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#include <gigamonkey/paymail/capability.hpp>

namespace Gigamonkey::Bitcoin::Paymail {

    std::string getCapabilityId(Capability cap) {
        {
            switch (cap) {
                case basicAddressResolution:
                    return "759684b1a19a";
                case p2pPaymentDestination:
                    return "2a40af698840";
                case p2pTransactions:
                    return "5f1323cddf31";
                case paymentDestination:
                    return "paymentDestination";
                case payToProtocolPrefix:
                    return "7bd25e5a1fc6";
                case pki:
                    return "pki";
                case pkiAlternate:
                    return "0c4339ef99c2";
                case publicProfile:
                    return "f12f968c92d6";
                case receiverApprovals:
                    return "3d7c2ca83a46";
                case senderValidation:
                    return "6745385c3fc0";
                case verifyPublicKeyOwner:
                    return "a9f510c16bde";
                default:
                    return "";
                case p2pPaymentDestinationWithToken:
                    return "f792b6eff07a";
                case sfpAssetInformation:
                    return "1300361cb2d4";
                case sfpAuthorizeAction:
                    return "95dddb461bff";
                case sfpBuildAction:
                    return "189e32d93d28";
            }
        }
    }
}
