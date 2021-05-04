// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <gigamonkey/p2p/node.hpp>

#include <gmock/gmock.h>
#include <gigamonkey/p2p/var_int.hpp>



namespace Gigamonkey::p2p {
    TEST(VarInt,VarIntToBytes) {
        VarInt var(0xF0);
        EXPECT_EQ(var.size(),1);
        auto small=(uint8_t)var;

        VarInt var2(0xFF0);
        EXPECT_EQ(var2.size(),3);
        EXPECT_EQ(((data::bytes)var2)[0],0xFD);


        VarInt var3(0xFFFFE);
        EXPECT_EQ(var3.size(),5);
        EXPECT_EQ(((data::bytes)var3)[0],0xFE);

        VarInt var4(0xFFFFFFFFE);
        EXPECT_EQ(var4.size(),9);
        EXPECT_EQ(((data::bytes)var4)[0],0xFF);
    }

    TEST(VarInt,VarIntFromBytes) {

        data::bytes small=data::bytes(100);
        small[0]=0xF0;
        VarInt smallVI(small);
        EXPECT_EQ(smallVI.size(),1);
        EXPECT_EQ(((uint8_t)smallVI),0xF0);

        small[0]=0xFD;
        small[1]=0xFF;
        small[2]=0x0E;
        VarInt mediumVI(small);
        EXPECT_EQ(mediumVI.size(),3);
        EXPECT_EQ((uint16_t)mediumVI,0x0EFF);
    }
}