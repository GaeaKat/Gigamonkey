// Copyright (c) 2021 Katrina Knight
// Distributed under the Open BSV software license, see the accompanying file LICENSE.


#include <gigamonkey/paymail/paymail.hpp>
#include "gtest/gtest.h"

namespace Gigamonkey::Bitcoin::Paymail {

class PayMailTest: public Paymail, public testing::Test
{

};
    TEST_F(PayMailTest,ValidateAddress) {
        EXPECT_TRUE( isValid("nekotest3000@simply.cash")) ;
        EXPECT_FALSE(isValid("nekotest3000simply.cash"));
        EXPECT_FALSE(isValid("nekotest3000@simply"));
        EXPECT_FALSE(isValid("simply.cash"));
    }
    TEST_F(PayMailTest,ValidateSplit) {
        auto cur=splitAddress("nekotest3000@simply.cash");
        EXPECT_EQ(cur.first,"nekotest3000");
        EXPECT_EQ(cur.second,"simply.cash");

        auto nxt=splitAddress("nekotest3000simply.cash");
        EXPECT_EQ(nxt.first,"");
        EXPECT_EQ(nxt.second,"");

    }

    TEST_F(PayMailTest, GetCapabilitiesServer) {
        auto tmp= getCapabilityServer("simply.cash");
    }
}